FROM ubuntu

WORKDIR /usr/src/

RUN apt-get update && apt-get upgrade -y && \
	apt-get install -yq \
	gcc \
	g++ \
	make \
	lsof \
  libcap2-bin

ADD . .

RUN make
RUN setcap cap_sys_resource=pe hide-cmd

