import { deleteDate } from '@/api/ScheduledDate';
import { ScheduledDate } from '@/types/scheduledDate';
import { X } from 'lucide-react';
import { useEffect, useRef } from 'react';

type Props = {
  dates: ScheduledDate[];
  onClose: () => void;
};

export function ScheduledDatesModal({ dates, onClose }: Props) {
  const modalRef = useRef<HTMLDivElement>(null);
  const handleDelete = async (id: number) => {
    await deleteDate(id);
  };

  useEffect(() => {
    function handleClickOutside(event: MouseEvent) {
      if (modalRef.current && !modalRef.current.contains(event.target as Node)) {
        onClose();
      }
    }

    function handleEscape(event: KeyboardEvent) {
      if (event.key === 'Escape') {
        onClose();
      }
    }

    document.addEventListener('mousedown', handleClickOutside);
    document.addEventListener('keydown', handleEscape);

    return () => {
      document.removeEventListener('mousedown', handleClickOutside);
      document.removeEventListener('keydown', handleEscape);
    };
  }, [onClose]);
  console.log(dates)


  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-black bg-opacity-60">
      <div ref={modalRef} className="bg-gray-800 md:h-auto h-full dark:bg-gray-900 p-6 rounded-lg w-full max-w-md shadow-lg relative flex flex-col">
        {/* Close button */}
        <button
          onClick={onClose}
          className="absolute top-4 right-4 text-gray-400 hover:text-gray-200"
        >
          <X className="w-6 h-6" />
        </button>

        <h2 className="text-xl font-semibold text-center text-gray-300 mb-6">📅 My Scheduled Dates</h2>

        {/* Scrollable date list */}
        <div className="flex-1 overflow-y-auto md:max-h-80">
          {dates.length === 0 ? (
            <p className="text-gray-500 text-center">You have no scheduled dates yet.</p>
          ) : (
            <div className="space-y-4">
              {dates.map((date) => (
                <div
                  key={date.id}
                  className="flex items-center justify-between bg-gray-700 dark:bg-gray-800 p-4 rounded-md"
                >
                  <div className="flex-1 text-gray-300">
                    <p><strong>With:</strong> {date.partner_username || 'Unknown'}</p>
                    <p><strong>When:</strong> {new Date(date.scheduled_at).toLocaleString()}</p>
                    <p><strong>Location:</strong> {date.location}</p>
                    <p><strong>Note:</strong> {date.note}</p>
                  </div>
                  {/* Placeholder for Delete Button */}
                  <button
                    onClick={() => handleDelete(parseInt(date.id))}
                    className="text-red-500 hover:text-red-400 font-semibold"
                  >
                    ❌
                  </button>
                </div>
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}
