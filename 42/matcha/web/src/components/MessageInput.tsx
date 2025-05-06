import { useState, KeyboardEvent } from 'react';
import { useWebSocket } from '@/hooks/useWebSocket';
import { VoiceChat } from './chat/VoiceChat';
import { PublicUser } from '@/types/user';

type MessageInputProps = {
  sender: PublicUser;
  onSendLocalMessage: (content: string) => void;
};

export function MessageInput({ sender, onSendLocalMessage }: MessageInputProps) {
  const { sendMessage } = useWebSocket();
  const [text, setText] = useState('');

  const handleSend = () => {
    if (!text.trim()) return;

    sendMessage({
      type: 'message',
      payload: {
        to_user_id: sender.id,
        content: text,
      },
    });

    onSendLocalMessage(text);
    setText('');
  };

  const handleKeyDown = (e: KeyboardEvent<HTMLInputElement>) => {
    sendMessage({
      type: 'typing',
      payload: {
        to_user_id: sender.id,
      },
    });

    if (e.key === 'Enter' && !e.shiftKey) {
      e.preventDefault();
      handleSend();
    }
  };

  return (
    <div className="border-t px-4 py-3 flex items-center gap-2 dark:border-gray-700">
      <input
        type="text"
        placeholder="Type a message..."
        value={text}
        onChange={(e) => setText(e.target.value)}
        onKeyDown={handleKeyDown}
        className="flex-1 px-3 py-2 rounded bg-gray-100 dark:bg-gray-800 dark:text-white"
      />
      <button
        onClick={handleSend}
        className="px-4 py-2 text-sm font-semibold bg-blue-500 text-white rounded hover:bg-blue-600"
      >
        Send
      </button>
      <div><VoiceChat toUserId={sender.id} username={sender.username} /></div>
    </div>
  );
}
