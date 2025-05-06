import { showApiErrorToast } from '@/utils/showApiErrorToast';
import axios, { AxiosError, InternalAxiosRequestConfig, AxiosResponse } from 'axios';

/**
 * Create a base axios instance:
 * - baseURL: your Sinatra API URL.
 * - withCredentials: false by default, only set true if you need cross-site cookie sending.
 */
const axiosInstance = axios.create({
  baseURL: `${import.meta.env.VITE_API_BASE || 'http://localhost:9292'}`,
  timeout: 10000, // optional; sets a 10-second timeout
});

/**
 * REQUEST Interceptor:
 * - Attach bearer token if present in localStorage (or from a more secure store).
 */
axiosInstance.interceptors.request.use(
  (config: InternalAxiosRequestConfig) => {
    const token = localStorage.getItem('token');
    if (token && config.headers) {
      config.headers.Authorization = `Bearer ${token}`;
    }
    return config;
  },
  (error: AxiosError) => {
    // Request error (e.g., no network)
    return Promise.reject(error);
  }
);

/**
 * RESPONSE Interceptor:
 * - Handle errors in one place.
 * - E.g., if server returns 401/403, we can log out or refresh token.
 */
axiosInstance.interceptors.response.use(
  (response: AxiosResponse) => {
    if (response.data && typeof response.data === 'object' && 'data' in response.data) {
      return response.data.data;
    }
    return response.data;
  },
  async (error: AxiosError) => {
    if (error.response) {
      showApiErrorToast(error);
      const status = error.response.status;

      // Unauthorized or Forbidden – your Sinatra API might use 401 or 403
      // for invalid/expired tokens. Decide how to handle:
      if (status === 401) {
        // Option A: Force logout:
        if (!['/login', '/register'].includes(location.pathname)) {
          localStorage.removeItem('token');
          window.location.href = '/login'; // or set some global state
        }
        // Option B: Attempt token refresh if your API supports refresh tokens.
        // (Not shown here; you'd do a separate call to refresh, then retry.)
      }

      // For other status codes (404, 422, 500, etc.), you can handle globally or
      // throw the error to be caught locally in your component.
    }

    // If you do not handle the error, pass it along:
    return Promise.reject(error);
  }
);

export default axiosInstance;
