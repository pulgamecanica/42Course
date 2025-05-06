// src/components/chat/GlobalCallHandler.tsx
import { useMessages } from '@/hooks/useMessages';
import { useWebSocket } from '@/hooks/useWebSocket';
import { useMemo } from 'react';
import { CallIncomingModal } from './chat/CallIncomingModal';
import { useNavigate } from 'react-router-dom';

export function GlobalCallHandler() {
  const { incomingCall, callStatus, setCallStatus, setIncomingCall, conversations } = useMessages();
  const { sendMessage } = useWebSocket();
  const navigate = useNavigate();

  const callerData = useMemo(() => {
    const user = conversations.find(c => c.user.id === incomingCall?.from_user_id)?.user;
  
    return {
      username: user?.username || 'Unknown',
      avatarUrl: user?.pictures.find(p => p.is_profile === 't')?.url ?? undefined,
    };
  }, [incomingCall, conversations]);

  const handleDecline = () => {
    if (incomingCall) {
      sendMessage({ type: 'call:decline', payload: { to_user_id: incomingCall.from_user_id } });
    }
    setCallStatus('declined');
    setIncomingCall(null);
  };

  const handleAccept = () => {
    setCallStatus('connected');
    navigate(`/conversations?user=${callerData.username}`)
  };

  if (!incomingCall || callStatus !== 'incoming') return null;

  return (
    <CallIncomingModal
      callerUsername={callerData.username}
      callerAvatarUrl={callerData.avatarUrl}
      onAccept={handleAccept}
      onDecline={handleDecline}
    />
  );
}
