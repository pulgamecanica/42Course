import { useState } from 'react';
import axios from '@/api/axios';
import toast from 'react-hot-toast';
import { RelationshipStatusType } from '@api/relationshipStatus';
import { useMessages } from '@/hooks/useMessages';
import { PublicUser } from '@/types/user';
import { useUserMe } from '@/hooks/useUserMe';
import { DatesModal } from '@/components/SetScheduledDatesModal';

type PublicProfileActionsProps = {
  user: PublicUser;
  relationship: RelationshipStatusType;
  refresh: () => void;
  scheduledAt: string;
  setScheduledAt: (val: string) => void;
  location: string;
  setLocation: (val: string) => void;
  note: string;
  setNote: (val: string) => void;
  clearForm: () => void;
};

export function PublicProfileActions({
  user,
  relationship,
  refresh,
  scheduledAt,
  setScheduledAt,
  location,
  setLocation,
  note,
  setNote,
  clearForm,
}: PublicProfileActionsProps) {


  const { liked, likedBy, matched, connected, blocked } = relationship;
  const { startConversationWith, removeConversationWith } = useMessages();
  const { refreshMatches } = useUserMe();
  const username = user.username;
  const [isLoading, setIsLoading] = useState(false);
  const [isModalOpen, setIsModalOpen] = useState(false);


  const handleLikeToggle = async () => {
    setIsLoading(true);
    try {
      if (liked) {
        await axios.delete('/me/like', { data: { username } });
        removeConversationWith(user);
        toast.success('Unliked');
      } else {
        await axios.post('/me/like', { username });
        toast.success('Liked!');
      }
      refreshMatches();
    } catch {
      toast.error('Failed to update like');
    } finally {
      await refresh();
      setIsLoading(false);
    }
  };

  const handleBlock = async () => {
    setIsLoading(true);
    try {
      await axios.post('/me/block', { username });
      refreshMatches();
      toast.success('User blocked');
    } catch {
      toast.error('Failed to block');
    } finally {
      await refresh();
      setIsLoading(false);
    }
  };

  const handleConnect = async () => {
    setIsLoading(true);
    try {
      await axios.post('/me/connect', { username });
      startConversationWith(user);
      toast.success('Connected!');
    } catch {
      toast.error('Failed to connect');
    } finally {
      await refresh();
      setIsLoading(false);
    }
  };

  const handleDisconnect = async () => {
    setIsLoading(true);
    try {
      await axios.delete('/me/connect', { data: { username } });
      removeConversationWith(user);
      toast.success('Disconnected');
    } catch {
      toast.error('Failed to disconnect');
    } finally {
      await refresh();
      setIsLoading(false);
    }
  };
  return (
    <div className="w-full mt-6 space-y-4">
      {/* 🩷 Match Label */}
      {matched ? (
        <div className="text-center">
          <span className="text-pink-500 font-semibold text-lg animate-bounce">
            💖 It's a Match!
          </span>
        </div>
      ) :
        (likedBy && (
          <div className="text-center">
            <span className="text-pink-500 font-semibold text-lg animate-bounce">
              ❤️ likes you!
            </span>
          </div>
        ))}

      {/* 🤝 Core Actions */}
      <div className="flex flex-wrap justify-center gap-3">
        <button
          disabled={isLoading}
          onClick={handleLikeToggle}
          className="px-4 py-2 rounded-md text-white bg-pink-600 hover:bg-pink-700 disabled:opacity-50"
        >
          {liked ? '💔 Unlike' : '❤️ Like'}
        </button>

        {matched && !connected && (
          <button
            disabled={isLoading}
            onClick={handleConnect}
            className="px-4 py-2 rounded-md text-white bg-blue-600 hover:bg-blue-700 disabled:opacity-50"
          >
            🔗 Connect
          </button>
        )}

        {connected && (
          <button
            disabled={isLoading}
            onClick={handleDisconnect}
            className="px-4 py-2 rounded-md text-white bg-gray-600 hover:bg-gray-700 disabled:opacity-50"
          >
            ❌ Disconnect
          </button>
        )}
      </div>

      {/* 🔒 Secondary Actions */}
      <div className="flex flex-wrap justify-center gap-3">
        {connected && (
          <button
          onClick={() => {
          
            setIsModalOpen(true);
          }}
          
            className="px-4 py-2 rounded-md text-white bg-green-600 hover:bg-green-700 disabled:opacity-50"
          >
            📅 Propose Date
          </button>
        )}

        {/* Render modal only if open */}
        {isModalOpen && (
          <DatesModal
            onClose={() => setIsModalOpen(false)}
            username={user.username}
            scheduledAt={scheduledAt}
            setScheduledAt={setScheduledAt}
            location={location}
            setLocation={setLocation}
            note={note}
            setNote={setNote}
            clearForm={clearForm}
          />
        )}

        {!blocked && (
          <button
            disabled={isLoading}
            onClick={handleBlock}
            className="px-4 py-2 rounded-md text-white bg-red-600 hover:bg-red-700 disabled:opacity-50"
          >
            🚫 Block
          </button>
        )}
      </div>
    </div>
  );
}