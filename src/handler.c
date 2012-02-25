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

void headers(int client) {
  fputs("HTTP/1.1 200 OK\r\n", stdout);
  fputs(SERVER_STRING, stdout);
  fputs("Content-Type: text/html\r\n", stdout);
  fputs("\r\n", stdout);
}

void not_found(int client) {
  fputs("HTTP/1.1 404 NOT FOUND\r\n", stdout);
  fputs(SERVER_STRING, stdout);
  fputs("Content-Type: text/html\r\n\r\n", stdout);
  fputs("<html><head><title>NOT FOUND</title></head>\r\n", stdout);
  fputs("<body><p>The server could not fulfill\r\n", stdout);
  fputs("your request because the resource specified\r\n", stdout);
  fputs("is unavailable or nonexistent</p></body></html>\r\n", stdout);
}

void unimplemented(int client) {
  fputs("HTTP/1.1 501 Method Not Implemented\r\n", stdout);
  fputs(SERVER_STRING, stdout);
  fputs("Content-Type: text/html\r\n\r\n", stdout);
  fputs("<html><head><title>Method Not Implemented\r\n", stdout);
  fputs("</title></head>\r\n", stdout);
  fputs("<body><p>HTTP request method not supported.\r\n", stdout);
  fputs("</body></html>\r\n", stdout);
}


static Request * parse_request(int fd) {
  char buf[128];
  int i;

  Request * request = (Request *)(malloc(sizeof(Request)));
  request->query_string = 0;

  fscanf(stdin, "%s", request->method);
  fscanf(stdin, "%s", request->url);
  fscanf(stdin, "%s", request->protocol);
  fscanf(stdin, "%s", buf);
  fscanf(stdin, "%s", request->host);
  fscanf(stdin, "%s", buf);
  fscanf(stdin, "%s", request->connection);
  fscanf(stdin, "%s", buf);
  fscanf(stdin, "%s", request->accept);
  fscanf(stdin, "%s", buf);
  fscanf(stdin, "%[^\r]", request->agent);
  fscanf(stdin, "%s", buf);
  fscanf(stdin, "%s", request->encoding);
  

#ifdef DEBUG
  fprintf(stderr, "method: %s\n", request->method);
  fprintf(stderr, "url: %s\n", request->url);
  fprintf(stderr, "protocol: %s\n", request->protocol);
  fprintf(stderr, "host: %s\n", request->host);
  fprintf(stderr, "connection: %s\n", request->connection);
  fprintf(stderr, "accept: %s\n", request->accept);
  fprintf(stderr, "agent: %s\n", request->agent);
  fprintf(stderr, "encoding: %s\n", request->encoding);
  if (request->query_string) {
    fprintf(stderr, "query_string: %s\n", request->query_string);
  }
#endif
  
  i = 0; 
  int len = strlen(request->url);
  while ((i < len) && (request->url[i] != '?' ))
    i++;
  if (i != len) {
    request->url[i] = '\0';
    request->query_string = &request->url[i+1];
  }

  if (STRCASE_NOT_EQUAL(request->method, "GET")) {
#ifdef DEBUG
  fprintf(stderr, "Method Unimplementd\n");
#endif
    unimplemented(fd);
    return NULL;
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
    fputs(buf, stdout);
    fgets(buf,sizeof(buf), resource);
  }
}
