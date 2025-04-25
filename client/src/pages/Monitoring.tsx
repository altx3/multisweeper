import { Typography } from "@mui/material";
import DefaultBox from "../components/common/DefaultBox";
import LobbyList from "../components/LobbyList";

function Monitoring() {
  return (
    <DefaultBox>
      <Typography variant="h2" color="primary">
        I'm watching (◕‿◕)
      </Typography>
      <LobbyList />
    </DefaultBox>
  );
}

export default Monitoring;
