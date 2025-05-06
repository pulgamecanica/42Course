export function StatSidebar({
  visible,
  title,
  onClose,
  children,
}: {
  visible: boolean;
  title: string;
  onClose: () => void;
  children: React.ReactNode;
}) {
  return (
    <>
      {visible && (
        <div
          className="fixed z-40 inset-0 bg-black bg-opacity-50 backdrop-blur-sm"
          onClick={onClose}
        />
      )}

      <div
        className={`fixed top-0 right-0 h-screen bg-slate-900 text-white shadow-lg transform transition-transform duration-300 z-50 ${
          visible ? 'translate-x-0' : 'translate-x-full'
        } w-full sm:w-80 max-w-full`}
      >
        <div className="flex items-center justify-between p-4 border-b border-gray-700">
          <h2 className="text-lg font-bold capitalize">{title}</h2>
          <button
            onClick={onClose}
            className="text-pink-400 hover:text-pink-600 text-sm"
          >
            Close
          </button>
        </div>
        <div className="p-4 overflow-y-auto max-h-[calc(100vh-64px)]">{children}</div>
      </div>
    </>
  );
}
