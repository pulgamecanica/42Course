import { ElNotification } from "element-plus";
import axios, { AxiosError } from 'axios'
import type { AxiosResponse } from 'axios'
import { ref } from "vue";
import type { ApiErrors } from "./types";

const BACKEND_URL = import.meta.env.VITE_BACKEND_URL.replace(/\/+$/, "");
const API_PATH = '/api';
const LOGIN_PATH = '/api/auth/login';

const axiosInstance = axios.create({
  baseURL: BACKEND_URL + API_PATH,
  timeout: 3000,
  withCredentials: true,
  headers: {
    'Content-Type': 'application/json'
  }
})

const setToken = (newToken: string) => {
  localStorage.setItem('token', newToken)
}

const clearToken = () => {
  localStorage.removeItem('token')
}

const getToken = () => {
  return localStorage.getItem('token')
}

axiosInstance.interceptors.request.use(
  config => {
    if (config.headers && !config.headers['Authorization'] && getToken()) {
      config.headers['Authorization'] = `Bearer ${getToken()}`;
    }
    return config;
  },
  error => Promise.reject(error)
);

axiosInstance.interceptors.response.use(
  response => response,
  error => {
    if (error.response && [401, 403].includes(error.response.status)) {
      // Auto logout if 401 Unauthorized or 403 Forbidden response returned from api
      clearToken();
      window.location.reload(); // This way router rules will be applied
      return null; // break the promise chain so no error is thrown
    }
    return handleRequestError(error)
  }
)

const login = () => window.location.href = BACKEND_URL + LOGIN_PATH;

const displayed = ref(false)
const onClose = () => displayed.value = false

// Recursively find a string error message for the notification
export const stringifyError = (error: any): string => {
  if (typeof error === "string") {
    return error
  } else if (Array.isArray(error) && error.length) {
    return stringifyError(error[0])
  } else if (typeof error === "object" && error.message) {
    return stringifyError(error.message)
  }
  console.log(`Unable to stringify error`, error)
  return ''
}

export const parseErrors = (error: any): ApiErrors => {
  // Implement when have streamlined backend error structure
  return {}
}

// Main Error notification wrapper
export const ErrorNotification = (
  message: string = 'An error occurred, return back later.',
  title: string = 'Error',
) => {
  if (!displayed.value) {
    ElNotification.error({
      title,
      message,
      dangerouslyUseHTMLString: true,
      duration: 0,
      onClose,
    });
    displayed.value = true
  }
}

export const parseAxiosError = (error: AxiosError, notification = true) => {
  if (!error.response) {
    return ''
  }
  const { data, status }: AxiosResponse = error.response
  const message = stringifyError(data)
  const title = 'Validation error:'

  console.log(`${title} ${status}`, data)
  if (notification) {
    ErrorNotification(message, title)
  }
  return message // parseError()
}

const handleRequestError = (error: AxiosError) => {
  let title: string = '';

  if (error.isAxiosError) {
    if (error.response) {
      /*
        Please do NOT edit the following line!

        handle on a use case basis - all api calls should be wrapped
          - if using async / await, use `try {}; catch() {}`
          - if using promise, use `then().catch()`

        use a mix of
          - `parseAxiosError` / `stringifyError`
          - `ErrorNotification`
      */
      throw error
    } else {
      /* handle network error

         Behaviour when server not reachable
         - restrict user actions (TBD)
      */
      title = 'Network error';
      ErrorNotification('Server not reachable, return back later.', title)
      console.log(title, error)
    }
  } else {
    // handle non-Axios error
    title = "TODO: handle this!";
    ErrorNotification('Should not be a notification, probably a bug in the server code!', title)
    console.log(error, title)
  }

  return Promise.reject(error);
}

export const useAxios = () => {
  return {
    axios: axiosInstance,
    authenticated: () => getToken() !== null,
    setToken,
    clearToken,
    login,
    getToken
  }
}