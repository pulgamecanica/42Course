##############
# User SetUp #
##############
su
sudo adduser arosado-
usermod -aG sudo arosado-
sudo reboot
##################
# Install Docker #
##################
sudo apt-get update
sudo apt-get install \
    ca-certificates \
    curl \
    gnupg \
    lsb-release
sudo mkdir -p /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-compose-plugin
##########################
# Install Docker Compose #
##########################
sudo apt-get update
sudo apt-get install docker-compose-plugin
sudo curl -L https://github.com/docker/compose/releases/download/v2.0.1/docker-compose-linux-x86_64 -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
docker-compose --version
#############################################
# Allow user to execute without permissions #
#############################################
## INSTEAD ADD TO DOCKER GROUP
sudo chmod 666 /var/run/docker.sock
###########################################
# [nginx] Build Docker image and check it #
###########################################
docker build --tag nginx .
docker image -ls -a
docker image history nginx
############################
# [nginx] Run docker Image #
############################
[OPT 1] docker run -d -p 443:443 --name nginx nginx
[OPT 2] docker run -it --rm -d -p 443:443 --name nginx nginx
docker container ls
docker exec nginx /etc/init.d/nginx status
docker exec nginx nginx -t
docker ps -a
#####################################
# [wordpress] Build Wordpress image #
#####################################
docker build --tag wordpress .
docker image -ls -a
docker image history wordpress
###################################
# [wordpress] Run wordpress image #
###################################
docker run -d -p  9000:9000 --name wordpress wordpress
docker container exec wordpress /etc/init.d/php7.3-fpm status
docker exec wordpress wp --info
################################
# [mariadb] Build MariaDb image#
################################
# TO RUN THE DAEMON: cd '/usr' ; /usr/bin/mysqld_safe --datadir='/var/lib/mysql'
docker build --tag mariadb .
###############################
# [mariadb] Run mariadb image #
###############################
docker run -d -p  3600:3600 --name mariadb mariadb

########################
# Sdd Hosts to machine #
########################
sudo cat << EOF >> /etc/hosts
#Inception Setup
127.0.0.1 arosado-42.fr
127.0.0.1 www.arosado-.42.fr
127.0.0.1 https:www.arosado.42.f
EOF



################
# USEFULL SHIT #
################
# Delete last ARG containers
ARG=3; docker rm $(docker ps -a | tail -$ARG | awk '{print($15)}')
# Clean images
docker image prune
# See docker run result and more stuff about the container
docker inspect container_name
# See docker compose result
docker compose convert
# Feature docker compose
SHOW ERROR WHEN ENV NOT FOUND: $(ENV:?error)
# Enter container terminal
docker exec -it mariadb /bin/bash
# See Image JSON
docker inspect image_name
# Location of 50 config
/etc/mysql/mariadb.conf.d
# Access MariaDB
# [As Root]
mysql -p
# [As User]
mysql -u $MYSQL_USER -p
SHOW DATABASES;
