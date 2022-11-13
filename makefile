clean:
	$(RM)  frontend

all:

backend:

client: #frontend
	gcc -o client ./frontend/main.c ./frontend/commands_handler.c ./frontend/commands.c -I.