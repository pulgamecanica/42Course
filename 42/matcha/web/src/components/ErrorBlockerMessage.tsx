import React from 'react';

type ErrorBlockerMessageProps = {
    message: string;
    locationMessage: string;
};

export const ErrorBlockerMessage: React.FC<ErrorBlockerMessageProps> = ({ message, locationMessage }) => {
    return (
        <div className="flex justify-center items-center min-h-screen">
            <div className="bg-white dark:bg-gray-900 border p-6 rounded-lg shadow-md text-center">
                <p className="text-lg ">{message}</p>
                <button
                    className="mt-4 px-4 py-2 bg-blue-600 text-white rounded"
                    onClick={() => window.location.href = '/login'}
                >
                   {locationMessage}
                </button>
            </div>
        </div>
    );
};
