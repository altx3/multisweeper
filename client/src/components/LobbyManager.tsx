import { Box, Button, TextField, Typography } from "@mui/material";
import useWebSocket from "react-use-websocket";
import { useState, useEffect } from "react";
import { CreateLobbyResponse } from "../types/typings";

const createLobby = async () => {
  try {
    const url = import.meta.env.VITE_SERVER_HTTP_URL + "lobbies";
    const response = await fetch(url, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        /* lobby data, e.g., name */
      }),
    });
    if (!response.ok) throw new Error("Failed to create lobby");
    return await response.json(); // Returns { lobby_id: string, player_id: string }
  } catch (error) {
    console.error("Error creating lobby:", error);
    throw error;
  }
};

const joinLobby = async (lobbyId: string) => {
  try {
    const response = await fetch(
      `${import.meta.env.VITE_SERVER_HTTP_URL}lobbies/${lobbyId}/join`,
      {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          /* join data, e.g., user info */
        }),
      }
    );
    if (!response.ok) throw new Error("Failed to join lobby");
    return await response.json(); // Returns lobby details
  } catch (error) {
    console.error("Error joining lobby:", error);
    throw error;
  }
};

const LobbyManager = () => {
  const [response, setResponse] = useState<CreateLobbyResponse | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [lobbyCode, setLobbyCode] = useState<string>("");

  const handleCreateLobby = async () => {
    try {
      setError(null);
      const data: CreateLobbyResponse = await createLobby();
      setResponse(data);
    } catch (err) {
      setError("Failed to create lobby");
    }
  };

  const handleJoinLobby = async () => {
    try {
      setError(null);
      await joinLobby(lobbyCode);
    } catch (err) {
      setError("Failed to join lobby");
    }
  };

  return (
    <Box
      sx={{ display: "flex", flexDirection: "column", alignItems: "center" }}
    >
      <Button variant="contained" onClick={handleCreateLobby}>
        Create Lobby
      </Button>
      {response && (
        <Typography variant="body1" color="text.primary">
          Lobby ID: {response.lobby_id}
        </Typography>
      )}
      {error && (
        <Typography variant="body1" color="Red">
          {error}
        </Typography>
      )}
      <TextField
        label="Enter lobby code"
        variant="filled"
        value={lobbyCode}
        onChange={(e) => setLobbyCode(e.target.value)}
        sx={{
          "& .MuiInput-input": {
            color: "text.primary",
          },
          "& .MuiInput-underline:before": {
            borderBottomColor: "text.primary",
          },
          "& .MuiInput-underline:after": {
            borderBottomColor: "text.primary",
          },
          "& .MuiInputLabel-root": {
            color: "text.primary",
          },
        }}
      />
      <Button variant="contained" onClick={handleJoinLobby}>
        Join Lobby
      </Button>
    </Box>
  );
};

export default LobbyManager;
