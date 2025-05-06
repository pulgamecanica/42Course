import { WebSocketContext } from "@/context/WebSocketContext";
import { useContext } from "react";

export const useWebSocket = () => useContext(WebSocketContext);
