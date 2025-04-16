import React, { useState, useRef } from 'react';
import { Button, Box, Typography, Paper } from '@mui/material';

const SimpleWebSocketClient: React.FC = () => {
  const [connected, setConnected] = useState(false);
  const [messages, setMessages] = useState<string[]>([]);
  const wsRef = useRef<WebSocket | null>(null);

  // Connect to WebSocket server
  const connect = () => {
    // Create new WebSocket connection
    const ws = new WebSocket('ws://localhost:9001');

    // Set up event handlers
    ws.onopen = () => {
      console.log('Connected to server');
      setConnected(true);
      setMessages(prev => [...prev, "Connected to server"]);
    };

    ws.onmessage = (event) => {
      console.log('Received:', event.data);
      setMessages(prev => [...prev, `Received: ${event.data}`]);
    };

    ws.onclose = () => {
      console.log('Disconnected from server');
      setConnected(false);
      setMessages(prev => [...prev, "Disconnected from server"]);
    };

    // Store the WebSocket instance
    wsRef.current = ws;
  };

  // Disconnect from server
  const disconnect = () => {
    if (wsRef.current) {
      wsRef.current.close();
      wsRef.current = null;
    }
  };

  // Send ping message
  const sendPing = () => {
    if (wsRef.current && connected) {
      wsRef.current.send('ping');
      setMessages(prev => [...prev, "Sent: ping"]);
    }
  };

  return (
    <Paper sx={{ p: 2, m: 2, maxWidth: 400 }}>
      <Typography variant="h5" mb={2}>WebSocket Test</Typography>

      <Box mb={2}>
        <Button
          variant="contained"
          color={connected ? "error" : "primary"}
          onClick={connected ? disconnect : connect}
          sx={{ mr: 1 }}
        >
          {connected ? "Disconnect" : "Connect"}
        </Button>

        {connected && (
          <Button variant="outlined" onClick={sendPing}>
            Send Ping
          </Button>
        )}
      </Box>

      <Typography variant="subtitle1">Messages:</Typography>
      <Paper variant="outlined" sx={{ p: 1, maxHeight: 200, overflow: 'auto' }}>
        {messages.map((msg, i) => (
          <Typography key={i} variant="body2">{msg}</Typography>
        ))}
      </Paper>
    </Paper>
  );
};

export default SimpleWebSocketClient;
