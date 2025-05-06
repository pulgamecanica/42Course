type Props = {
  onClose: () => void;
  onSubmit: (e: React.FormEvent) => void;
  reason: string;
  setReason: (val: string) => void;
  description: string;
  setDescription: (val: string) => void;
  loading: boolean;
  clearForm: () => void;
  username: string;
};

export function ReportUserModal({
  onClose,
  onSubmit,
  reason,
  setReason,
  description,
  setDescription,
  loading,
}: Props) {
  return (
    <div className="fixed inset-0 bg-black bg-opacity-50 z-50 flex items-center justify-center px-4">
      <div className="bg-white dark:bg-gray-800 rounded-lg p-6 w-full max-w-lg shadow-lg relative">
        <form onSubmit={onSubmit} className="space-y-4">
          {/* Reason */}
          <div>
            <label className="block mb-1 text-sm font-medium text-gray-700 dark:text-gray-300">
              Reason
            </label>
            <select
              required
              value={reason}
              onChange={(e) => setReason(e.target.value)}
              className="w-full px-3 py-2 border rounded-md dark:bg-gray-700 dark:text-white"
            >
              <option value="" disabled>Select a reason</option>
              <option value="fake_account">Fake Account</option>
              <option value="scam">Scam</option>
              <option value="sexual_harassment">Sexual Harassment</option>
            </select>
          </div>

          {/* Description */}
          <div>
            <label className="block mb-1 text-sm font-medium text-gray-700 dark:text-gray-300">
              Description
            </label>
            <textarea
              required
              value={description}
              onChange={(e) => setDescription(e.target.value)}
              className="w-full px-3 py-2 border rounded-md dark:bg-gray-700 dark:text-white"
              rows={3}
            />
          </div>

          {/* Buttons */}
          <div className="flex justify-end gap-2">
            <button
              type="button"
              onClick={onClose}
              className="px-4 py-2 border rounded-md bg-white text-gray-700"
            >
              Cancel
            </button>
            <button
              type="submit"
              disabled={loading}
              className="px-4 py-2 rounded-md bg-red-600 text-white disabled:opacity-50"
            >
              {loading ? 'Reporting...' : 'Report'}
            </button>
          </div>
        </form>
      </div>
    </div>
  );
}
