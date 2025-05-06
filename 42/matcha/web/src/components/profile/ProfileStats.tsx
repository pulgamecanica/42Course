import { JSX, useState, useEffect } from 'react';
import { PublicUser } from '@/types/user';
import { StatItem } from '@/components/profile/StatItem';
import { StatSidebar } from '@/components/profile/StatSidebar';
import { UserListStat } from '@/components/profile/UserListStat';

type Props = {
  user: PublicUser;
  showMessage?: boolean;
  showMatches?: boolean;
};

type StatType = 'likes' | 'liked_by' | 'views' | 'visits' | 'matches';

export function ProfileStats({ user, showMessage = false, showMatches = false }: Props) {
  const [showSidebar, setShowSidebar] = useState(false);
  const [activeStat, setActiveStat] = useState<StatType | null>(null);
  const likesReceived = user.liked_by.length;

  const handleClick = (stat: StatType) => {
    setActiveStat(stat);
    setShowSidebar(true);
  };

  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      if (e.key === 'Escape') {
        setShowSidebar(false);
      }
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, []);

  const isConnected = (otherUser: PublicUser) => {
    return user.connections?.some((connection) => connection.id === otherUser.id);
  };

  const statContent: Record<StatType, JSX.Element> = {
    likes: <UserListStat users={user.likes} showMessage={showMessage} isConnected={isConnected} />,
    liked_by: <UserListStat users={user.liked_by} showMessage={showMessage} isConnected={isConnected} />,
    views: <UserListStat users={user.views} showMessage={showMessage} isConnected={isConnected} />,
    visits: <UserListStat users={user.visitors} showMessage={showMessage} isConnected={isConnected} />,
    matches: <UserListStat users={user.matches} showMessage={showMessage} isConnected={isConnected} />,
  };

  return (
    <>
      <div className="w-full flex justify-around text-sm text-gray-700 dark:text-gray-300 my-4">
        {showMatches &&
          <StatItem
            label="Matches"
            value={user.matches.length}
            color="text-blue-600 dark:text-blue-400"
            onClick={() => handleClick('matches')}
          />}
        <StatItem
          label="Likes Sent"
          value={user.likes.length}
          color="text-pink-600 dark:text-pink-400"
          onClick={() => handleClick('likes')}
        />
        <StatItem
          label="Likes Received"
          value={likesReceived}
          color="text-pink-600 dark:text-pink-400"
          onClick={() => handleClick('liked_by')}
        />
        <StatItem
          label="Views"
          value={user.views.length}
          color="text-blue-600 dark:text-blue-400"
          onClick={() => handleClick('views')}
        />
        <StatItem
          label="Visits"
          value={user.visitors.length}
          color="text-green-600 dark:text-green-400"
          onClick={() => handleClick('visits')}
        />
      </div>

      <StatSidebar
        visible={showSidebar}
        title={activeStat?.replace('_', ' ') || ''}
        onClose={() => setShowSidebar(false)}
      >
        {activeStat && statContent[activeStat]}
      </StatSidebar>
    </>
  );
}
