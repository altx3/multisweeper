import { Box } from "@mui/material";

import CursorImage from "../assets/cursor.svg";

interface Props {
  left: number;
  top: number;
}

const Cursor = ({ left, top }: Props) => {
  return (
    <Box
      component="img"
      src={CursorImage}
      sx={{
        position: "absolute",
        height: "15px",
        zIndex: 10,
        left: `${left}px`,
        top: `${top}px`,
      }}
    />
  );
};
export default Cursor;
