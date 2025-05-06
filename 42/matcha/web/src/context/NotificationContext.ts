import { createContext } from "react";
import { Notification } from "@/types/notification"

export type NotificationContextType = {
  notifications: Notification[];
  markNotificationAsRead: (id: number) => void;
  removeNotification: (id: number) => void;
  markAllAsRead: () => void;
  hasUnread: boolean;
};

export const NotificationContext = createContext<NotificationContextType>({
  notifications: [],
  markNotificationAsRead: () => {},
  removeNotification: () => {},
  markAllAsRead: () => {},
  hasUnread: false,
});
