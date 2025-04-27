import { useState } from "react";
import { useNavigate } from "react-router-dom";
import { Box, Button, TextField, Typography } from "@mui/material";

import { CreateLobbyResponse } from "../types/typings";
import { getErrorMessage } from "../utils/utils";
import { useUser } from "../contexts/UserContext";

const LobbyManager = () => {
  const { user, setUser } = useUser();
  const [error, setError] = useState<string | null>(null);
  const [enterLobbyCode, setEnterLobbyCode] = useState<string>("");
  const navigate = useNavigate();

  const createLobby = async () => {
    try {
      console.log(user.player_id);
      const url = `${import.meta.env.VITE_SERVER_HTTP_URL}/lobbies/create`;
      const response = await fetch(url, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          host_id: user.player_id,
        }),
      });
      if (!response.ok) throw new Error("Failed to create lobby");
      return await response.json();
    } catch (error) {
      console.error("Error creating lobby:", error);
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
            player_id: user.player_id,
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

  const handleCreateLobby = async () => {
    try {
      setError(null);
      const data: CreateLobbyResponse = await createLobby();
      console.log(data);
      const updatedUser = { ...user, lobby_id: data.lobby_id };
      setUser(updatedUser);
      localStorage.setItem("user", JSON.stringify(updatedUser));
      navigate(`/lobby/${data.lobby_id}`);
    } catch (error) {
      setError(getErrorMessage(error));
    }
  };

  const handleJoinLobby = async () => {
    try {
      setError(null);
      const data: CreateLobbyResponse = await joinLobby(enterLobbyCode);
      const updatedUser = { ...user, lobby_id: data.lobby_id };
      setUser(updatedUser);
      localStorage.setItem("user", JSON.stringify(updatedUser));
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
