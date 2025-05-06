import {
  useEffect,
  useMemo,
  useState,
  ReactNode,
  useRef,
} from 'react';
import { useWebSocket } from '@hooks/useWebSocket';
import { fetchAllMessages } from '@api/messages';
import { Conversation } from '@/types/conversation';
import { Message } from '@/types/message';
import { PublicUser } from '@/types/user';
import { MessagesContext } from '@context/MessagesContext';
import toast from 'react-hot-toast';
import { fetchPublicProfile } from '@/api/publicProfile';
import { useUserMe } from '@/hooks/useUserMe';

type CallStatus =
  | 'idle'
  | 'incoming'
  | 'calling'
  | 'connected'
  | 'declined'
  | 'ended'
  | 'unavailable'
  | 'error';

export function MessagesProvider({ children }: { children: ReactNode }) {
  const { registerHandler, sendMessage } = useWebSocket();
  const [conversations, setConversations] = useState<Conversation[]>([]);
  const [typingUsers, setTypingUsers] = useState<Record<number, boolean>>({});
  const { user } = useUserMe();

  const [incomingCall, setIncomingCall] = useState<{ from_user_id: number; offer: RTCSessionDescriptionInit } | null>(null);
  const [callStatus, setCallStatus] = useState<CallStatus>('idle');
  const [remoteAnswer, setRemoteAnswer] = useState<RTCSessionDescriptionInit | null>(null);
  const iceCandidatesRef = useRef<RTCIceCandidateInit[]>([]);

  const enrichConversations = async (convos: Conversation[]) => {
    const enriched = await Promise.all(
      convos.map(async (c) => {
        try {
          const fullUser = await fetchPublicProfile(c.user.username, false);
          return { ...c, user: fullUser };
        } catch (e) {
          toast.error(`Failed to load full profile for ${c.user.username}: ${e}`);
          return c; // fallback to partial
        }
      })
    );
    setConversations(enriched);
  };

  useEffect(() => {
    if (!user) return;

    fetchAllMessages()
      .then(enrichConversations)
      .catch((e) => toast.error(`Failed to load messages: ${e}`));
  }, [user]);

  useEffect(() => {
    registerHandler('message', (msg: Message) => {
      appendMessageToConversationById(msg.sender_id, msg);
    });

    registerHandler('typing', ({ from }: { from: number }) => {
      setTypingUsers((prev) => ({ ...prev, [from]: true }));
      setTimeout(() => {
        setTypingUsers((prev) => {
          const updated = { ...prev };
          delete updated[from];
          return updated;
        });
      }, 1500);
    });

    registerHandler('call:offer', ({ from_user_id, offer }) => {
      if (callStatus === 'idle') {
        setIncomingCall({ from_user_id, offer });
        setCallStatus('incoming');
      } else {
        sendMessage({ type: 'call:busy', payload: { to_user_id: from_user_id } });
      }
    });

    registerHandler('call:answer', ({ answer }) => {
      setRemoteAnswer(answer);
      setCallStatus('connected');
    });

    registerHandler('call:ice-candidate', ({ candidate }) => {
      iceCandidatesRef.current.push(candidate);
    });

    registerHandler('call:end', () => {
      setCallStatus('ended');
      setIncomingCall(null);
    });

    registerHandler('call:decline', () => {
      setCallStatus('declined');
      setIncomingCall(null);
      setTimeout(() => {
        setCallStatus('idle');
      }, 1000);
    });

    registerHandler('call:busy', () => {
      setCallStatus('unavailable');
    });

    registerHandler('call:unavailable', () => {
      setCallStatus('unavailable');
      toast.error('User is unavailable');
      setTimeout(() => {
        setCallStatus('idle');
      }, 1000);
    });
  }, [registerHandler, callStatus, sendMessage]);

  const appendMessageToConversation = (username: string, msg: Message) => {
    setConversations((prev) => {
      const updated = [...prev];
      const convo = updated.find((c) => c.user.username === username);
      if (convo && !convo.messages.some((m) => m.id === msg.id)) {
        convo.messages = [...convo.messages, msg];
      }
      return updated;
    });
  };

  const appendMessageToConversationById = (id: number, msg: Message) => {
    setConversations((prev) => {
      const updated = [...prev];
      const convo = updated.find((c) => c.user.id === id);
      if (convo && !convo.messages.some((m) => m.id === msg.id)) {
        convo.messages = [...convo.messages, msg];
      }
      return updated;
    });
  };

  const isUserTyping = useMemo(() => {
    return (id: number) => Boolean(typingUsers[id]);
  }, [typingUsers]);

  const startConversationWith = (user: PublicUser) => {
    setConversations((prev) => {
      const exists = prev.some((c) => c.user.username === user.username);
      if (exists) return prev;

      const newConvo: Conversation = {
        user,
        messages: [],
      };

      return [newConvo, ...prev];
    });
  };

  const removeConversationWith = (user: PublicUser) => {
    setConversations(conversations.filter((c) => c.user.username !== user.username));
  };

  const refetchAllMessages = async () => {
    try {
      const conversations = await fetchAllMessages();
      await enrichConversations(conversations);
    } catch (e) {
      toast.error(`Failed to reload conversations: ${e}`);
    }
  };

  return (
    <MessagesContext.Provider
      value={{
        conversations,
        isUserTyping,
        appendMessageToConversation,
        startConversationWith,
        removeConversationWith,
        refetchAllMessages,
        incomingCall,
        callStatus,
        setCallStatus,
        setIncomingCall,
        remoteAnswer,
        iceCandidates: iceCandidatesRef.current,
      }}
    >
      {children}
    </MessagesContext.Provider>
  );
}
