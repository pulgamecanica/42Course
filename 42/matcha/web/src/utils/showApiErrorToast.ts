import { AxiosError } from 'axios';
import toast from 'react-hot-toast';

interface ErrorResponse {
  error: string;
  details?: string[];
}

export function showApiErrorToast(error: unknown) {
  if (error && typeof error === 'object' && 'isAxiosError' in error) {
    const err = error as AxiosError<ErrorResponse>;
    const data = err.response?.data;

    if (data) {
      toast.error(data.error);
      if (data.details && Array.isArray(data.details)) {
        data.details.forEach(detail => toast.error(detail));
      }
    } else {
      toast.error('Something went wrong. Please try again.');
    }
  } else {
    toast.error('Unexpected error occurred.');
  }
}
