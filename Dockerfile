FROM g++:11.2.0

COPY . /usr/main_filess

WORKDIR /usr/main_files

RUN g++ src/*.cpp -Iinclude -o CommandConsole

CMD [ "./CommandConsole" ]