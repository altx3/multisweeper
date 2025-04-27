import { Routes, Route } from "react-router-dom";
import Home from "../pages/Home.tsx";
import Lobby from "../pages/Lobby.tsx";
import Monitoring from "../pages/Monitoring.tsx";

function AppRoutes() {
  return (
    <Routes>
      <Route path="/" element={<Home />} />
      <Route path="/lobby/:lobbyId" element={<Lobby />} />
      <Route path="/lobby" element={<Lobby />} />
      <Route path="/monitoring" element={<Monitoring />} />
    </Routes>
  );
}

export default AppRoutes;
