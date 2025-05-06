import { useMessages } from '@/hooks/useMessages';
import { useUserMe } from '@/hooks/useUserMe';
import { useEffect, useState } from 'react';
import { ChatWindow } from '@/components/chat/ChatWindow';
import { ConversationPanel } from '@/components/chat/ConversationPanel';
import { Conversation } from '@/types/conversation';
import { useSearchParams } from 'react-router-dom';

export function ConversationsPage() {
  const { user } = useUserMe();
  const { conversations, isUserTyping, appendMessageToConversation } = useMessages();
  const [selectedUsername, setSelectedUsername] = useState<string | null>(null);
  const [activeConversation, setActiveConversation] = useState<Conversation | null>(null);
  const [searchParams] = useSearchParams();
  const initialUsername = searchParams.get('user');

  useEffect(() => {
    if (initialUsername) {
      setSelectedUsername(initialUsername);
    }
  }, [initialUsername]);
  
  useEffect(() => {
    setActiveConversation(conversations.find((c) => c.user.username === selectedUsername) || null);
  }, [conversations, selectedUsername]);

  const handleSendLocalMessage = (content: string) => {
    if (!activeConversation || !user) return;

    appendMessageToConversation(activeConversation.user.username, {
      id: Date.now(),
      connection_id: activeConversation.messages[activeConversation.messages.length -1]?.connection_id ?? -1,
      sender_id: user.id,
      sender_username: user.username,
      content,
      created_at: new Date().toISOString(),
    });
  };

  if (!user) {
    return (
      <div className='flex h-screen w-screen items-center justify-center'>
        "Fetching all messages..."
      </div>
    )
  }
  return (
    <div className="flex h-screen shadow-md overflow-hidden bg-gray-200 dark:bg-gray-800">
      <ConversationPanel
        onSelectUser={setSelectedUsername}
        activeUsername={selectedUsername}
      />
      <div className="flex-1 flex flex-col me-20 bg-white dark:bg-gray-900">
        {activeConversation ? (
          <ChatWindow
            conversation={activeConversation}
            currentUser={user}
            isTyping={isUserTyping(activeConversation.user.id)}
            onSendLocalMessage={handleSendLocalMessage}
          />
        ) : (
          <div className="flex items-center justify-center h-full text-gray-400">Select a conversation</div>
        )}
      </div>
    </div>
  );
}
