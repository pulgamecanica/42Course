import { SocialLoginData } from '@/context/AuthContext';
import axiosInstance from './axios';

export type AuthCredentials = {
  username: string;
  password: string;
};

export type RegisterData = {
  username: string;
  email: string;
  password: string;
  first_name: string;
  last_name: string;
};

export async function registerUser(data: RegisterData): Promise<void> {
  await axiosInstance.post('/auth/register', data);
}

export async function confirmUser(username: string): Promise<void> {
  await axiosInstance.post('/auth/confirm', { username });
}

export async function loginUser(credentials: AuthCredentials): Promise<string> {
  const response = await axiosInstance.post('/auth/login', credentials);
  const { token } = response as unknown as { token: string };
  return token;
}

export async function loginSocialUser(data: SocialLoginData): Promise<string> {
  const response = await axiosInstance.post('/auth/social', data);
  const { token } = response as unknown as { token: string };
  return token;
}

export function logoutUser(): void {
  localStorage.removeItem('token');
}
