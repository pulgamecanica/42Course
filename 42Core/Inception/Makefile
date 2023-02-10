NAME := Inception
UNAME = $(uname -a)
CONTAINERS = nginx wordpress mariadb

#nginx: docker build [--tag image-name] .

$(NAME): up

all: $(NAME)

up:
	docker-compose -f srcs/docker-compose.yml up --build -d
	bash srcs/requirements/tools/hosts.sh create

down:
	docker-compose -f srcs/docker-compose.yml down
	bash srcs/requirements/tools/hosts.sh delete

clean: down
	echo "Cleaning Docker Volumes"
	sudo rm -rf /home/pulga/data/*

fclean: clean
	docker system prune -f
	@-docker rmi -f $(docker images -qa)
	@-docker volume rm db wp

re: fclean up
