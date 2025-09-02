DOCKER_CONTAINER_NAME = web

up:
	docker compose up -d

build:
	docker compose up --build -d

down:
	docker compose down

logs:
	docker compose logs -f

run:
	docker compose run \
    -it \
    --remove-orphans \
    ${DOCKER_CONTAINER_NAME} \
    /bin/bash

docs:
	docker compose run \
		-it \
		--remove-orphans \
		${DOCKER_CONTAINER_NAME} \
		rake doc:export

create:
	-docker compose run \
		-it \
		--remove-orphans \
		${DOCKER_CONTAINER_NAME} \
		rake db:create

migrate:
	-docker compose run \
		-it \
		--remove-orphans \
		${DOCKER_CONTAINER_NAME} \
		rake db:migrate

seed: create migrate
	-docker compose run \
		-it \
		--remove-orphans \
		${DOCKER_CONTAINER_NAME} \
		rake db:seed

drop:
	-docker compose run \
		-it \
		--remove-orphans \
		${DOCKER_CONTAINER_NAME} \
		rake db:drop

test:
	-docker compose run \
		-it \
		--remove-orphans \
		${DOCKER_CONTAINER_NAME} \
		bundle exec rspec $(filter-out $@,$(MAKECMDGOALS))

console:
	docker compose run \
		${DOCKER_CONTAINER_NAME} \
		irb -r ./app

re: down build re_db

re_db: drop create migrate

.PHONY: docs test migrate
