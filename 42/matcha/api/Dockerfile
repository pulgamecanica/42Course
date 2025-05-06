FROM ruby:3.2

WORKDIR /app
COPY . /app

RUN apt-get update -qq && apt-get install -y build-essential libpq-dev
RUN gem install bundler
RUN bundle install

EXPOSE 9292

CMD ["bundle", "exec", "rackup", "--server", "puma", "--host", "0.0.0.0", "--port", "9292"]
