clean:
	$(RM) frontend
	$(RM) backend

all: backend frontend

backend:
	gcc -o backend -pthread apps/backend/main.c apps/backend/backend.c apps/backend/commands/initializer.c apps/backend/commands/commands.c shared/commands/commands_handler.c apps/backend/config/config.c  shared/helpers/helpers.c users_lib.o

frontend:
	gcc -o frontend apps/frontend/main.c apps/frontend/commands/initializer.c apps/frontend/commands/commands.c  shared/commands/commands_handler.c

