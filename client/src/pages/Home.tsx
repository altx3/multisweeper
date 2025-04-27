import { useNavigate } from "react-router-dom";
import { Box, Button, Typography } from "@mui/material";
import DefaultBox from "../components/common/DefaultBox";
import LobbyManager from "../components/LobbyManager";
import mascot from "../assets/multisweeper_mascot.png";

function Home() {
  const navigate = useNavigate();

  const handleNavigation = (path: string) => {
    navigate(path);
  };

  return (
    <Box>
      <DefaultBox sx={{ gap: "1rem" }}>
        <Typography variant="h2" color="primary">
          Multisweeper++ 💣
        </Typography>
        <LobbyManager />
        <Button
          variant="contained"
          onClick={() => handleNavigation("/monitoring")}
        >
          monitoring
        </Button>
      </DefaultBox>
      <Box
        component="img"
        src={mascot}
        alt="decorative image"
        sx={{
          position: "fixed",
          bottom: 0,
          right: 0,
          width: 300,
          height: "auto",
          zIndex: 1000,
        }}
      />
    </Box>
  );
}

export default Home;
