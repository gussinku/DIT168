# docker run --rm -ti -v $PWD:/opt/sources alpine:3.7 /bin/sh
FROM alpine:3.7 as builder
MAINTAINER Jonathan Partain gusparjo@student.gu.se
RUN apk update && \
    apk --no-cache add \
    ca-certificates \
    make \
    cmake \
    g++ \
    libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted

ADD . /opt/sources
WORKDIR /opt/sources
# "dit168" comes from project($PROJECT_NAME) in CMakeLists.txt
RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && cp dit168.Sender /tmp && cp dit168.Receiver /tmp

# Deploy
FROM alpine:3.7
MAINTAINER Jonathan Partain gusparjo@student.gu.se
RUN apk update && \
    apk add libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted && \
    mkdir /opt
WORKDIR /opt
COPY --from=builder /tmp/dit168.Sender .
COPY --from=builder /tmp/dit168.Receiver .
