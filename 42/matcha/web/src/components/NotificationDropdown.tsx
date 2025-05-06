import { useState, useRef, useEffect } from 'react';
import { Bell, MailOpen, MailPlus } from 'lucide-react';
import { useNotifications } from '@/hooks/useNotifications';
import clsx from 'clsx';
import { useNavigate } from 'react-router-dom';

export function NotificationDropdown() {
  const [open, setOpen] = useState(false);
  const ref = useRef<HTMLDivElement>(null);
  const {
    notifications,
    markAllAsRead,
    hasUnread,
  } = useNotifications();
  
  const navigate = useNavigate();
  // Close dropdown on outside click
  useEffect(() => {
    function handleClickOutside(event: MouseEvent) {
      if (ref.current && !ref.current.contains(event.target as Node)) {
        setOpen(false);
      }
    }
    document.addEventListener('mousedown', handleClickOutside);
    return () => document.removeEventListener('mousedown', handleClickOutside);
  }, []);

  useEffect(() => {
    if (open) markAllAsRead();
  }, [open, markAllAsRead]);

  if (['/login', '/register', '/setup'].includes(location.pathname)) return;

  return (
    <div className="absolute right-0 p-4" ref={ref}>
      <button
        onClick={() => setOpen(prev => !prev)}
        className="relative p-3 rounded-full text-white hover:dark:bg-gray-700 dark:bg-gray-800 bg-gray-200 hover:bg-gray-300"
      >
        <Bell className="h-5 w-5 text-gray-800 dark:text-gray-100" />
        {hasUnread && (
          <span className="absolute top-0 right-0 h-2 w-2 bg-red-500 rounded-full border-2 border-gray-900" />
        )}
      </button>

      {open && (
        <div className="absolute right-0 m-2 w-80 bg-gray-100 dark:bg-gray-800 border border-gray-300 dark:border-gray-700 rounded-md shadow-lg z-50 text-sm">
          <div className="px-4 py-2 font-semibold border-b border-gray-300 dark:border-gray-700">Notifications</div>
          <ul className="max-h-80 overflow-y-auto divide-y divide-gray-300 dark:divide-gray-700">
            {notifications.length === 0 && (
              <li className="px-4 py-3 text-gray-400">No notifications yet</li>
            )}
            {notifications.map((n) => (
              <li
                key={n.id}
                onClick={() => {
                  switch (n.type) {
                    case 'like':
                    case 'unlike':
                    case 'match':
                    case 'connection':
                    case 'view':
                    case 'other':
                      navigate(`/profile/${n.from_username}`);
                      break;
                    case 'message':
                    case 'date':
                        navigate(`/conversations?user=${n.from_username}`);
                      break;
                    default:
                      break;
                  }
                }}
                className={clsx(
                  'px-4 py-3 hover:bg-gray-300 hover:dark:bg-gray-700 transition cursor-pointer',
                  n.read !== 't' && 'bg-gray-300 dark:bg-gray-700'
                )}
              >
                <div className="flex items-center justify-between">
                  <div>
                    <div className="font-medium">{n.message}</div>
                    <div className="text-xs text-gray-400">
                      {n.from_username || 'System'} · {new Date(n.created_at).toLocaleString()}
                    </div>
                  </div>
                  {n.read === 't'
                    ? <MailOpen className='h-5 w-5'/>
                    : <MailPlus className='h-5 w-5'/>
                  }
                </div>
              </li>
            ))}
          </ul>
        </div>
      )}
    </div>
  );
}
