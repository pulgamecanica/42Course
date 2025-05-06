import { useState } from 'react';
import { useUserMe } from '@/hooks/useUserMe';
import toast from 'react-hot-toast';

export const PictureManager = () => {
  const {
    pictures,
    profilePicture,
    uploadPicture,
    setProfilePicture,
    deletePicture,
  } = useUserMe();

  const [newPicUrl, setNewPicUrl] = useState('');
  const [submitting, setSubmitting] = useState(false);

  const handleUpload = async () => {
    const url = newPicUrl.trim();
    if (!url) return;

    setSubmitting(true);
    try {
      await uploadPicture(url);
      toast.success('Picture uploaded!');
      setNewPicUrl('');
    } catch {
      toast.error('Failed to upload picture');
    } finally {
      setSubmitting(false);
    }
  };

  const handleSetProfile = async (id: number) => {
    try {
      await setProfilePicture(id);
      toast.success('Set as profile picture');
    } catch {
      toast.error('Failed to update profile picture');
    }
  };

  const handleDelete = async (id: number) => {
    try {
      await deletePicture(id);
      toast.success('Picture deleted');
    } catch {
      toast.error('Failed to delete picture');
    }
  };

  return (
    <div className="mb-6">
      <h3 className="text-lg font-semibold mb-2">Pictures</h3>

      <div className="flex gap-2 mb-4">
        <input
          type="text"
          placeholder="Paste image URL"
          value={newPicUrl}
          onChange={(e) => setNewPicUrl(e.target.value)}
          className="flex-1 px-2 py-1 rounded border border-gray-300 dark:border-gray-600 bg-white dark:bg-gray-900"
          disabled={submitting}
        />
        <button
          onClick={handleUpload}
          className="px-3 py-1 bg-blue-600 text-white rounded hover:bg-blue-700 disabled:opacity-50"
          disabled={submitting}
        >
          Upload
        </button>
      </div>

      <div className="grid grid-cols-3 sm:grid-cols-4 gap-4">
        {profilePicture && (
          <div key={profilePicture.id} className="relative border-2 border-green-500 rounded overflow-hidden">
            <img src={profilePicture.url} alt="Profile" className="w-full h-28 object-cover" />
            <div className="absolute top-0 right-0 p-1 bg-green-600 text-xs text-white">Profile</div>
          </div>
        )}

        {pictures.map((pic) => (
          <div key={pic.id} className="relative border rounded overflow-hidden">
            <img src={pic.url} alt="Picture" className="w-full h-28 object-cover" />
            <div className="absolute top-1 right-1 flex gap-1">
              <button
                className="text-green-500 text-xs bg-white rounded px-1 py-0.5"
                onClick={() => handleSetProfile(pic.id)}
              >
                Set as Profile
              </button>
              <button
                className="text-red-500 text-xs bg-white rounded px-1 py-0.5"
                onClick={() => handleDelete(pic.id)}
              >
                ✕
              </button>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
};
