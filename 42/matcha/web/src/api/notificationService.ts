import axiosInstance from '@/api/axios';
import { Notification } from '@/types/notification';

export async function getNotifications(): Promise<Notification[]> {
  const res = await axiosInstance.get<Notification[]>('/me/notifications');
  return res as unknown as Notification[];
}

export async function markAsRead(id: number) {
  await axiosInstance.patch(`/me/notifications/${id}`);
}

export async function deleteNotification(id: number) {
  await axiosInstance.delete(`/me/notifications/${id}`);
}
