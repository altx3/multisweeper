import { createTheme } from "@mui/material/styles";

const theme = createTheme({
  palette: {
    primary: { main: "#c600f3" }, // Purple
    secondary: { main: "#fe87ff" }, // Violet
    background: { default: "#383838" }, // Dark Gray
    text: {
      primary: "#FFFFFF", // White
    },
  },
  typography: {
    fontFamily: '"Comic Neue", "Roboto", sans-serif',
  },
});

export default theme;
