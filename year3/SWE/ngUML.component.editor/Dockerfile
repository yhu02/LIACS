# syntax=docker/dockerfile:1

FROM node:16.14.2

ENV NODE_ENV=development
WORKDIR /app
ENV PATH /app/node_modules/.bin:$PATH

# install app dependencies
COPY ./package.json ./
COPY ./yarn.lock ./
RUN yarn install

# start app
CMD ["yarn", "dev", "--host"]

