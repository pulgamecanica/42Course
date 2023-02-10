FROM debian:10.11

RUN apt-get update && \
    apt-get -y install \
        gettext-base \
        mariadb-server \
	mariadb-client \
        procps \
        nano
#    rm -rf /var/lib/apt/lists/*

#RUN apt-get install nano -yq

#RUN sed -i "s/^bind-address/#bind-address/" /etc/mysql/mariadb.conf.d/50-server.cnf
#RUN sed -i "s/^#port/port/" /etc/mysql/mariadb.conf.d/50-server.cnf
# RUN sed -i "s/^#general_log/general_log/g" /etc/mysql/mariadb.conf.d/50-server.cnf

# init script and launch bash
RUN mkdir -p /var/run/mysqld \
  && chown -R mysql:mysql /var/run/mysqld

COPY ./conf/my.cnf /my.cnf.tmpl

RUN chmod 744 /my.cnf.tmpl

RUN mv /my.cnf.tmpl /etc/mysql/my.cnf

COPY ./conf/tmpl.sql /tmpl.sql

COPY ./tools/run.sh /run.sh

VOLUME [ "/sys/fs/cgroup" ]

ENTRYPOINT [ "bash", "/run.sh" ]
#CMD [ "mysqld_safe" ]
