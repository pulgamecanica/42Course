FROM debian:10.11

RUN apt-get update -yq

RUN apt-get install nano -yq
RUN apt-get install nginx -yq
RUN apt-get install openssl -yq
RUN apt-get install gettext-base -yq
RUN ln -sf /usr/share/zoneinfo/Europe/Lisbon /etc/localtime
RUN mkdir -p /etc/nginx/ssl
RUN openssl req -new -x509 -days 365 -nodes \
-out /etc/nginx/ssl/arosado-.pem \
-keyout /etc/nginx/ssl/arosado-.key \
-subj "/C=PO/ST=Lisbon/L=Lisbon/O=42Lisboa/OU=42Lisboa-/CN=arosado-.42.fr/"
RUN chmod 600 /etc/nginx/ssl/arosado-*

COPY /tools/ /tmp/
COPY /conf/default.conf.tmpl /default.conf.tmpl

ENTRYPOINT ["sh","-c","/bin/bash /tmp/run.sh"]
#ENTRYPOINT ["sh", "-c", "/bin/bash ./usr/sbin/nginx"]
