import { BrowserRouter, Routes, Route } from 'react-router-dom';
import { Toaster } from 'react-hot-toast';
import { AuthProvider } from '@context/AuthProvider';
import { ThemeProvider } from '@context/ThemeProvider';
import { LoginPage } from '@pages/LoginPage';
import { RegisterPage } from '@pages/RegisterPage';
import { MatchingPage } from '@pages/MatchingPage';
import { ProfilePage } from '@pages/ProfilePage';
import { Dashboard } from '@pages/DashboardPage';
import { NotFoundPage } from '@pages/NotFoundPage';
import { ProtectedRoute } from '@components/ProtectedRoute';
import { ThemeToggle } from '@components/ThemeToggle';
import { SetupProfilePage } from '@pages/SetupProfilePage';
import { EditProfilePage } from '@pages/EditProfilePage';
import { WebSocketProvider } from '@context/WebSocketProvider';
import { NotificationProvider } from '@context/NotificationProvider';
import { NotificationDropdown } from '@components/NotificationDropdown';
import { ConversationsPage } from '@pages/ConversationsPage';
import { ActionsMenu }  from '@components/ActionsMenu';
import { PublicProfilePage } from '@pages/PublicProfilePage';
import { MessagesProvider } from '@context/MessagesProvider';
import { UserMeProvider } from '@context/UserMeProvider';
import { RecoverPasswordPage } from './pages/RecoverPasswordPage';
import { NewPasswordPage } from './pages/NewPasswordPage';
import { ConfirmAccountPage } from './pages/ConfirmAccountPage';
import { GoogleOAuthProvider } from '@react-oauth/google';
import { IntraCallbackPage } from './pages/IntraCallbackPage';
import { GlobalCallHandler } from './components/GlobalCallHandler';

function App() {
  const clientId = import.meta.env.VITE_GOOGLE_CLIENT_ID || '';
  return (
    <>
    <Toaster
      position="top-center"
      toastOptions={{
        className: `
          border
          shadow-lg
          px-4 py-3 rounded-md
          text-sm font-medium
          bg-white text-gray-800 border-gray-300
          dark:bg-gray-800 dark:text-white dark:border-gray-600
        `,
        duration: 5000,
        success: {
          className: `
            bg-green-500 text-white border-green-700
            dark:bg-green-600 dark:border-green-400 dark:text-white
          `,
        },
        error: {
          className: `
            bg-red-500 text-white border-red-700
            dark:bg-gray-600 dark:border-red-400 dark:text-white
          `,
        },
      }}
    />
    <ThemeProvider>
      <div className="min-h-screen bg-gray-100 dark:bg-gray-900 text-gray-900 dark:text-gray-100">
        <div className="fixed bottom-4 right-4 z-50">
          <ThemeToggle/>
        </div>
        <GoogleOAuthProvider clientId={clientId}>
          <AuthProvider>
            <UserMeProvider>
              <WebSocketProvider>
                <MessagesProvider>
                  <BrowserRouter>
                    <NotificationProvider>
                      <NotificationDropdown />
                      <ActionsMenu />
                      <GlobalCallHandler />
                      <Routes>
                        <Route path="/login" element={<LoginPage />} />
                        <Route path="/intra/callback" element={<IntraCallbackPage />} />
                        <Route path="/register" element={<RegisterPage />} />
                        <Route path="/recover-password" element={<RecoverPasswordPage />} />
                        <Route path="/reset-password" element={<NewPasswordPage />} />
                        <Route path="/confirm" element={<ConfirmAccountPage />} />
                        <Route path="/" element={<ProtectedRoute><Dashboard/></ProtectedRoute>} />
                        <Route path="/setup" element={<ProtectedRoute><SetupProfilePage /></ProtectedRoute>} />
                        <Route path="/profile/edit" element={<ProtectedRoute><EditProfilePage /></ProtectedRoute>} />
                        <Route path="/discover" element={<ProtectedRoute><MatchingPage/></ProtectedRoute>}/>
                        <Route path="/profile" element={<ProtectedRoute><ProfilePage/></ProtectedRoute>}/>
                        <Route path="/conversations" element={<ProtectedRoute><ConversationsPage /></ProtectedRoute>} />
                        <Route path="/profile/:username" element={<ProtectedRoute><PublicProfilePage /></ProtectedRoute>} />
                        <Route path="*" element={<NotFoundPage />} />
                      </Routes>
                    </NotificationProvider>
                  </BrowserRouter>
                </MessagesProvider>
              </WebSocketProvider>
            </UserMeProvider>
          </AuthProvider>
        </GoogleOAuthProvider>
      </div>
    </ThemeProvider>
    </>
  );
}

export default App;
