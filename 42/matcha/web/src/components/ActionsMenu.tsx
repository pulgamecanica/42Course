import { useState } from 'react';
import { LogOut, Home, Menu, User } from 'lucide-react';
import { useNavigate, useLocation } from 'react-router-dom';
import { useAuth } from '@/hooks/useAuth';
import clsx from 'clsx';

export function ActionsMenu() {
  const [expanded, setExpanded] = useState(false);
  const navigate = useNavigate();
  const location = useLocation();
  const { logout } = useAuth();

  if (['/login', '/register', '/setup'].includes(location.pathname)) return null;

  const actions = [
    {
      icon: <Home className="w-5 h-5" />,
      label: 'Menu',
      onClick: () => navigate('/'),
      color: 'bg-pink-500 hover:bg-pink-600',
    },
    {
      icon: <User className="w-5 h-5" />,
      label: 'Profile',
      onClick: () => navigate('/profile'),
      color: 'bg-blue-400 hover:bg-blue-500',
    },
    {
      icon: <LogOut className="w-5 h-5" />,
      label: 'Log Out',
      onClick: logout,
      color: 'bg-red-500 hover:bg-red-600',
    },
  ];

  const toggleExpand = () => {
    setExpanded(!expanded);
  }

  return (
    <div
      className="fixed bottom-4 left-4 z-50 group"
      onClick={toggleExpand}
      onMouseEnter={() => setExpanded(true)}
      onMouseLeave={() => setExpanded(false)}
    >
      <div className="relative flex flex-col items-start space-y-2">
        {actions.map((action, idx) => (
          <button
            key={idx}
            onClick={action.onClick}
            className={clsx(
              'flex items-center space-x-2 text-white px-4 py-2 rounded-full shadow-md transition-all duration-300 ease-in-out transform',
              expanded ? 'opacity-100 scale-100' : 'opacity-0 scale-0 h-0',
              action.color
            )}
            style={{
              transitionDelay: `${(actions.length - idx) * 50}ms`,
            }}
          >
            {action.icon}
            <span className={clsx('whitespace-nowrap', !expanded && 'hidden')}>{action.label}</span>
          </button>
        ))}

        {/* Main FAB */}
        <div
          className={clsx(
            'flex items-center justify-center w-10 h-10 rounded-full dark:bg-gray-600 dark:text-white bg-gray-300 text-black cursor-pointer shadow-lg transition-transform',
            expanded ? 'rotate-90' : ''
          )}
        >
           <Menu className="w-6 h-6" />
        </div>
      </div>
    </div>
  );
}
