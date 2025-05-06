import { createContext } from 'react';
import { RegisterData } from '@api/authService';

export type SocialLoginData = {
  provider: string;
  provider_user_id: string;
  first_name?: string;
  last_name?: string;
  email?: string;
  picture_url?: string;
}

export type AuthContextType = {
  isAuthenticated: boolean;
  token: string | null;
  login: (username: string, password: string) => Promise<void>;
  loginSocial: (data: SocialLoginData) => Promise<void>;
  logout: () => void;
  register: (data: RegisterData) => Promise<void>;
  confirm: (username: string) => Promise<void>;
  loading: boolean;
};

export const AuthContext = createContext<AuthContextType>({} as AuthContextType);
