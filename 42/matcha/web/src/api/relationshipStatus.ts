import axios from '@/api/axios';
import { PublicUser } from '@/types/user';

export type RelationshipStatusType = {
  liked: boolean;
  likedBy: boolean;
  matched: boolean;
  connected: boolean;
  blocked: boolean;
  matches: PublicUser[];
};

export async function getRelationshipStatus(username: string, currentUsername: string): Promise<RelationshipStatusType | null> {
  if (!username || username === currentUsername) return null;

  try {
    const [likes, likedByList, matches, connections, blockedUsers, matchList] = await Promise.all([
      axios.get('/me/likes'),
      axios.get('/me/liked_by'),
      axios.get('/me/matches'),
      axios.get('/me/connections'),
      axios.get('/me/blocked'),
      axios.get('/me/matches'),

    ]);
    return {
      liked: ((likes as unknown as PublicUser[]).some((u) => u.username === username)),
      likedBy: ((likedByList as unknown as PublicUser[]).some((u) => u.username === username)),
      matched: ((matches as unknown as PublicUser[]).some((u) => u.username === username)),
      connected: ((connections as unknown as PublicUser[]).some((u) => u.username === username)),
      blocked: ((blockedUsers as unknown as string[]).some((u) => u === username)),
      matches: (matchList as unknown as PublicUser[])
    };
  } catch (e) {
    console.error('Failed to fetch relationship status', e);
  }
  return null;
}
