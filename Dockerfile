FROM ubuntu

WORKDIR /usr/src/

RUN apt-get update && apt-get upgrade -y && \
	apt-get install -yq \
	gcc \
	g++ \
	make

ADD . .

RUN make