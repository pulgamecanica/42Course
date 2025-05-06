import { MatchFilters, MatchResult } from '@/types/match';
import axiosInstance from '@/api/axios';
import toast from 'react-hot-toast';

type DiscoverProps = {
  filters: MatchFilters;
};

export async function discoverMatches({
  filters,
}: DiscoverProps): Promise<MatchResult[]> {
  try {
    const response = await axiosInstance.post<MatchResult[]>(
      '/me/discover',
      filters,
    );
    return response as unknown as MatchResult[];
  } catch {
    toast.error('Something went wrong.');
    return [];
  }
}
