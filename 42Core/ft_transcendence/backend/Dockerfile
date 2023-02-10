FROM node:19.6

# Copy startup.sh
COPY /tools/startup.sh /tmp/

# Create app directory
WORKDIR /usr/src/app

# Copy app dependencies
COPY ./package*.json ./

# You need to install latest npm, and then install the packages
RUN npm install -g npm@9.4.2
RUN npm install

# Bundle app source
COPY . .

ENTRYPOINT ["sh","-c", "/bin/bash /tmp/startup.sh;"]