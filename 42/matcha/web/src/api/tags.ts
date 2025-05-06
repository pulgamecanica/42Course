import axiosInstance from "@/api/axios";
import { Tag } from "@/types/tag";

export async function fetchAllTags(): Promise<string[]> {
  const response = await axiosInstance.get<Tag[]>("/tags");
  return (response as unknown as Tag[]).map((tag: { name: string }) => tag.name);
}