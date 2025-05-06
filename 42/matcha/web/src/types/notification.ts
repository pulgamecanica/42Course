export type NotificationType =
  | 'like'
  | 'unlike'
  | 'view'
  | 'message'
  | 'match'
  | 'connection'
  | 'video_call'
  | 'date'
  | 'other';

export interface Notification {
  id: number;
  user_id: number;
  from_user_id: number | null;
  from_username?: string | null;
  message: string;
  type: NotificationType;
  read: string;
  created_at: string;
}
