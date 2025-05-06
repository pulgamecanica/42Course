export type Picture = {
  id: number;
  user_id: number;
  url: string;
  is_profile: 'f' | 't' | null;
  created_at: string;
  updated_at: string;
};
