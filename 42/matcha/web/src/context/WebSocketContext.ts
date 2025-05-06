import { WSOutgoingMessage, WSIncomingPayloadMap } from '@/types/websocket';
import { createContext } from 'react';

export type Handler<T> = (payload: T) => void;

export type WebSocketContextType = {
  sendMessage: (msg: WSOutgoingMessage) => void;
  registerHandler: <K extends keyof WSIncomingPayloadMap>(
    type: K,
    handler: Handler<WSIncomingPayloadMap[K]>
  ) => void;
  ready: boolean;
};

export const WebSocketContext = createContext<WebSocketContextType>({
  sendMessage: () => {},
  registerHandler: () => {},
  ready: false,
});