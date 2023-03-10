clean:
	$(RM) frontend
	$(RM) backend

all: backend frontend

backend:
	gcc -o backend -g -pthread apps/backend/main.c apps/backend/promotions.c apps/backend/backend.c apps/backend/commands/initializer.c apps/backend/commands/commands.c shared/commands/commands_handler.c apps/backend/config/config.c  shared/helpers/helpers.c apps/backend/notifier.c  users_lib.o

frontend:
	gcc -o frontend -g -pthread apps/frontend/main.c apps/frontend/frontend.c apps/frontend/notifier.c apps/frontend/commands/initializer.c apps/frontend/commands/commands.c  shared/commands/commands_handler.c

