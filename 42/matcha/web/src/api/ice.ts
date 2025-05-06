import axiosInstance from './axios';
import toast from 'react-hot-toast';
import { IceServer } from '@/types/iceServer';

export async function fetchIceServers(): Promise<IceServer[]> {
  try {
    const servers = await axiosInstance.get('/ice');
    return servers as unknown as IceServer[];
  } catch {
    toast.error('Failed to retrieve ICE servers for call setup.');
    return [];
  }
}
