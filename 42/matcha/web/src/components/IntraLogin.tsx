import { useEffect } from 'react';
import toast from 'react-hot-toast';
import { useAuth } from '@hooks/useAuth';

export function IntraLogin() {
  const { loginSocial } = useAuth();

  function openIntraPopup() {
    const clientId = import.meta.env.VITE_INTRA_CLIENT_ID;
    const redirectUri = import.meta.env.VITE_INTRA_REDIRECT_URI;
    const url = `https://api.intra.42.fr/oauth/authorize?client_id=${clientId}&redirect_uri=${redirectUri}&response_type=code`;

    const width = 600;
    const height = 600;
    const left = window.screenX + (window.outerWidth - width) / 2;
    const top = window.screenY + (window.outerHeight - height) / 2;

    window.open(
      url,
      'IntraLogin',
      `width=${width},height=${height},left=${left},top=${top}`
    );
  }

  useEffect(() => {
    window.addEventListener('message', async (event) => {
      if (event.origin !== window.location.origin) return;
      const { intra_user_id, first_name, last_name, email, picture_url } = event.data;

      if (intra_user_id) {
        try {
          await loginSocial({
            provider: 'intra',
            provider_user_id: intra_user_id,
            first_name,
            last_name,
            email,
            picture_url
          });
          window.location.href = '/';
        } catch {
          toast.error('Intra login failed.');
        }
      }
    });

    return () => {
      window.removeEventListener('message', () => {});
    };
  }, [loginSocial]);

  return (
    <button
      onClick={openIntraPopup}
      className="py-2 px-4 bg-black text-white rounded-md hover:bg-gray-950 transition-colors"
    >
      Login with 42 Intra
    </button>
  );
}
