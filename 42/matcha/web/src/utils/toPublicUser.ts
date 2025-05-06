import { User, PublicUser } from '@/types/user';
import { Picture } from '@/types/picture';
import { Tag } from '@/types/tag';

export function toPublicUser(
  user: User,
  tags: Tag[],
  pictures: Picture[],
  views: PublicUser[],
  viewers: PublicUser[],
  likes: PublicUser[],
  liked_by: PublicUser[],
  matches: PublicUser[],
  connections: PublicUser[],
): PublicUser {
  return {
    id: user.id,
    username: user.username,
    first_name: user.first_name,
    last_name: user.last_name,
    biography: user.biography,
    gender: user.gender,
    sexual_preferences: user.sexual_preferences,
    birth_year: user.birth_year.toString(),
    profile_picture_id: user.profile_picture_id,
    latitude: user.latitude,
    longitude: user.longitude,
    online_status: user.online_status,
    last_seen_at: user.last_seen_at,
    pictures,
    tags,
    views: views,
    visitors: viewers,
    likes: likes,
    liked_by: liked_by,
    matches: matches,
    connections : connections,
    fame_rating: user.fame_rating,
  };
}
