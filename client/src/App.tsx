import { useState, useEffect } from "react";
import { ThemeProvider } from "@mui/material/styles";
import { BrowserRouter } from "react-router-dom";
import { UserProvider } from "./contexts/UserContext";
import { User } from "./types/typings";
import theme from "./styles/theme";
import AppRoutes from "./routes/AppRoutes";

function App() {
  const [user, setUser] = useState(() => {
    const storedUser = localStorage.getItem("user");
    return storedUser
      ? JSON.parse(storedUser)
      : { player_id: "", username: "", lobby_Id: "" };
  });

  useEffect(() => {
    if (!user.player_id) {
      fetch(`${import.meta.env.VITE_SERVER_HTTP_URL}/user/create`, {
        method: "POST",
      })
        .then((res) => res.json())
        .then((data) => {
          const newUser: User = {
            player_id: data.player_id,
            username: "",
            lobby_id: "",
          };
          localStorage.setItem("user", JSON.stringify(newUser));
          setUser(newUser);
        })
        .catch((e) => console.error("Failed to create user", e));
    }
  }, [user.player_id]);

  return (
    <UserProvider user={user} setUser={setUser}>
      <ThemeProvider theme={theme}>
        <BrowserRouter>
          <AppRoutes />
        </BrowserRouter>
      </ThemeProvider>
    </UserProvider>
  );
}

export default App;
