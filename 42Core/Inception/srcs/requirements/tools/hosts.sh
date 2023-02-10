#!/bin/bash

function delete_host {
	sudo sed -i '/42.fr/d' /etc/hosts
}

function create_host {
	delete_host
	NGINX_IP=`docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' nginx`

	sudo sed -i '1i'$NGINX_IP' arosado-.42.fr' /etc/hosts
	sudo sed -i '1i'$NGINX_IP' www.arosado-.42.fr' /etc/hosts
	sudo sed -i '1i'$NGINX_IP' https://wwww.arosado-.42.fr' /etc/hosts
	sudo sed -i '1i # Inception setup ... arosado-.42.fr ' /etc/hosts
}


if [ $# -ne 1 ]; then
	echo "Usage: $0 [create|delete]"
elif [ $1 == "create" ]; then
	create_host
	echo "Created Hosts"
elif [ $1 == "delete" ]; then
	delete_host
	echo "Deleted Hosts"
else
	echo "Usage: $0 [create|delete]"
fi
