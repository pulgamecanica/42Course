#### ######## #### #### ######## #### #### ######## #### #### ######## #### #### ######## ####

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	SED = sed -i ""
else
	SED	= sed -i
endif

all: clear message info

up:
	@docker-compose up --build;

up_detached:
	@docker-compose up -d --build;

down:
	@echo "Down!";
	@docker-compose down

clean: volume_down
	@echo "Clean!";
	@docker-compose down
	@docker volume rm $$(docker volume ls -qf "dangling="true"") || true
	@rm -rf ./backend/node_modules
	@rm -rf ./backend/dist
	@rm -rf ./backend/client
	@rm -rf ./frontend/node_modules
	@rm -rf ./frontend/dist

fclean:	clean
	@echo "FClean!";
	@docker system prune -f -a --volumes

re:		fclean up

#### ######## #### #### ######## #### #### ######## #### #### ######## #### #### ######## ####

prod: prod_env up
dev:  dev_env up

prod_env:
	@if [ -f .env ]; then \
		$(SED) 's/NODE_ENV=.*/NODE_ENV=production/g' .env; \
	else \
		printf "\n\033[3;31m !! You need to add the .env variable !! \e[0m \n\n"; \
	fi

dev_env:
	@if [ -f .env ]; then \
		$(SED) 's/NODE_ENV=.*/NODE_ENV=development/g' .env; \
	else \
		printf "\n\033[3;31m !! You need to add the .env variable !! \e[0m \n\n"; \
	fi

#### ######## #### #### ######## #### #### ######## #### #### ######## #### #### ######## ####

volume_down:
	@rm -rf ./volumes

#### ######## #### #### ######## #### #### ######## #### #### ######## #### #### ######## ####

clear:
	@clear

message:
	@printf "\n"
	@printf "\033[1;36m	!! ft_transcendence @42Lisboa !! \e[0m \n"
	@printf "\033[1;35m		!! ft_transcendence @42Lisboa !! \e[0m \n"
	@printf "\033[1;31m			!! ft_transcendence @42Lisboa !! \e[0m \n"
	@printf "\033[1;38m				!! ft_transcendence @42Lisboa !! \e[0m \n"
	@printf "\n"

info:
	@printf "\n  \033[1;32mFor Production use 'make prod'.\e[0m";
	@printf "\n  \033[1;31mFor Development use 'make dev'. \e[0m \n";
	@printf "\n  \033[1;36m'make down' to docker-compose down.\e[0m";
	@printf "\n  \033[1;36m'make clean' to docker-compose down + remove volumes.\e[0m";
	@printf "\n  \033[1;36m'make fclean' to docker-compose down + docker system prune -a --volumes.\e[0m\n";
	@printf "\n  \033[4;29mTo delete volumes locally use:\e[0m 'make volume_down' \n\n\n";

#### ######## #### #### ######## #### #### ######## #### #### ######## #### #### ######## ####
