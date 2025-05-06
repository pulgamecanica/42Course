import { useEffect, useRef, useState } from 'react';
import { useWebSocket } from '@/hooks/useWebSocket';
import { PhoneIcon, PhoneOffIcon } from 'lucide-react';
import { CallIncomingModal } from './CallIncomingModal';
import { useMessages } from '@/hooks/useMessages';
import { fetchIceServers } from '@/api/ice';
import { IceServer } from '@/types/iceServer';

type Props = {
  toUserId: number;
  username: string;
};

export function VoiceChat({ toUserId, username }: Props) {
  const { sendMessage } = useWebSocket();
  const {
    incomingCall,
    callStatus,
    setCallStatus,
    setIncomingCall,
    remoteAnswer,
    iceCandidates,
  } = useMessages();

  const pcRef = useRef<RTCPeerConnection | null>(null);
  const localStreamRef = useRef<MediaStream | null>(null);
  const analyserRef = useRef<AnalyserNode | null>(null);
  const [volume, setVolume] = useState(0);

  useEffect(() => {
    if (!remoteAnswer || !pcRef.current) return;
    pcRef.current.setRemoteDescription(new RTCSessionDescription(remoteAnswer));
    setCallStatus('connected');
  }, [remoteAnswer, setCallStatus]);

  useEffect(() => {
    if (!pcRef.current || !iceCandidates?.length) return;
    iceCandidates.forEach((candidate) => {
      pcRef.current?.addIceCandidate(new RTCIceCandidate(candidate));
    });
  }, [iceCandidates]);

  useEffect(() => {
    let animationFrameId: number = 0;
    const dataArray = new Uint8Array(32);

    const updateVolume = () => {
      if (analyserRef.current) {
        analyserRef.current.getByteFrequencyData(dataArray);
        const avg =
          dataArray.reduce((sum, val) => sum + val, 0) / dataArray.length;
        setVolume(avg);
      }
      animationFrameId = requestAnimationFrame(updateVolume);
    };

    if (callStatus === 'connected' || callStatus === 'calling') {
      updateVolume();
    } else {
      cancelAnimationFrame(animationFrameId);
      setVolume(0);
    }

    return () => cancelAnimationFrame(animationFrameId);
  }, [callStatus, analyserRef]);

  function normalizeIceServers(raw: IceServer[]): RTCIceServer[] {
    return raw.map(({ urls, username, credential }) => ({
      urls,
      username,
      credential,
    }));
  }
  
  const createPeerConnection = async (targetUserId: number) => {
    const iceServers = normalizeIceServers(await fetchIceServers());
    const pc = new RTCPeerConnection({ iceServers });

    pc.onicecandidate = (event) => {
      if (event.candidate) {
        sendMessage({
          type: 'call:ice-candidate',
          payload: {
            to_user_id: targetUserId,
            candidate: event.candidate.toJSON(),
          },
        });
      }
    };

    pc.ontrack = (event) => {
      const audio = new Audio();
      audio.srcObject = event.streams[0];
      audio.play().catch(() => {
        console.warn("User gesture required to play audio.");
      });
    };

    pcRef.current = pc;
    return pc;
  };

  const setupAudioAnalyser = (stream: MediaStream) => {
    const audioContext = new AudioContext();
    const source = audioContext.createMediaStreamSource(stream);
    const analyser = audioContext.createAnalyser();
    analyser.fftSize = 64;
    source.connect(analyser);
    analyserRef.current = analyser;
  };


  const startCall = async () => {
    const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
    localStreamRef.current = stream;
    setupAudioAnalyser(stream);
  
    const pc = await createPeerConnection(toUserId);
    stream.getTracks().forEach((track) => pc.addTrack(track, stream));
  
    const offer = await pc.createOffer();
    await pc.setLocalDescription(offer);
  
    // ⚠️ Wait until ICE gathering completes before sending offer
    await new Promise((resolve) => {
      if (pc.iceGatheringState === 'complete') return resolve(null);
      const checkState = () => {
        if (pc.iceGatheringState === 'complete') {
          pc.removeEventListener('icegatheringstatechange', checkState);
          resolve(null);
        }
      };
      pc.addEventListener('icegatheringstatechange', checkState);
    });
  
    sendMessage({ type: 'call:offer', payload: { to_user_id: toUserId, offer: pc.localDescription! } });
  
    setCallStatus('calling');
  
    setTimeout(() => {
      if (callStatus === 'calling') {
        setCallStatus('unavailable');
        endCall();
      }
    }, 15000);
  };

  const acceptCall = async () => {
    if (!incomingCall) return;

    const pc = await createPeerConnection(incomingCall.from_user_id);

    const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
    localStreamRef.current = stream;
    setupAudioAnalyser(stream);
    stream.getTracks().forEach((track) => pc.addTrack(track, stream));

    await pc.setRemoteDescription(new RTCSessionDescription(incomingCall.offer));
    const answer = await pc.createAnswer();
    await pc.setLocalDescription(answer);

    sendMessage({
      type: 'call:answer',
      payload: { to_user_id: incomingCall.from_user_id, answer },
    });

    setCallStatus('connected');
    setIncomingCall(null);
  };

  const declineCall = () => {
    if (incomingCall) {
      sendMessage({ type: 'call:decline', payload: { to_user_id: incomingCall.from_user_id } });
    }
    setCallStatus('declined');
    setIncomingCall(null);
    setTimeout(() => {
      setCallStatus('idle');
    }, 1000);
  };

  const endCall = () => {
    pcRef.current?.close();
    pcRef.current = null;
    localStreamRef.current?.getTracks().forEach((track) => track.stop());
    localStreamRef.current = null;

    if (callStatus === 'connected' || callStatus === 'calling') {
      sendMessage({ type: 'call:end', payload: { to_user_id: toUserId } });
    }

    setCallStatus('ended');
    setIncomingCall(null);
    analyserRef.current = null;
    setVolume(0);
    setTimeout(() => {
      setCallStatus('idle');
    }, 1000);
  };

  return (
    <div className="flex items-center gap-2">
      {callStatus !== 'idle' && (
        <span className="text-sm text-gray-400">
          {callStatus === 'calling' && 'Calling...'}
          {callStatus === 'incoming' && 'Incoming call...'}
          {callStatus === 'connected' && 'Connected ✅'}
          {callStatus === 'declined' && 'Declined ❌'}
          {callStatus === 'unavailable' && 'Waiting ...'}
          {callStatus === 'ended' && 'Call ended.'}
        </span>
      )}

      {(callStatus === 'connected' || callStatus === 'calling') && (
        <div className="w-2 h-6 bg-green-400 transition-all duration-100"
            style={{ transform: `scaleY(${Math.max(volume / 50, 0.1)})`, transformOrigin: 'bottom' }} />
      )}

      {callStatus === 'incoming' && incomingCall && (
        <CallIncomingModal
          callerUsername={username}
          onAccept={acceptCall}
          onDecline={declineCall}
        />
      )}

      {callStatus === 'connected' || callStatus === 'calling' ? (
        <button onClick={endCall} className="bg-red-500 px-4 py-2 rounded text-white">
          <PhoneOffIcon size={16} />
        </button>
      ) : (
        <button onClick={startCall} className="bg-green-500 px-4 py-2 rounded text-white">
          <PhoneIcon size={16} />
        </button>
      )}
    </div>
  );
}
