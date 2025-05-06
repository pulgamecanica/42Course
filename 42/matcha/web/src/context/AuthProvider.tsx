import { ReactNode, useEffect, useState } from 'react';
import { jwtDecode } from 'jwt-decode';
import { loginUser, logoutUser, registerUser, confirmUser, RegisterData, loginSocialUser } from '@api/authService';
import { AuthContext, SocialLoginData } from '@context/AuthContext';

export function AuthProvider({ children }: { children: ReactNode }) {
  const [token, setToken] = useState<string | null>(null);
  const [loading, setLoading] = useState(true);
  const [isAuthenticated, setIsAuthenticated] = useState(false);

  useEffect(() => {
    const storedToken = localStorage.getItem('token');
    
    async function initializeAuth() {
      try {
        if (storedToken && !isTokenExpired(storedToken)) {
          setToken(storedToken);
          setLoading(true);
          setIsAuthenticated(true);
        }
      } catch {
        logoutUser();
        setToken(null);
        setIsAuthenticated(false);
      }
      setLoading(false);
    }
    initializeAuth();
  }, []);

  const isTokenExpired = (token: string): boolean => {
    const decoded = jwtDecode(token);
    const currentTime = Date.now() / 1000;
    if (!decoded.exp) {
      return true;
    }
  
    return decoded.exp < currentTime;
  };

  async function login(username: string, password: string) {
    const newToken = await loginUser({ username, password });
    localStorage.setItem('token', newToken);
    setToken(newToken);
    setIsAuthenticated(true);
  }
  
  async function loginSocial({
    provider,
    provider_user_id,
    first_name = '',
    last_name = '',
    email = '',
    picture_url = '',
  }: SocialLoginData): Promise<void> {
    const newToken = await loginSocialUser({
      provider,
      provider_user_id,
      first_name,
      last_name,
      email,
      picture_url,})

    localStorage.setItem('token', newToken);
    setToken(newToken);
    setIsAuthenticated(true);
  }

  
  function logout() {
    logoutUser();
    setToken(null);
    setIsAuthenticated(false);
  };

  async function register(data: RegisterData) {
    await registerUser(data);
  }

  async function confirm(username: string) {
    await confirmUser(username);
  }

  const value = {
    token,
    isAuthenticated,
    login,
    loginSocial,
    logout,
    register,
    confirm,
    loading,
  };

  return <AuthContext.Provider value={value}>{children}</AuthContext.Provider>;
}
