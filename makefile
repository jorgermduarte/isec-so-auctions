clean:
	$(RM) client
	$(RM) server

all: server client


server:
	

client:
	gcc -o client frontend/main.c frontend/commands_handler.c frontend/commands.c 