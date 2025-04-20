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
  const [lobbyId, setLobbyId] = useState<string | null>(null);
  const [inputLobbyId, setInputLobbyId] = useState('');
  const [error, setError] = useState<string | null>(null);

  // WebSocket setup
  const { sendMessage, lastMessage } = useWebSocket(
    lobbyId ? `ws://localhost:9001/lobby/${lobbyId}/chat` : null
  );
  const [messages, setMessages] = useState<string[]>([]);

  useEffect(() => {
    if (lastMessage) {
      setMessages((prev) => [...prev, lastMessage.data]);
    }
  }, [lastMessage]);

  // Create lobby
  const handleCreateLobby = async () => {
    try {
      setError(null);
      const data = await createLobby();
      setLobbyId(data.id);
    } catch (err) {
      setError('Failed to create lobby');
    }
  };

  // Join lobby
  const handleJoinLobby = async () => {
    try {
      setError(null);
      await joinLobby(inputLobbyId);
      setLobbyId(inputLobbyId);
    } catch (err) {
      setError('Failed to join lobby');
    }
  };

  // Send chat message
  const handleSendMessage = (message: string) => {
    sendMessage(message);
  };

  return (
    <div>
      {!lobbyId ? (
        <div>
          <h2>Lobby System</h2>
          <button onClick={handleCreateLobby}>Create Lobby</button>
          <div>
            <input
              type="text"
              value={inputLobbyId}
              onChange={(e) => setInputLobbyId(e.target.value)}
              placeholder="Enter Lobby ID"
            />
            <button onClick={handleJoinLobby}>Join Lobby</button>
          </div>
          {error && <p style={{ color: 'red' }}>{error}</p>}
        </div>
      ) : (
        <div>
          <h2>Lobby: {lobbyId}</h2>
          <h3>Chat</h3>
          <ul>
            {messages.map((msg, index) => (
              <li key={index}>{msg}</li>
            ))}
          </ul>
          <button onClick={() => handleSendMessage('Hello!')}>
            Send Hello
          </button>
          <button onClick={() => setLobbyId(null)}>Leave Lobby</button>
        </div>
      )}
    </div>
  );
};

export default LobbyManager;
