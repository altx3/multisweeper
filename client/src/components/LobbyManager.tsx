import { useState, useEffect } from 'react';
import useWebSocket from 'react-use-websocket';

const createLobby = async () => {
  try {
    const url = import.meta.env.VITE_SERVER_HTTP_URL + 'lobbies';
    const response = await fetch(url, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        /* lobby data, e.g., name */
      }),
    });
    if (!response.ok) throw new Error('Failed to create lobby');
    return await response.json(); // Returns { id: string, name: string }
  } catch (error) {
    console.error('Error creating lobby:', error);
    throw error;
  }
};

const joinLobby = async (lobbyId: string) => {
  try {
    const response = await fetch(
      `${import.meta.env.VITE_SERVER_HTTP_URL}lobbies/${lobbyId}/join`,
      {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          /* join data, e.g., user info */
        }),
      }
    );
    if (!response.ok) throw new Error('Failed to join lobby');
    return await response.json(); // Returns lobby details
  } catch (error) {
    console.error('Error joining lobby:', error);
    throw error;
  }
};

const LobbyManager = () => {
  const [lobby
};
