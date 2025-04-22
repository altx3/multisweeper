import { useParams, useNavigate } from "react-router-dom";
import { useEffect, useState } from "react";
import useWebSocket from "react-use-websocket";

import { Button, Typography } from "@mui/material";
import DefaultBox from "../components/common/DefaultBox";

import { CreateLobbyResponse } from "../types/typings";

const checkLobbyExists = async (lobbyId: string) => {
  try {
    const response = await fetch(
      `${import.meta.env.VITE_SERVER_HTTP_URL}lobbies/${lobbyId}`,
      {
        method: "GET",
        headers: { "Content-Type": "application/json" },
      }
    );
    if (!response.ok) {
      if (response.status === 404) {
        throw new Error("Lobby does not exist");
      }
      throw new Error("Failed to check lobby");
    }
    return await response.json();
  } catch (error) {
    console.error("Error checking lobby:", error);
    throw error;
  }
};

// New function to join the lobby
const joinLobby = async (lobbyId: string) => {
  try {
    const response = await fetch(
      `${import.meta.env.VITE_SERVER_HTTP_URL}lobbies/join/${lobbyId}`,
      {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ lobby_id: lobbyId }),
      }
    );
    if (!response.ok) {
      throw new Error("Failed to join lobby: " + response.status);
    }
    const data: CreateLobbyResponse = await response.json(); // { player_id: "player_123" }
    sessionStorage.setItem("player_id", data.player_id); // Store in sessionStorage
    return data.player_id;
  } catch (error) {
    console.error("Error joining lobby:", error);
    throw error;
  }
};

const Lobby = () => {
  const { lobbyId } = useParams<{ lobbyId: string }>();
  const navigate = useNavigate();
  const [isValidLobby, setIsValidLobby] = useState<boolean | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [playerId, setPlayerId] = useState<string | null>(
    sessionStorage.getItem("player_id")
  );

  // Check lobby existence
  useEffect(() => {
    if (!lobbyId) {
      setError("No lobby ID provided");
      setIsValidLobby(false);
      return;
    }

    checkLobbyExists(lobbyId)
      .then(() => {
        setIsValidLobby(true);
        setError(null);
        // If no player_id, join the lobby
        if (!playerId) {
          joinLobby(lobbyId)
            .then((newPlayerId) => {
              console.log("yoza " + lobbyId);
              console.log("gyoza " + newPlayerId);

              setPlayerId(newPlayerId);
              sendMessage(
                JSON.stringify({ type: "initDirect", lobbyId, newPlayerId })
              );
            })
            .catch((err) => {
              setError(err.message || "Failed to join lobby");
              setIsValidLobby(false);
            });
        }
      })
      .catch((err) => {
        setIsValidLobby(false);
        setError(err.message || "Failed to check lobby");
      });
  }, [lobbyId, playerId]);

  // WebSocket connection
  const { readyState, lastMessage, sendMessage } = useWebSocket(
    isValidLobby && lobbyId
      ? `${import.meta.env.VITE_SERVER_WS_URL}lobbies/${lobbyId}`
      : null,
    {
      shouldReconnect: () => true,
      reconnectAttempts: 10,
      reconnectInterval: 3000,
      onOpen: () => {
        console.log(`WebSocket opened for lobby ${lobbyId}`);
        if (playerId != null) {
          console.log(playerId);
          sendMessage(JSON.stringify({ type: "init", lobbyId, playerId }));
        }
      },
      onClose: () => {
        sessionStorage.removeItem("player_id"); // Clean up on leave
        console.log(`WebSocket closed for lobby ${lobbyId}`);
      },
    }
  );

  // Log connection status (for debugging)
  useEffect(() => {
    if (readyState === WebSocket.OPEN) {
      console.log(`WebSocket connected to lobby ${lobbyId}`);
    } else if (readyState === WebSocket.CLOSED) {
      console.log(`WebSocket disconnected from lobby ${lobbyId}`);
    }
  }, [readyState, lobbyId]);

  useEffect(() => {
    if (lastMessage) {
      console.log("Received message:", lastMessage.data);
    }
  }, [lastMessage]);

  const handleLeaveLobby = () => {
    sessionStorage.removeItem("player_id"); // Clean up on leave
    navigate("/");
  };

  if (isValidLobby === null) {
    <DefaultBox>
      return <Typography variant="h6">Loading...</Typography>;
    </DefaultBox>;
  }

  if (!isValidLobby) {
    return (
      <DefaultBox>
        <Typography variant="h4" color="error">
          {error}
        </Typography>
        <Button
          variant="contained"
          onClick={() => navigate("/")}
          sx={{ mt: 2 }}
        >
          Back to Lobby Manager
        </Button>
      </DefaultBox>
    );
  }

  return (
    <DefaultBox>
      <Typography variant="h4">Lobby: {lobbyId}</Typography>
      <Typography variant="body1">
        Player ID: {playerId || "Not joined"}
      </Typography>
      <Typography variant="body1">
        WebSocket Status:{" "}
        {readyState === WebSocket.OPEN ? "Connected" : "Disconnected"}
      </Typography>
      <Button
        variant="contained"
        color="secondary"
        onClick={handleLeaveLobby}
        sx={{ mt: 2 }}
      >
        Leave Lobby
      </Button>
    </DefaultBox>
  );
};

export default Lobby;
