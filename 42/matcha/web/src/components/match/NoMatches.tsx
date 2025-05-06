import { useEffect, useState } from "react";
import { Typography, Container, Box } from "@mui/material";

const noMatchMessages = [
  "No matches. Maybe lower your standards?",
  "It's not you, it's everyone else.",
  "Tough crowd out there.",
  "Modern dating is broken. Try turning it off and on again.",
  "Not even a bot wants you right now.",
  "0 matches. 1 tear.",
  "Try waving at the void again later.",
];

export function NoMatches() {
  const [message, setMessage] = useState("");

  useEffect(() => {
    const msg = noMatchMessages[Math.floor(Math.random() * noMatchMessages.length)];
    setMessage(msg);
  }, []);

  return (
    <Container maxWidth="sm" sx={{ py: 6 }}>
      <Box className="flex flex-col items-center text-center">
        <img
          src="/frodo-keep-your-secrets.gif"
          alt="Tumbleweed"
          className="w-80 h-52 mb-4"
        />
        <Typography variant="h6" className="text-gray-700 dark:text-gray-300">
          {message}
        </Typography>
      </Box>
    </Container>
  );
}
