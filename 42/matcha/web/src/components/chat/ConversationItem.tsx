import { Message } from "@/types/message";
import { PublicUser } from "@/types/user";

export function ConversationItem({
  user,
  lastMessage,
  isActive,
  isTyping,
  onClick
}: {
  user: PublicUser;
  lastMessage: Message;
  isActive: boolean;
  isTyping: boolean;
  onClick: () => void;
}) {
  if (!user) return;

  const profilePic = (user.pictures || []).find((p) => p.is_profile === "t")?.url;
  return (
    <div
      onClick={onClick}
      className={`cursor-pointer px-4 py-3 text-sm border-b dark:border-gray-400 hover:bg-gray-200 dark:hover:bg-gray-700 ${
        isActive ? 'bg-gray-200 dark:bg-gray-700' : ''
      }`}
    >
      <div className="flex items-center gap-3">
        <img src={profilePic || '/default.png'} className="w-8 h-8 rounded-full object-cover" />
        <div className="flex-1">
          <div className="font-medium">{user.username}</div>
          <div className="max-w-[180px] truncate text-xs text-gray-500 dark:text-gray-400">
            {isTyping ? 'Typing...' : lastMessage?.content || 'No messages yet'}
          </div>
        </div>
      </div>
    </div>
  );
}
