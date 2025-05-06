import { Loader2 } from 'lucide-react';
import { motion } from 'framer-motion';

export default function LoadingScreen() {
  return (
    <div className="w-screen h-screen flex items-center justify-center bg-gray-100 dark:bg-gray-900 text-white">
      <motion.div
        animate={{ rotate: 360 }}
        transition={{ repeat: Infinity, duration: 1, ease: "linear" }}
      >
        <Loader2 className="w-12 h-12 animate-spin text-orange-400" />
      </motion.div>
      <p className="ml-4 text-lg font-medium">Checking authentication...</p>
    </div>
  );
}
