import { Box, Button, TextField, Typography } from "@mui/material";
import { useNavigate } from "react-router-dom";
import { useState } from "react";
import { CreateLobbyResponse } from "../types/typings";
import { getErrorMessage } from "../utils/utils";

const createLobby = async () => {
  try {
    const url = import.meta.env.VITE_SERVER_HTTP_URL + "lobbies/create";
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
      `${import.meta.env.VITE_SERVER_HTTP_URL}lobbies/join/${lobbyId}`,
      {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          /* join data, e.g., user info */
        }),
      }
    );
    if (!response.ok) {
      if (response.status === 404) {
        throw new Error("Lobby does not exist");
      }
      throw new Error("Failed to join lobby");
    }
    return await response.json();
  } catch (error) {
    console.error("Error joining lobby:", error);
    throw error;
  }
};

const LobbyManager = () => {
  const [error, setError] = useState<string | null>(null);
  const [enterLobbyCode, setEnterLobbyCode] = useState<string>("");
  const navigate = useNavigate();

  const handleCreateLobby = async () => {
    try {
      setError(null);
      const data: CreateLobbyResponse = await createLobby();
      sessionStorage.setItem("player_id", data.player_id);
      navigate(`/lobby/${data.lobby_id}`);
    } catch (error) {
      setError("Failed to create lobby");
    }
  };

  const handleJoinLobby = async () => {
    try {
      setError(null);
      const data: CreateLobbyResponse = await joinLobby(enterLobbyCode);
      sessionStorage.setItem("player_id", data.player_id);
      navigate(`/lobby/${data.lobby_id}`);
    } catch (error) {
      setError(getErrorMessage(error));
    }
  };

  return (
    <Box
      sx={{ display: "flex", flexDirection: "column", alignItems: "center" }}
    >
      <Button variant="contained" onClick={handleCreateLobby}>
        Create Lobby
      </Button>
      {error && (
        <Typography variant="body1" color="Red">
          {error}
        </Typography>
      )}
      <TextField
        label="Enter lobby code"
        variant="filled"
        value={enterLobbyCode}
        onChange={(e) => setEnterLobbyCode(e.target.value)}
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
