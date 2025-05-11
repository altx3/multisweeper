import React, { useRef, useEffect } from "react";

import { Box } from "@mui/material";

import Cursor from "./Cursor";

interface GameBoardProps {
  sendMessage: (message: string) => void;
  cursors: Map<string, { x: number; y: number }>;
  playerId: string;
}
const GameBoard: React.FC<GameBoardProps> = ({
  sendMessage,
  cursors,
  playerId,
}) => {
  const fps = 60;
  const ms = 1000 / fps;
  const gameBoardRef = useRef<HTMLDivElement>(null);
  let timeout: number | null = null;
  let lastSent = 0;

  useEffect(() => {
    const handleMouseMove = (event: MouseEvent) => {
      if (gameBoardRef.current) {
        const now = Date.now();
        if (now - lastSent >= ms) {
          lastSent = now;
          const rect = gameBoardRef.current.getBoundingClientRect();
          const x = (event.clientX - rect.left) / rect.width;
          const y = (event.clientY - rect.top) / rect.height;
          sendMessage(
            JSON.stringify({
              type: "mouse_move",
              x: String(x),
              y: String(y),
            })
          );
        }
      }
    };

    const gameBoard = gameBoardRef.current;
    if (gameBoard) {
      gameBoard.addEventListener("mousemove", handleMouseMove);
    }

    return () => {
      if (gameBoard) {
        gameBoard.removeEventListener("mousemove", handleMouseMove);
      }
      if (timeout) clearTimeout(timeout);
    };
  }, [sendMessage]);

  return (
    <Box
      ref={gameBoardRef}
      id="game-board"
      sx={{
        position: "absolute",
        top: 0,
        left: 0,
        width: "100%",
        height: "100%",
      }}
    >
      {Array.from(cursors.entries()).map(([id, pos]) => {
        if (gameBoardRef.current) {
          const rect = gameBoardRef.current.getBoundingClientRect();
          console.log(pos.x * rect.width);
          console.log(pos.y * rect.height);
          return id !== playerId ? (
            <Cursor
              key={id}
              left={
                pos.x *
                (gameBoardRef.current?.getBoundingClientRect().width || 0)
              }
              top={
                pos.y *
                (gameBoardRef.current?.getBoundingClientRect().height || 0)
              }
            />
          ) : null;
        }
      })}
    </Box>
  );
};

export default GameBoard;
