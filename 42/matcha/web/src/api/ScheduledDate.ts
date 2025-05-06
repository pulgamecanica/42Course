import axiosInstance from '@/api/axios';
import { ScheduledDateResponse, SetDatePayload } from '@/types/scheduledDate';
import toast from 'react-hot-toast';

export async function setDate(payload: SetDatePayload): Promise<ScheduledDateResponse | null> {
  try {
    console.log("Setting date", payload)
    const data  = await axiosInstance.post<ScheduledDateResponse>('/me/dates', payload);

    toast.success('Date successfully scheduled!');
    return data as unknown as ScheduledDateResponse;
  } catch {
    toast.error('Something went wrong while scheduling the date.');
    return null;
  }
}

export async function deleteDate(id: number) {
  try {
    await axiosInstance.delete(`/me/dates/${id}`);
    toast.success('Successfully deleted the date');
  } catch {
    toast.error('Something went wrong could not delete the dates');
  }
}
