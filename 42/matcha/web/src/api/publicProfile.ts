import axiosInstance from "@/api/axios";
import { PublicUser } from "@/types/user";

export async function fetchPublicProfile(
  username: string,
  countView: boolean = true
): Promise<PublicUser> {
  const query = countView ? '' : '?count_view=false';
  const response = await axiosInstance.get<{ data: PublicUser }>(`/users/${username}${query}`);
  return response as unknown as PublicUser;
}
