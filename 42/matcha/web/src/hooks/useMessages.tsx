import { MessagesContext } from "@context/MessagesContext";
import { useContext } from "react";

export function useMessages() {
  const context = useContext(MessagesContext);
  if (!context) {
    throw new Error('useMessages must be used within a MessagesProvider');
  }
  return context;
}