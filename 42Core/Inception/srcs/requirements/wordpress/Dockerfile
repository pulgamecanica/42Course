FROM debian:10.11

RUN apt-get update -yq

RUN apt-get install vim -yq
RUN apt-get install curl -yq
RUN apt-get install wget -yq
RUN apt-get install php7.3-cli php7.3-fpm php7.3-mysql php7.3-json php7.3-opcache \
                    php7.3-mbstring php7.3-xml php7.3-cgi php-pear php7.3-zip \
                    php-net-socket php7.3-common php7.3-gd php7.3-curl \
                    php-xml-util php7.3-gettext php7.3-bcmath -yq

RUN ln -sf /usr/share/zoneinfo/Europe/Lisbon /etc/localtime

COPY /tools/ /tmp/

RUN mkdir /var/www
RUN mkdir /var/www/html
WORKDIR /var/www/html
RUN wget https://wordpress.org/latest.tar.gz
RUN tar -xvzf latest.tar.gz
RUN rm latest.tar.gz
WORKDIR /var/www/html/wordpress
COPY /conf/ .
RUN chown -R www-data:www-data /var/www/html/wordpress

COPY /conf/www.conf /etc/php/7.3/fpm/pool.d/www.conf

# Wordpress Instalation
WORKDIR /tmp/
RUN curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar
RUN chmod +x wp-cli.phar
RUN mv wp-cli.phar /usr/local/bin/wp

ENTRYPOINT ["sh","-c", "/bin/bash /tmp/run.sh;"]
