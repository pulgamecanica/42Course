import { createContext } from 'react';
import { Conversation } from '@/types/conversation';
import { Message } from '@/types/message';
import { PublicUser } from '@/types/user';

type CallStatus =
  | 'idle'
  | 'incoming'
  | 'calling'
  | 'connected'
  | 'declined'
  | 'ended'
  | 'unavailable'
  | 'error';

type MessagesContextType = {
  conversations: Conversation[];
  isUserTyping: (id: number) => boolean;
  appendMessageToConversation: (username: string, msg: Message) => void;
  startConversationWith: (user: PublicUser) => void;
  removeConversationWith: (user: PublicUser) => void;
  refetchAllMessages: () => Promise<void>;
  incomingCall: { from_user_id: number; offer: RTCSessionDescriptionInit } | null;
  callStatus: CallStatus;
  setCallStatus: (status: CallStatus) => void;
  setIncomingCall: (call: { from_user_id: number; offer: RTCSessionDescriptionInit } | null) => void;
  remoteAnswer: RTCSessionDescriptionInit | null;
  iceCandidates: RTCIceCandidateInit[];
};

export const MessagesContext = createContext<MessagesContextType | undefined>(undefined);
