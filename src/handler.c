#include "handler.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>


#define SERVER_STRING "Server: Tiny-server-reasond"


static int get_line(int sock, char * buf, int size);
static void headers(int client);
static void not_found(int client);
static void unimplemented(int client);
static void serve_file(int client, const char * filename);
static void cat(int client, FILE * resource);
static Request * parse_request(int client);

void handle_request(int fd) {
  char path[200];
  struct stat st;
  
  Request * request = parse_request(fd);

  if (!request)
    return;

  sprintf(path, "htdocs%s", request->url);
  fprintf(stderr, "%s\n", path);
  if (path[strlen(path) - 1] == '/')
    strcat(path, "index.html");
  if (stat(path, &st) == -1) {
    not_found(fd);
  } else {
    if ((st.st_mode & S_IFMT) == S_IFDIR)
      strcat(path, "/index.html");
    serve_file(fd, path);
  }
  free(request);
}


int get_line(int sock, char* buf, int size) {
  int i = 0;
  char c = '\0';
  int n;
#ifdef DEBUG
  //fprintf(stderr, "%s", buf);
#endif
  while((i < size - 1) && (c != '\n')) {
    n = recv(sock, &c, 1, 0); 
    if (n > 0) {
      if (c == '\r') {
        n = recv(sock, &c, 1, MSG_PEEK);
        if ((n > 0) && (c == '\n'))
          recv(sock, &c, 1, 0); 
        else
          c = '\n';
      }   
      buf[i] = c;
      i++;
    } else
      c = '\n';
  }
  buf[i] = '\0';
  return i;
}

void headers(int client) {
  char buf[1024];

  strcpy(buf, "HTTP/1.1 200 OK\r\n");
  send(client, buf, strlen(buf), 0);
  strcpy(buf, SERVER_STRING);
  send(client, buf, strlen(buf), 0);
  sprintf(buf, "Content-Type: text/html\r\n");
  send(client, buf, strlen(buf), 0);
  strcpy(buf, "\r\n");
  send(client, buf, strlen(buf), 0);
}

void not_found(int client) {
  char buf[1024];

  sprintf(buf, "HTTP/1.1 404 NOT FOUND\r\n");
  send(client, buf, strlen(buf), 0);
  sprintf(buf, SERVER_STRING);
  send(client, buf, strlen(buf), 0);
  sprintf(buf, "Content-Type: text/html\r\n");
  send(client, buf, strlen(buf), 0);
  sprintf(buf, "\r\n");
  send(client, buf, strlen(buf), 0);
  sprintf(buf, "<html><head><title>NOT FOUND</title></head>\r\n");
  send(client, buf, strlen(buf), 0);
  sprintf(buf, "<body><p>The server could not fulfill\r\n");
  send(client, buf, strlen(buf), 0);
  sprintf(buf, "your request because the resource specified\r\n");
  send(client, buf, strlen(buf), 0);
  sprintf(buf, "is unavailable or nonexistent</p></body></html>\r\n");

  send(client, buf, strlen(buf), 0);
}

void unimplemented(int client) {
  char buf[1024];

  sprintf(buf, "HTTP/1.1 501 Method Not Implemented\r\n");
  send(client, buf, sizeof(buf), 0);
  sprintf(buf, SERVER_STRING);
  send(client, buf, sizeof(buf), 0);
  sprintf(buf, "Content-Type: text/html\r\n");
  send(client, buf, sizeof(buf), 0);
  sprintf(buf, "\r\n");
  send(client, buf, sizeof(buf), 0);
  sprintf(buf, "<html><head><title>Method Not Implemented\r\n");
  send(client, buf, sizeof(buf), 0);
  sprintf(buf, "</title></head>\r\n");
  send(client, buf, sizeof(buf), 0);
  sprintf(buf, "<body><p>HTTP request method not supported.\r\n");
  send(client, buf, sizeof(buf), 0);
  sprintf(buf, "</body></html>\r\n");
  send(client, buf, sizeof(buf), 0);
}


static Request * parse_request(int fd) {
  char buf[1024];

  Request * request = (Request *)(malloc(sizeof(Request)));
  request->query_string = 0;

  size_t i, j;
  int numchars;
  numchars = get_line(fd, buf, sizeof(buf));

  i = 0;
  j = 0;
  while  (!isspace(buf[j]) && (i < sizeof(request->method) -1)) {
    request->method[i] = buf[j];
    i++;
    j++;
  }
  request->method[i] = '\0';

  if (STRCASE_NOT_EQUAL(request->method, "GET")) {
#ifdef DEBUG
  fprintf(stderr, "Method Unimplementd\n");
#endif
    unimplemented(fd);
    return NULL;
  }

  i = 0;
  while (isspace(buf[j]) && (j < sizeof(buf)))
    j++;
  while (!isspace(buf[j]) && (i < sizeof(request->url) - 1) && (j < sizeof(buf))) {
    request->url[i] = buf[j];
    if (buf[j] == '?') {
      request->url[i] = '\0';
      request->query_string = &request->url[i+1];
    }
    i++;
    j++;
  }
  request->url[i] = '\0';

#ifdef DEBUG
  fprintf(stderr, "method: %s\n", request->method);
  fprintf(stderr, "url: %s\n", request->url);
  if (request->query_string) {
    fprintf(stderr, "query_string: %s\n", request->query_string);
  }
#endif
  while ((numchars > 0) && STR_NOT_EQUAL("\n", buf)) {
    numchars = get_line(fd, buf, sizeof(buf));
  }
  return request;
}

void serve_file(int client, const char * filename) {
  FILE * resource = NULL;

  resource = fopen(filename, "r");
  if (resource == NULL)
    not_found(client);
  else {
    headers(client);
    cat(client, resource);
  }
  fclose(resource);
}

void cat(int client, FILE * resource) {
  char buf[1024];
  fgets(buf, sizeof(buf), resource);
  while (!feof(resource)) {
    send(client, buf, strlen(buf), 0);
    fgets(buf,sizeof(buf), resource);
  }
}
