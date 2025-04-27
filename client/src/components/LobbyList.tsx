import { useState, useEffect } from "react";
import {
  List,
  ListItem,
  ListItemText,
  Typography,
  CircularProgress,
  Alert,
  Box,
  Button,
} from "@mui/material";
import { GetLobbiesResponse, Lobby } from "../types/typings";

const API_URL = `${import.meta.env.VITE_SERVER_HTTP_URL}/lobbies`;
const POLL_INTERVAL = 1000;

const LobbyList = () => {
  const [lobbies, setLobbies] = useState<Lobby[]>([]);
  const [loading, setLoading] = useState<boolean>(true);
  const [error, setError] = useState<string | null>(null);

  const fetchLobbies = async () => {
    try {
      setLoading(true);
      const response = await fetch(API_URL, {
        method: "GET",
        headers: { "Content-Type": "application/json" },
      });

      if (!response.ok) {
        throw new Error(`Error: ${response.status} - ${response.statusText}`);
      }

      const data: GetLobbiesResponse = await response.json();
      setLobbies(data.lobbies);
      setError(null);
    } catch (err) {
      setError(err instanceof Error ? err.message : "Failed to fetch lobbies");
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchLobbies(); // Initial fetch

    const interval = setInterval(fetchLobbies, POLL_INTERVAL);

    return () => clearInterval(interval); // Cleanup on unmount
  }, []);

  return (
    <Box>
      <Typography variant="h4" gutterBottom>
        Available Lobbies
      </Typography>
      <Button
        variant="contained"
        onClick={fetchLobbies}
        disabled={loading}
        sx={{ mb: 2 }}
        aria-label="Refresh lobbies manually"
      >
        Refresh
      </Button>
      {loading && !lobbies.length && (
        <Box sx={{ display: "flex", justifyContent: "center", my: 2 }}>
          <CircularProgress aria-label="Loading lobbies" />
        </Box>
      )}
      {error && (
        <Alert severity="error" sx={{ my: 2 }}>
          {error}
        </Alert>
      )}
      {!loading && !error && lobbies.length === 0 && (
        <Typography>No lobbies available.</Typography>
      )}
      <List>
        {lobbies.map((lobby) => (
          <ListItem
            key={lobby.lobby_id}
            divider
            alignItems="center"
            sx={{ bgcolor: "background.paper", borderRadius: 1, mb: 1 }}
          >
            <Box display={"flex"} flexDirection={"column"}>
              <ListItemText primary={`Lobby ID: ${lobby.lobby_id}`} />
              <ListItemText secondary={`Host: ${lobby.host_id}`} />
              <ListItemText
                secondary={`Players: ${lobby.players.join(", ")}`}
              />
            </Box>
          </ListItem>
        ))}
      </List>
    </Box>
  );
};

export default LobbyList;
