docker build --tag nginx .
docker run -d -p 443:443 --name nginx nginx
