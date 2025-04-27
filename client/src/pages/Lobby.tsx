import { useParams, useNavigate } from "react-router-dom";
import { useEffect, useRef, useState } from "react";
import useWebSocket from "react-use-websocket";

import { Button, Typography } from "@mui/material";
import DefaultBox from "../components/common/DefaultBox";

import { CreateLobbyResponse } from "../types/typings";
import { useUser } from "../contexts/UserContext";

const Lobby = () => {
  const navigate = useNavigate();
  const hasJoinedRef = useRef(false);
  const isMountedRef = useRef(true);
  const isLeavingRef = useRef(false);
  const { user, setUser } = useUser();
  const { lobbyId } = useParams<{ lobbyId: string }>();
  const [isValidLobby, setIsValidLobby] = useState<boolean | null>(null);
  const [error, setError] = useState<string | null>(null);

  const checkLobbyExists = async (lobbyId: string) => {
    try {
      const response = await fetch(
        `${import.meta.env.VITE_SERVER_HTTP_URL}/lobbies/${lobbyId}`,
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

  const joinLobby = async (lobbyId: string) => {
    try {
      const response = await fetch(
        `${import.meta.env.VITE_SERVER_HTTP_URL}/lobbies/${lobbyId}/join`,
        {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({
            lobby_id: lobbyId,
            player_id: user.player_id,
          }),
        }
      );
      if (!response.ok) {
        throw new Error("Failed to join lobby: " + response.status);
      }
      const data: CreateLobbyResponse = await response.json();
      const updatedUser = { ...user, lobby_id: data.lobby_id };
      if (isMountedRef.current) {
        setUser(updatedUser);
        localStorage.setItem("user", JSON.stringify(updatedUser));
      }
    } catch (error) {
      console.error("Error joining lobby:", error);
      throw error;
    }
  };

  // WebSocket connection
  const { readyState, lastMessage, sendMessage } = useWebSocket(
    isValidLobby && lobbyId
      ? `${import.meta.env.VITE_SERVER_WS_URL}/lobbies/${lobbyId}`
      : null,
    {
      shouldReconnect: () => isMountedRef.current,
      reconnectAttempts: 10,
      reconnectInterval: 3000,
      onOpen: () => {
        console.log(
          `WebSocket opened for ${user.player_id} in lobby ${lobbyId}`
        );
        if (user.player_id) {
          sendMessage(
            JSON.stringify({
              type: "init",
              lobby_id: lobbyId,
              player_id: user.player_id,
            })
          );
        }
      },
      onClose: () => {
        console.log(
          `WebSocket closed for ${user.player_id} in lobby ${lobbyId}`
        );
        const updatedUser = { ...user, lobby_id: "" };
        setUser(updatedUser);
        localStorage.setItem("user", JSON.stringify(updatedUser));
      },
      onError: (event) => {
        console.error(`😿 WebSocket error for lobby ${lobbyId}:`, event);
      },
    }
  );

  // Cleanup on unmount
  useEffect(() => {
    isMountedRef.current = true;
    return () => {
      isMountedRef.current = false;
    };
  }, []);

  // Reset hasJoinedRef when lobbyId changes
  useEffect(() => {
    hasJoinedRef.current = false;
  }, [lobbyId]);

  useEffect(() => {
    // Check if we're leaving right at the start and skip if join if we are
    if (isLeavingRef.current) {
      return;
    }

    if (!lobbyId) {
      setError("No lobby ID provided");
      setIsValidLobby(false);
      return;
    }

    if (hasJoinedRef.current || user.lobby_id === lobbyId) {
      setIsValidLobby(true);
      setError(null);
      return;
    }

    checkLobbyExists(lobbyId)
      .then(() => {
        setIsValidLobby(true);
        setError(null);

        hasJoinedRef.current = true;
        joinLobby(lobbyId)
          .then(() => {
            sendMessage(
              JSON.stringify({
                type: "initDirect",
                lobby_id: lobbyId,
                player_id: user.player_id,
              })
            );
          })
          .catch((err) => {
            hasJoinedRef.current = false;
            throw err;
          });
      })
      .catch((err) => {
        setError(err.message || "Failed to join lobby");
        setIsValidLobby(false);
      });
  }, [lobbyId, user.lobby_id]);

  // Log received messages for debugging
  useEffect(() => {
    if (lastMessage) {
      console.log("📬 Received message:", lastMessage.data);
    }
  }, [lastMessage]);

  // Handle leave lobby
  const handleLeaveLobby = () => {
    isLeavingRef.current = true;
    const updatedUser = { ...user, lobby_id: "" };
    if (isMountedRef.current) {
      setUser(updatedUser);
      localStorage.setItem("user", JSON.stringify(updatedUser));
    }
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
        <Button variant="contained" onClick={handleLeaveLobby} sx={{ mt: 2 }}>
          Home
        </Button>
      </DefaultBox>
    );
  }

  return (
    <DefaultBox>
      <Typography variant="h4">Lobby: {lobbyId}</Typography>
      <Typography variant="body1">
        Player ID: {user.player_id || "Not joined"}
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
