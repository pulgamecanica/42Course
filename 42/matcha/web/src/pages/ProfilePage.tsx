import { useUserMe } from '@/hooks/useUserMe';
import { ProfileHeader } from '@/components/profile/ProfileHeader';
import { TagList } from '@/components/profile/TagList';
import { LocationCard } from '@/components/profile/LocationCard';
import { PictureGallery } from '@/components/profile/PictureGallery';
import LoadingScreen from '@/components/LoadingScreen';
import { LocationEditorModal } from '@/components/LocationEditorModal';
import { useEffect, useState } from 'react';
import { Pencil } from 'lucide-react';
import { toPublicUser } from '@/utils/toPublicUser';
import { ProfileStats } from '@/components/profile/ProfileStats';
import { PublicUser } from '@/types/user';
import { ScheduledDate } from '@/types/scheduledDate';
import { ScheduledDatesModal } from '@/components/ScheduledDatesModal';


export function ProfilePage() {
  const {
    user,
    tags,
    pictures,
    profilePicture,
    location,
    views,
    viewers,
    loading,
    likes,
    likedBy,
    matches,
    connections,
    scheduledDates,
    reloadRelationships,
  } = useUserMe();
  const [showModal, setShowModal] = useState(false);
  const [showDatesModal, setShowDatesModal] = useState(false);
  const [publicUser, setPublicUser] = useState<PublicUser | null>(null);

  useEffect(() => {
    if (!user) return;
    reloadRelationships();
  }, [user]);

  useEffect(() => {
    if (!user) return;
    setPublicUser(toPublicUser(
      user,
      tags,
      pictures,
      views as unknown as PublicUser[],
      viewers as unknown as PublicUser[],
      likes as unknown as PublicUser[],
      likedBy as unknown as PublicUser[],
      matches as unknown as PublicUser[],
      connections as unknown as PublicUser[],
    ))
  }, [user, tags, pictures, views, viewers, likes, likedBy, matches, connections]);
  
  if (loading || !user || !publicUser) return <LoadingScreen />;

  return (
    <div className="max-w-3xl mx-auto px-4 py-6 flex flex-col items-center justify-center min-h-screen">
      <ProfileHeader user={publicUser} profilePicture={profilePicture} location={location} />
      <div className="flex gap-2">
        <LocationCard location={location} />
        <button
          onClick={() => setShowModal(true)}
          className="mt-2 text-sm text-blue-600 dark:text-blue-400 underline hover:text-blue-800 dark:hover:text-blue-300"
        >
          <Pencil className='w-4 h-4' />
        </button>
        {showModal && (
          <LocationEditorModal
            initialLocation={location}
            onClose={() => setShowModal(false)}
          />
        )}
      </div>
      <ProfileStats showMatches showMessage user={publicUser} />

      <TagList tags={tags} />
      <button
        className="mt-6 px-4 py-2 rounded-md text-white bg-green-600 hover:bg-green-700 disabled:opacity-50"
        onClick={() => setShowDatesModal(true)}
      >
        📅 See My Dates
      </button>
      {showDatesModal && (
        <ScheduledDatesModal
          dates={scheduledDates as ScheduledDate[]}
          onClose={() => setShowDatesModal(false)}
        />
      )}
      <h3 className="font-bold mt-4 text-lg">📷 Pictures</h3>
      <PictureGallery pictures={pictures} />
    </div>
  );
}
