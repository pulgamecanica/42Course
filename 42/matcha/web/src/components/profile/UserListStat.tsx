import { MailOpen } from 'lucide-react';
import { useNavigate } from 'react-router-dom';
import { PublicUser } from '@/types/user';

type Props = {
  users: PublicUser[];
  showMessage?: boolean;
  isConnected: (user: PublicUser) => boolean;
};

export function UserListStat({ users, showMessage, isConnected }: Props) {
  const navigate = useNavigate();
  const uniqueUsers = Array.from(new Map(users.map(u => [u.id, u])).values());

  return (
    <ul className="text-sm space-y-1">
      {uniqueUsers.map((user, i) => (
        <li
          key={i}
          className="flex justify-between items-center gap-2 cursor-pointer"
        >
          <span
            onClick={() => navigate(`/profile/${user.username}`)}
            className="hover:underline"
          >
            {user.username}
          </span>
          {showMessage && isConnected(user) &&(
            <MailOpen
              className="h-4 w-4 text-blue-500 hover:text-blue-700"
              onClick={(e) => {
                e.stopPropagation();
                navigate(`/conversations?user=${user.username}`);
              }}
            />
          )}
        </li>
      ))}
    </ul>
  );
}
