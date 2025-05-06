import { useContext } from 'react';
import { AuthContext } from '@context/AuthContext';

export function useAuth() {
  return useContext(AuthContext);
}
