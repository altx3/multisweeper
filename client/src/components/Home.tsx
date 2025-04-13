import { Box, Typography } from '@mui/material';

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
      <Typography variant="h4" gutterBottom>
        Multisweeper++
      </Typography>
    </Box>
  );
}

export default Home;
