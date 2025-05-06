import { ConversationItem } from '@/components/chat/ConversationItem';
import { useMessages } from '@/hooks/useMessages';
import { useUserMe } from '@/hooks/useUserMe';
import { LucideSearch } from 'lucide-react';
import { useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';

export function ConversationPanel({
  onSelectUser,
  activeUsername,
}: {
  onSelectUser: (username: string) => void;
  activeUsername: string | null;
}) {
  const [search, setSearch] = useState('');
  const { conversations, isUserTyping } = useMessages();
  const { user, profilePicture } = useUserMe();
  const navigate = useNavigate();

  const filteredConversations = conversations.filter((conv) =>
    conv.user.username.toLowerCase().includes(search.toLowerCase())
  );

  return (
    <div className="w-72 border-r dark:border-gray-500 bg-gray-100 dark:bg-gray-800 flex flex-col">
      <div className="border-b dark:border-gray-500">
        <Link to="/profile" className="flex items-center gap-2 hover:opacity-90 transition hover:bg-gray-300 hover:dark:bg-gray-600 p-4">
          <img
            src={profilePicture?.url || '/default.png'}
            alt="Profile"
            className="h-10 w-10 rounded-full object-cover"
          />
          <div className="text-sm font-semibold text-gray-900 dark:text-white">
            {user?.username}
          </div>
        </Link>
      </div>

      <div className="px-2 py-6">
        <div className="relative">
          <input
            type="text"
            placeholder="Search..."
            value={search}
            onChange={(e) => setSearch(e.target.value)}
            className="w-full pl-10 pr-2 py-1.5 text-sm rounded bg-white dark:bg-gray-700 border dark:border-gray-500"
          />
          <LucideSearch className="absolute left-2 top-2.5 h-4 w-4 text-gray-400" />
        </div>
      </div>

      <hr className='dark:border-gray-500' />

      <div className="overflow-y-auto flex-1">
        {filteredConversations.map((conv) => (
          <ConversationItem
            key={conv.user.username}
            user={conv.user}
            lastMessage={conv.messages[conv.messages.length - 1]}
            isActive={conv.user.username === activeUsername}
            isTyping={isUserTyping(conv.user.id)}
            onClick={() => {
              onSelectUser(conv.user.username);
              navigate(`?user=${conv.user.username}`, { replace: true });
            }}
          />
        ))}
      </div>
    </div>
  );
}
