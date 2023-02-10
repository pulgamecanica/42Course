# ft_transcendence

No more C! No more C++! This project is about doing something you’ve never done before.\
Remind yourself the beginning of your journey in computer science. Look at you now. Time to shine!

## Compilation

### dotenv frontend

You need to create an `.env` file in the `./frontend` folder with ```VITE_APP_BACKEND=http://localhost:3000``` (you can replace the port with the port you are using for your API).

### dotenv

You need to create an `.env` file in the `.` folder next to `.env.example`.

To get the INTRA_CLIENT_ID and INTRA_CLIENT_SECRET you need to go to:\
`intra > profile > settings > api` and register a new app.

The name can be anything, but the `Redirect URI` needs to be `http://localhost:3000/api/auth/callback_intra`.

After the app is created, you can fill the INTRA_CLIENT_ID and INTRA_CLIENT_SECRET.

The SESSION_SECRET, JWT_SECRET, POSTGRES_USER and POSTGRES_PASSWORD can be anything you want.\
And the NODE_ENV and DEBUG should be one of those two options.

```
############################### NestJS Setup ################################

INTRA_CLIENT_ID=
INTRA_CLIENT_SECRET=
INTRA_REDIRECT_URI=http://localhost:3000/api/auth/callback_intra

SESSION_SECRET=
JWT_SECRET=
SALT=

NESTJS_HOST_PORT=3000
FRONTEND_URL=http://localhost:8080

############################### VueJS Setup ################################

VUEJS_HOST_PORT=8080
VITE_BACKEND_URL=http://localhost:3000
VITE_IMAGE_URL=https://api.imgbb.com/1/upload
VITE_IMAGE_KEY=

############################### Postgres Setup ##############################

POSTGRES_USER=
POSTGRES_PASSWORD=
POSTGRES_DB=transcendence

POSTGRES_HOST=database-docker
POSTGRES_HOST_PORT=5432

############################### Adminer Setup ###############################

ADMINER_HOST_PORT=8081

############################### General Setup ###############################

NODE_ENV=production|development
DEBUG=true|false

#############################################################################
#                                                                           #
#                               General Info                                #
#                                                                           #
#  $NODE_ENV = development | production                                     #
#                                                                           #
#    It changes:                                                            #
#      - How NestJS starts ('npm run start:dev' or 'npm run start:prod').   #
#      - How VueJS starts ('npm run dev' or 'npm run build')                #
#                                                                           #
#                                                                           #
#  $DEBUG = true | false                                                    #
#                                                                           #
#    It changes:                                                            #
#      - On NestJS it changes 'logging' on the typeorm config.              #
#        It means that it will log changes to the database.                 #
#                                                                           #
#############################################################################
```

### Docker-Compose

Order of container startup:
1. Compodoc - backend-documentation-docker
1. PostgreSQL - database-docker
2. Adminer - adminer-docker
3. NestJS - backend-docker
4. VueJS - frontend-docker

Ports:
- database-docker: http://localhost:5432
- backend-documentation-docker: http://localhost:8082
- adminer-docker: http://localhost:8081
- backend-docker: http://localhost:3000
- frontend-docker: http://localhost:8080

Volumes:
- database-docker:
	- Volume is created to persist data, to keep the database even if it's down.
- backend-docker:
	- Volume `/usr/src/app` is created so you can have access to the files in the container, in your
 	local machine. Meaning every change inside of it will affect the container, usde for development.
	- Volume `/usr/src/app/client` is created so you can use the built files from VueJS (Which will be
	inside `./volumes/build` (locally) inside the NestJS container, so that NestJS can create a Static Web
	Server out of it. (VueJS Production doesn't have a Web Server. NestJS will serve as one).
- frontend-docker:
	- Volume `/usr/src/app` is created so you can have access to the files in the container, in your
 	local machine. Meaning every change inside of it will affect the container, used for development.
	- Volume `/usr/src/app/dist` creates a folder `./volumes/build` if not already created locally.
	And it's filled with the `/dist/` (the files built during production of VueJS. Not for development!).

It also sets the Environment Variables of corresponding containers through the .env.

### Makefile

This Makefile has most options you need for development and production.

- make: Shows all options
- make dev: Changes your .env $NODE_ENV to development and does `docker-compose up --build`
- make prod: Changes your .env $NODE_ENV to production and does `docker-compose up --build`
- make down: Does `docker-compose down`
- make clean: Does `docker-compose down`, deletes `/dist` `/node_modules` `/client` `/documentation`
locally and removes volumes from the docker
- make fclean: Does `make clean` and `docker system prune -a --volumes`
- make volume_up: Installs the node_modules packages locally so you don't have VSCode errors.
- make volume_down: Removes the `/volumes` folder. It deletes the postgres data it previously
persisted.


Note: during the project presentation the Makefile will not be used, instead `docker-compose up --build`\
will due to the subject restrictions.

## Git

.gitignore ignores
- .env
- /dist
- /node_modules
- /documentation
- /backend/client
- /volumes
