import { useEffect, useRef, useState } from 'react';
import axiosInstance from '@api/axios';

type IntraUser = {
  intra_user_id: string;
  first_name: string;
  last_name: string;
  email: string;
  picture_url: string;
};

export function IntraCallbackPage() {
  const [status, setStatus] = useState<'loading' | 'success' | 'error'>('loading');
  const hasRun = useRef(false);

  useEffect(() => {
    if (hasRun.current) return;
    hasRun.current = true;

    const url = new URL(window.location.href);
    const code = url.searchParams.get('code');

    async function exchangeCodeOnce() {
      if (!code) {
        setStatus('error');
        return;
      }

      try {
        const intraUser = await axiosInstance.post<IntraUser>('/auth/oauth/intra', { code }) as unknown as IntraUser;

        if (window.opener) {
          window.opener.postMessage(
            {
              intra_user_id: intraUser.intra_user_id.toString(),
              first_name: intraUser.first_name,
              last_name: intraUser.last_name,
              email: intraUser.email,
              picture_url: intraUser.picture_url,
            },
            window.location.origin
          );
          setStatus('success');
          console.log('✅ Intra login successful, sent to opener');
          setTimeout(() => window.close(), 3000);
        } else {
          console.warn('⚠️ window.opener is null');
          setStatus('error');
        }
      } catch (err) {
        console.error('❌ Intra login failed', err);
        if (window.opener) {
          window.opener.postMessage({ error: 'intra_login_failed' }, window.location.origin);
        }
        setStatus('error');
        setTimeout(() => window.close(), 3000);
      }
    }

    exchangeCodeOnce();
  }, []);

  return (
    <div className="dark:bg-black text-green-400 font-mono h-screen w-screen flex flex-col items-center justify-center px-4 text-center">
      <div className="text-lg sm:text-xl md:text-2xl animate-fade-in">
        {status === 'loading' && (
          <div>
            <p className="animate-typing overflow-hidden whitespace-nowrap border-r-4 border-green-400 pr-2">
              Logging in with Intra...
            </p>
            <p className="mt-4 text-green-600 text-sm opacity-80 animate-pulse">
              Injecting caffeine... Establishing connection... Bypassing firewalls...
            </p>
          </div>
        )}
        {status === 'success' && (
          <div className="animate-fade-in">
            <p className="text-green-500 animate-typing whitespace-nowrap border-r-4 border-green-500 pr-2">
              ✅ Login successful.
            </p>
            <p className="mt-4 text-green-600 text-sm opacity-90">You may now close this window. Or don’t. I'm not your boss.</p>
          </div>
        )}
        {status === 'error' && (
          <div className="animate-fade-in">
            <p className="text-red-500 animate-typing whitespace-nowrap border-r-4 border-red-500 pr-2">
              ❌ Login failed.
            </p>
            <p className="mt-4 text-red-400 text-sm opacity-80">
              Someone spilled coffee on the authentication server.
              <br />
              This window will self-destruct shortly.
            </p>
          </div>
        )}
      </div>
    </div>
  );
  
}
