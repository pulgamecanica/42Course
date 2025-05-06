import { PublicUser } from '@/types/user';
import { Picture } from '@/types/picture';
import { Location } from '@/types/location';

type Props = {
  user: PublicUser;
  profilePicture: Picture | null;
  location?: Location | null;
};
export function ProfileHeader({ user, profilePicture, location }: Props) {
  
  const isOnline = user.online_status;
  const lastSeen = location?.city || location?.country || null;
  const calcAge = (birthYear: string) => {
    const birth = new Date(`${birthYear}-01-01`);
    const today = new Date();
    let age = today.getFullYear() - birth.getFullYear();
    const m = today.getMonth() - birth.getMonth();
    if (m < 0 || (m === 0 && today.getDate() < birth.getDate())) age--;
    return age;
  };
  const age = calcAge(user.birth_year);

  return (
    <div className="flex items-center gap-4">
      <div className='relative'>
      <img
        src={profilePicture?.url ?? '/placeholder-profile.jpg'}
        alt="Profile"
        className="w-24 h-24 rounded-full object-cover border-2 border-gray-300 dark:border-gray-700"
      />
      <div className='bottom-0 flex items-center justify-center right-0 text-xs w-2 h-2 absolute dark:border-gray-700 p-3 border dark:bg-gray-900 bg-gray-100 border-gray-300 rounded-full'>{parseFloat(user.fame_rating).toFixed(0)}</div>

      </div>
      <div>
        <div className="flex items-center gap-2">
          <h2 className="text-xl font-bold">
            {user.last_name}
          </h2>
          <span
            className={`inline-block w-3 h-3 rounded-full ${
              isOnline ? 'bg-green-500' : 'bg-gray-400'
            }`}
            title={isOnline ? 'Online' : `Last seen at: ${user.last_seen_at}`}
          />
        </div>

        <p className="text-sm text-gray-500">@{user.username}</p>

        {/* ✨ New: Age · Gender · Sexual Preference */}
        <p className="text-xs text-gray-400 mt-1">
          {age} years old · {user.gender} · Interested in {user.sexual_preferences}
        </p>

        {user.biography && (
          <p className="mt-1 text-sm text-gray-400 italic">"{user.biography}"</p>
        )}

        {!isOnline && lastSeen && (
          <p className="text-xs text-gray-400 mt-1">Last seen: {lastSeen}</p>
        )}
      </div>
    </div>
  );
}
