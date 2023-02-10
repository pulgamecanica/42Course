if [ "$NODE_ENV" == "development" ]; then
  echo development mode!
  npm run start:dev
elif [ "$NODE_ENV" == "production" ]; then
  echo production mode!
  npm run build
  npm run start:prod
fi