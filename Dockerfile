FROM php/5.6-alpine

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app
COPY . /usr/src/app

CMD ["php", "-S", "0.0.0.0:8080"]
