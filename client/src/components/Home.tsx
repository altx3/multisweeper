import { Box, Typography } from '@mui/material';
import SimpleWebSocketClient from './SimpleWebSocketClient';

function Home() {
  return (
    <Box
      sx={{
        height: '100vh',
        width: '100vw',
        display: 'flex',
        justifyContent: 'center',
        alignItems: 'center',
        textAlign: 'center',
      }}
    >
      <Box>
        <Typography variant="h4" gutterBottom>
          Multisweeper++
        </Typography>
        <SimpleWebSocketClient />
      </Box>
    </Box>
  );
}

export default Home;
