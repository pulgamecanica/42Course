import { Conversation } from '@/types/conversation';
import { Message } from '@/types/message';
import { User } from '@/types/user';
import { MessageInput } from '@components/MessageInput';
import { TypingIndicator } from '@/components/chat/TypingIndicator';
import { useEffect, useRef, useState } from 'react';
import { useNavigate } from 'react-router-dom';

type Props = {
  conversation: Conversation;
  currentUser: User;
  isTyping: boolean;
  onSendLocalMessage: (content: string) => void;
};

export function ChatWindow({ conversation, currentUser, isTyping, onSendLocalMessage }: Props) {
  const containerRef = useRef<HTMLDivElement>(null);
  const [showScrollButton, setShowScrollButton] = useState(false);
  const navigate = useNavigate();
  const profilePic = (conversation?.user?.pictures || []).find((p) => p.is_profile === "t")?.url;

  const scrollToBottom = () => {
    containerRef.current?.scrollTo({
      top: containerRef.current.scrollHeight,
      behavior: 'smooth',
    });
  };

  const handleScroll = () => {
    if (!containerRef.current) return;
    const { scrollTop, scrollHeight, clientHeight } = containerRef.current;
    const nearBottom = scrollHeight - scrollTop - clientHeight < 100;
    setShowScrollButton(!nearBottom);
  };

  useEffect(() => {
    const el = containerRef.current;
    if (!el) return;

    const { scrollTop, scrollHeight, clientHeight } = el;
    const nearBottom = scrollHeight - scrollTop - clientHeight < 100;

    if (nearBottom) scrollToBottom();
  }, [conversation.messages]);

  useEffect(() => {
    scrollToBottom();
  }, [conversation.user]);


  return (
    <div className="flex flex-col h-full border-e dark:border-gray-500 relative">
      <div
        onClick={() => navigate(`/profile/${conversation.user.username}`)}
        className="cursor-pointer px-4 py-4 border-b dark:border-gray-500 flex items-center justify-center gap-3 bg-gray-100 dark:bg-gray-800 hover:bg-gray-300 dark:hover:bg-gray-600 transition"
      >
        <img
          src={
            profilePic || '/default-avatar.png'
          }
          alt="avatar"
          className="w-10 h-10 rounded-full object-cover"
        />
        <div className="flex flex-col">
          <span className="font-medium text-sm text-gray-900 dark:text-white">
            {conversation.user.username}
          </span>
          <span className={`text-xs ${conversation.user.online_status ? 'text-green-500' : 'text-gray-400'}`}>
            {conversation.user.online_status ? 'Online' : `Last seen: ${conversation.user.last_seen_at ? new Date(conversation.user.last_seen_at).toLocaleString() : 'N/A'}`}
          </span>
        </div>
      </div>

      <div
        ref={containerRef}
        onScroll={handleScroll}
        className="flex-1 p-4 overflow-y-auto space-y-2"
      >
        {conversation.messages.map((msg: Message) => {
          const isMine = msg.sender_id === currentUser.id;
          return (
            <div
              key={msg.id}
              className={`flex ${isMine ? 'justify-end' : 'justify-start'}`}
              title={new Date(msg.created_at).toLocaleString()} // Tooltip on hover
            >
              <div
                className={`relative group max-w-xs px-3 py-2 rounded-md text-sm break-words ${
                  isMine
                    ? 'bg-blue-500 text-white'
                    : 'bg-gray-300 text-black dark:bg-gray-700 dark:text-white'
                }`}
              >
                {msg.content}
                <span className="absolute -bottom-4 right-1 text-[10px] text-gray-400 opacity-0 group-hover:opacity-100 transition-opacity duration-200 z-10">
                  {new Date(msg.created_at).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })}
                </span>
              </div>
            </div>
          );
        })}
        {isTyping && <TypingIndicator />}
      </div>

      {showScrollButton && (
        <button
          onClick={scrollToBottom}
          className="absolute bottom-20 left-1/2 -translate-x-1/2 bg-blue-500 text-white px-3 py-1.5 text-xs rounded shadow hover:bg-blue-600 transition"        >
          ↓ Go to bottom ↓
        </button>
      )}

      <MessageInput
        sender={conversation.user}
        onSendLocalMessage={onSendLocalMessage}
      />
    </div>
  );
}
