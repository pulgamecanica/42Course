import { useAuth } from '@hooks/useAuth';
import LoadingScreen from '@components/LoadingScreen';
import { JSX, useEffect } from 'react';
import { ErrorBlockerMessage } from '@components/ErrorBlockerMessage';
import { useNavigate } from 'react-router-dom';
import { useUserMe } from '@/hooks/useUserMe';

export function ProtectedRoute({ children }: { children: JSX.Element }) {
  const { token, isAuthenticated, loading } = useAuth();
  const { profileSetupComplete, user } = useUserMe();
  const navigate = useNavigate();

  useEffect(() => {
    if (loading) return;
    if (!isAuthenticated) {
      navigate('/login');
    }
    if (user && isAuthenticated && !profileSetupComplete && location.pathname !== '/setup') {
      navigate('/setup');
    }
  }, [loading, isAuthenticated, profileSetupComplete, navigate, user]);

  if (!token || !isAuthenticated) {
    return <ErrorBlockerMessage message="Session expired. Please log in again. ⚠️" locationMessage="Go to Login" />;
  }

  if (loading || !user || !profileSetupComplete && location.pathname !== '/setup') {
    return <LoadingScreen />;
  }

  return children;
}