import { useNavigate } from "react-router-dom";
import { Button, Typography } from "@mui/material";
import DefaultBox from "../components/common/DefaultBox";
import LobbyManager from "../components/LobbyManager";

function Home() {
  const navigate = useNavigate();

  const handleNavigation = (path: string) => {
    navigate(path);
  };

  return (
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
  );
}

export default Home;
