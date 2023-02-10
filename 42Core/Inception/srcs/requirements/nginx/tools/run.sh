mkdir -p /usr/share/nginx/logs

CONFIG='/etc/nginx/conf.d/default.conf'

if [ -e $CONFIG ]; then
	  echo "Nginx config already created"
else

    cat /default.conf.tmpl | envsubst '$DOMAIN_NAME $WORDPRESS_PORT' > $CONFIG
fi

nginx -g "daemon off;"
