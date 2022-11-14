clean:
	$(RM) client
	$(RM) server

all: server client

server:
	gcc -o server -pthread backend/main.c backend/backend.c commands/commands.c config/config.c helpers/helpers.c

client:
	gcc -o client frontend/main.c frontend/commands_handler.c frontend/commands.c 

