import axiosInstance from './axios';
import toast from 'react-hot-toast';

export type ReportPayload = {
  username: string;
  reason: string;
  description: string;
};

export async function reportUser(payload: ReportPayload): Promise<void> {
  try {
    await axiosInstance.post('/me/report', payload);
    toast.success('User successfully reported!');
  } catch {
    toast.error('Something went wrong while reporting the user please try again.');
  }
}
