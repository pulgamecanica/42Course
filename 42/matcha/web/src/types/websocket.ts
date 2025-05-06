import { Notification } from './notification';
import { Message } from './message';

export type WSIncomingMessage =
  | { type: 'notification'; payload: Notification }
  | { type: 'message'; payload: Message }
  | { type: 'typing'; payload: { from: number; connection_id: number } }
  | { type: 'call:offer'; payload: { from_user_id: number; offer: RTCSessionDescriptionInit } }
  | { type: 'call:answer'; payload: { from_user_id: number; answer: RTCSessionDescriptionInit } }
  | { type: 'call:ice-candidate'; payload: { from_user_id: number; candidate: RTCIceCandidateInit } }
  | { type: 'call:end'; payload: { from_user_id: number } }
  | { type: 'call:decline'; payload: { from_user_id: number } }
  | { type: 'call:busy'; payload: { from_user_id: number } }
  | { type: 'call:unavailable'; payload: { from_user_id: number } }
  | { type: 'date'; payload: { from: number; connection_id: number } };

export type WSOutgoingMessage =
  | { type: 'typing'; payload: { to_user_id: number } }
  | { type: 'message'; payload: { to_user_id: number; content: string } }
  | { type: 'ping' }
  | { type: 'call:offer'; payload: { to_user_id: number; offer: RTCSessionDescriptionInit } }
  | { type: 'call:answer'; payload: { to_user_id: number; answer: RTCSessionDescriptionInit } }
  | { type: 'call:ice-candidate'; payload: { to_user_id: number; candidate: RTCIceCandidateInit } }
  | { type: 'call:end'; payload: { to_user_id: number } }
  | { type: 'call:decline'; payload: { to_user_id: number } }
  | { type: 'call:busy'; payload: { to_user_id: number } }
  | { type: 'call:unavailable'; payload: { to_user_id: number } }
  | { type: 'date' };

export type WSIncomingPayloadMap = {
  notification: Notification;
  message: Message;
  date: { from: number; connection_id: number };
  typing: { from: number; connection_id: number };
  "call:offer": { from_user_id: number; offer: RTCSessionDescriptionInit };
  "call:answer": { from_user_id: number; answer: RTCSessionDescriptionInit };
  "call:ice-candidate": { from_user_id: number; candidate: RTCIceCandidateInit };
  "call:end": { from_user_id: number };
  "call:decline": { from_user_id: number };
  "call:busy": { from_user_id: number };
  "call:unavailable": { from_user_id: number };
};
