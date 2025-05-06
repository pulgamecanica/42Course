import React, { useState, useEffect } from 'react';
import { useNavigate, useLocation, Link } from 'react-router-dom';
import axiosInstance from '@/api/axios';
import toast from 'react-hot-toast';
import LoadingScreen from '@components/LoadingScreen';

export function NewPasswordPage() {
  const [newPassword, setNewPassword] = useState('');
  const [submitting, setSubmitting] = useState(false);
  const [code, setCode] = useState<string | null>(null);
  const navigate = useNavigate();
  const location = useLocation();

  useEffect(() => {
    const searchParams = new URLSearchParams(location.search);
    const resetCode = searchParams.get('code');
    if (resetCode) {
      setCode(resetCode);
    } else {
      toast.error('Invalid or missing reset code.');
      navigate('/recover-password');
    }
  }, [location, navigate]);

  async function handleSubmit(e: React.FormEvent) {
    e.preventDefault();
    if (!code) return;

    setSubmitting(true);
    try {
      await axiosInstance.post(`/email/reset-password/${code}`, {
        new_password: newPassword,
      });
      toast.success('Password updated! You can now log in.');
      navigate('/login');
    } catch {
      // Error handled globally by axios interceptor
    } finally {
      setSubmitting(false);
    }
  }

  if (submitting) {
    return <LoadingScreen />;
  }

  return (
    <div className="flex justify-center items-center min-h-screen px-4">
      <div className="w-full max-w-md p-6 pb-4 bg-white dark:bg-gray-800 rounded-lg shadow-md">
        <h1 className="text-2xl font-semibold mb-6 text-center text-gray-900 dark:text-white">
          Reset Your Password
        </h1>

        <form onSubmit={handleSubmit} className="space-y-5">
          <div>
            <label className="block text-sm font-medium mb-1 text-gray-700 dark:text-gray-200">
              New Password
            </label>
            <input
              type="password"
              autoComplete="new-password"
              value={newPassword}
              onChange={(e) => setNewPassword(e.target.value)}
              className="w-full px-4 py-2 rounded-md border border-gray-300 dark:border-gray-600 bg-white dark:bg-gray-700 text-gray-900 dark:text-white focus:outline-none focus:ring-2 focus:ring-blue-500"
              required
              minLength={6}
            />
          </div>

          <button
            type="submit"
            disabled={submitting}
            className="w-full py-2 px-4 bg-blue-600 hover:bg-blue-700 text-white rounded-md transition-colors disabled:opacity-50 disabled:cursor-not-allowed"
          >
            {submitting ? 'Resetting...' : 'Reset Password'}
          </button>
        </form>

        <p className="mt-4 text-center text-sm text-gray-600 dark:text-gray-300">
          Remembered your password?{' '}
          <Link to="/login" className="text-blue-600 dark:text-blue-400 hover:underline">
            Back to Login
          </Link>
        </p>
      </div>
    </div>
  );
}
