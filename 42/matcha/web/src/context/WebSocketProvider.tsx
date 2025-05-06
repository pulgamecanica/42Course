// src/context/WebSocketContext.tsx
import {
  WSOutgoingMessage,
  WSIncomingMessage,
  WSIncomingPayloadMap,
} from '@/types/websocket';
import { useEffect, useRef, useState } from 'react';
import { useAuth } from '@hooks/useAuth';
import { useUserMe } from '@hooks/useUserMe';
import { Handler, WebSocketContext } from '@context/WebSocketContext';

const baseURL = import.meta.env.VITE_WS_API_BASE || 'ws://localhost:9292';

export function WebSocketProvider({ children }: { children: React.ReactNode }) {
  const { token, isAuthenticated } = useAuth();
  const { user } = useUserMe();
  const wsRef = useRef<WebSocket | null>(null);
  const [ready, setReady] = useState(false);


  const handlersRef = useRef<Partial<{
    [K in keyof WSIncomingPayloadMap]: Handler<WSIncomingPayloadMap[K]>[];
  }>>({});

  const registerHandler = <K extends keyof WSIncomingPayloadMap>(
    type: K,
    handler: Handler<WSIncomingPayloadMap[K]>
  ) => {
    if (!handlersRef.current[type]) handlersRef.current[type] = [];
    handlersRef.current[type]!.push(handler);
  };

  const sendMessage = (msg: WSOutgoingMessage) => {
    if (wsRef.current?.readyState === WebSocket.OPEN) {
      wsRef.current.send(JSON.stringify(msg));
    }
  };

  useEffect(() => {
    if (!token || !isAuthenticated || !user) return;

    const ws = new WebSocket(`${baseURL}/ws?token=${token}`);
    wsRef.current = ws;

    ws.onopen = () => {
      setReady(true);
      console.log('🔌 WebSocket connected');
    };

    ws.onclose = () => {
      setReady(false);
      console.log('❌ WebSocket disconnected');
    };

    ws.onmessage = (event) => {
      const parsed = JSON.parse(event.data) as WSIncomingMessage;
      const handlers = handlersRef.current;

      const runHandlers = <K extends keyof WSIncomingPayloadMap>(type: K) => {
        handlers[type]?.forEach((h) => h(parsed.payload as WSIncomingPayloadMap[K]));
      };

      switch (parsed.type) {
        case 'message':
        case 'notification':
        case 'typing':
        case 'call:offer':
        case 'call:answer':
        case 'call:ice-candidate':
        case 'call:end':
        case 'call:decline':
        case 'call:busy':
        case 'call:unavailable':
        case 'date':
          runHandlers(parsed.type as keyof WSIncomingPayloadMap);
          break;
        default:
          console.warn('Unknown WebSocket message type');
      }
    };

    return () => {
      ws.close();
    };
  }, [token, isAuthenticated, user]);

  return (
    <WebSocketContext.Provider value={{ sendMessage, registerHandler, ready }}>
      {children}
    </WebSocketContext.Provider>
  );
}
