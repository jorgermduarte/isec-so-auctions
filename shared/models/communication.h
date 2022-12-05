#include <sys/types.h>
#include <stdio.h>

typedef struct Request {
    char arguments[256];
} Request;


typedef struct Response {
    char results[256];
} Response;

typedef struct Message {
    pid_t pid;
    Request request;
    Response response;
} Message;