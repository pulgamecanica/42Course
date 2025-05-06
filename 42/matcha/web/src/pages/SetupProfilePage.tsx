import { useNavigate } from 'react-router-dom';
import { ProfileForm } from '@components/forms/ProfileForm';
import toast from 'react-hot-toast';
import { useUserMe } from '@/hooks/useUserMe';
import { useEffect } from 'react';
import { UpdateUserProfilePayload } from '@/types/user';

export const SetupProfilePage = () => {
  const { profileSetupComplete, updateUser } = useUserMe();
  const navigate = useNavigate();

  useEffect(() => {
    if (profileSetupComplete) {
      toast.success('Profile is already setup, redirecting to /profile/edit');
      navigate('/profile/edit');
    }
  }, [profileSetupComplete, navigate]);

  const handleSubmit = async (data: UpdateUserProfilePayload) => {
    try {
      await updateUser(data);
      toast.success('Profile successfully setup!');
      window.location.href = '/';
      return;
    } catch (error) {
      toast.error(`Profile setup failed. ${error}`);
    }
  };

  return (
    <div className="min-h-screen flex items-center justify-center">
      <div className="container max-w-2xl mx-2 p-6 bg-white dark:bg-gray-800 rounded-md shadow-md">
        <h2 className="text-2xl font-semibold text-center text-gray-900 dark:text-white">Setup Your Profile</h2>
        <ProfileForm onSubmit={handleSubmit} buttonText="Complete Setup" />
      </div>
    </div>
  );
};
