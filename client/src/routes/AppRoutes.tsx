import { Routes, Route } from "react-router-dom";
import Home from "../pages/Home.tsx";
import Lobby from "../pages/Lobby.tsx";

function AppRoutes() {
  return (
    <Routes>
      <Route path="/" element={<Home />} />
      <Route path="/lobby/:lobbyId" element={<Lobby />} />
      <Route path="/lobby" element={<Lobby />} />
    </Routes>
  );
}

export default AppRoutes;
