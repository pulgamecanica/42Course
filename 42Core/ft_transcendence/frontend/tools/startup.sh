if [ "$NODE_ENV" == "development" ]; then
  echo development mode!
  npm run dev
elif [ "$NODE_ENV" == "production" ]; then
  echo production mode!
  npm run build
fi