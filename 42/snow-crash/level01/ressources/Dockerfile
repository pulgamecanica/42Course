FROM debian
RUN apt-get update -y && apt-get install john -y
COPY passwd /lefichier
RUN john /lefichier
ENTRYPOINT [ "john", "/lefichier", "--show" ]
# ENTRYPOINT [ "tail", "-f", "/dev/null" ]