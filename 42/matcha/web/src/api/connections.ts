import axiosInstance from "@/api/axios";
import { PublicUser } from "@/types/user";

export async function fetchConnections(): Promise<PublicUser[]> {
  const response = await axiosInstance.get<PublicUser[]>("/me/connections");
  return (response as unknown as PublicUser[]);
}
