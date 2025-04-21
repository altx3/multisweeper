import { Typography } from "@mui/material";
import DefaultBox from "../components/common/DefaultBox";
import LobbyManager from "../components/LobbyManager";

function Home() {
  return (
    <DefaultBox>
      <Typography variant="h2" color="primary">
        Multisweeper++ 💣
      </Typography>
      <LobbyManager />
    </DefaultBox>
  );
}

export default Home;
