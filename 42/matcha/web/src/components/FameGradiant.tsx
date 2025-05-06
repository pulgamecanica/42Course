import {
    Box,
    Tooltip,
  } from "@mui/material";

export const renderGradientBar = (label: string, score: number) => (
    <Tooltip title={`${label}: ${score.toFixed(2)}`} arrow>
      <Box
        sx={{
          mt: 0.5,
          height: 8,
          borderRadius: 4,
          background: "linear-gradient(to right, #fdba74, #93c5fd, #86eeac)",
          position: "relative",
          overflow: "hidden",
        }}
      >
        <Box
          sx={{
            width: `${100 - score}%`,
            height: "100%",
            background: "gray",
            position: "absolute",
            top: 0,
            right: 0,
          }}
        />
      </Box>
    </Tooltip>
  );