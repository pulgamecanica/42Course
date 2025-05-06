import { Link } from 'react-router-dom';

export const NotFoundPage = () => {
    return (
        <div className="flex flex-col items-center justify-center h-screen text-center px-4">
            <h1 className="text-7xl font-extrabold text-red-500">404</h1>
            <h2 className="text-2xl font-semibold text-gray-800 dark:text-gray-300 mt-4">
                Page Not Found
            </h2>
            <p className="text-gray-600 dark:text-gray-100 mt-2">
                Sorry, the page you're looking for doesn't exist or has been moved.
            </p>
            <Link
                to="/"
                className="mt-6 inline-block bg-blue-600 text-white px-6 py-2 rounded-xl shadow hover:bg-blue-700 transition-all duration-200"
            >
                Go back home
            </Link>
        </div>
    );
};