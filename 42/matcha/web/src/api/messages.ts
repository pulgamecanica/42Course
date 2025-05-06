import axiosInstance from "@/api/axios";
import { Conversation } from "@/types/conversation";

export async function fetchAllMessages(): Promise<Conversation[]> {
  const response = await axiosInstance.get<Conversation[]>("/me/messages");
  return (response as unknown as Conversation[]);
}

export async function fetchMessagesFrom(username: string): Promise<Conversation> {
  const response = await axiosInstance.get<Conversation>(`/me/messages/${username}`);
  return (response as unknown as Conversation);
}