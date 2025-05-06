import { Location } from '@/types/location';
import { LocationForm } from '@/components/forms/LocationForm';

type Props = {
  initialLocation: Location | null;
  onClose: () => void;
};

export function LocationEditorModal({ initialLocation, onClose }: Props) {
  return (
    <div className="fixed inset-0 bg-black bg-opacity-50 z-50 flex items-center justify-center px-4">
      <div className="bg-white dark:bg-gray-800 rounded-lg p-6 w-full max-w-lg shadow-lg">
        <div className="flex items-center justify-between mb-4">
          <h2 className="text-lg font-bold text-gray-800 dark:text-white text-center w-full">📍 Set Your Location</h2>
          <button
            onClick={onClose}
            className="text-sm text-gray-500 hover:text-gray-700 dark:text-gray-400 dark:hover:text-gray-200"
          >
            ✕
          </button>
        </div>

        <LocationForm
          initialLocation={initialLocation}
          onSuccess={() => {
            onClose();
          }}
        />
      </div>
    </div>
  );
}
