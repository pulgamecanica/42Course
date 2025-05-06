import { useState, useEffect } from 'react';
import { useNavigate, useLocation } from 'react-router-dom';
import axiosInstance from '@/api/axios';
import toast from 'react-hot-toast';
import LoadingScreen from '@components/LoadingScreen';

export function ConfirmAccountPage() {
  const [status, setStatus] = useState<'loading' | 'success' | 'error'>('loading');
  const navigate = useNavigate();
  const location = useLocation();

  useEffect(() => {
    const searchParams = new URLSearchParams(location.search);
    const code = searchParams.get('code');

    if (!code) {
      toast.error('Invalid confirmation link.');
      navigate('/register');
      return;
    }

    async function confirmAccount() {
      try {
        await axiosInstance.get(`/email/confirm/${code}`);
        setStatus('success');
        setTimeout(() => {
          navigate('/login');
        }, 3000);
      } catch {
        setStatus('error');
        setTimeout(() => {
          navigate('/register');
        }, 10000);
      }
    }

    confirmAccount();
  }, [location, navigate]);

  if (status === 'loading') {
    return <LoadingScreen />;
  }

  return (
    <div className="flex flex-col justify-center items-center min-h-screen px-4">
      <div className="w-full max-w-md p-8 bg-white dark:bg-gray-800 rounded-lg shadow-md text-center">
        {status === 'success' ? (
          <>
            <h1 className="text-3xl font-bold text-green-600 dark:text-green-400 mb-4">
              🎉 Account Confirmed!
            </h1>
            <p className="text-gray-700 dark:text-gray-300 mb-6">
              Your account has been successfully confirmed.<br />
              Redirecting you to login...
            </p>
          </>
        ) : (
          <>
            <h1 className="text-3xl font-bold text-red-600 dark:text-red-400 mb-4">
              ❌ Confirmation Failed
            </h1>
            <p className="text-gray-700 dark:text-gray-300 mb-6">
              We couldn't confirm your account.<br />
              Redirecting you to registration...
            </p>
          </>
        )}
        <div className="animate-spin rounded-full h-10 w-10 border-t-2 border-b-2 border-blue-500 mx-auto"></div>
      </div>
    </div>
  );
}
