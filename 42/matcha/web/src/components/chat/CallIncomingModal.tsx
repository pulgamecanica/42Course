import { PhoneIcon, PhoneOffIcon } from 'lucide-react';

type Props = {
  callerUsername: string;
  onAccept: () => void;
  onDecline: () => void;
  callerAvatarUrl?: string;
};

export function CallIncomingModal({
  callerUsername,
  onAccept,
  onDecline,
  callerAvatarUrl,
}: Props) {
  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-black bg-opacity-60 backdrop-blur-sm">
      <div className="bg-white dark:bg-gray-900 rounded-2xl shadow-2xl p-6 w-full max-w-md text-center animate-fadeIn scale-95">
        <div className="mb-4">
          <div className="w-20 h-20 mx-auto rounded-full overflow-hidden shadow-sm">
            {callerAvatarUrl ? (
              <img
                src={callerAvatarUrl}
                alt={callerUsername}
                className="w-full h-full object-cover"
              />
            ) : (
              <div className="w-full h-full bg-gray-200 dark:bg-gray-800 flex items-center justify-center text-3xl text-gray-500">
                📞
              </div>
            )}
          </div>

          <h2 className="mt-4 text-2xl font-bold text-gray-900 dark:text-white">
            Incoming Call
          </h2>
          <p className="text-gray-600 dark:text-gray-300 mt-1">
            <span className="font-medium text-blue-600 dark:text-blue-400">
              {callerUsername}
            </span>{' '}
            is calling you.
          </p>
        </div>

        <div className="flex justify-center gap-8 mt-6">
          <button
            onClick={onDecline}
            className="bg-red-500 hover:bg-red-600 text-white rounded-full p-4 shadow-md transition"
            title="Decline Call"
          >
            <PhoneOffIcon size={28} />
          </button>

          <button
            onClick={onAccept}
            className="bg-green-500 hover:bg-green-600 text-white rounded-full p-4 shadow-md transition"
            title="Accept Call"
          >
            <PhoneIcon size={28} />
          </button>
        </div>
      </div>
    </div>
  );
}
