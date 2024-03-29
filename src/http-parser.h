#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "finwo/mindex.h"
#include "tidwall/buf.h"

struct http_parser_event {
  struct http_parser_message *request;
  struct http_parser_message *response;
  struct http_parser_pair *pair;
  struct buf *chunk;
  void *udata;
};

struct http_parser_message {
  int ready;
  int status;
  char *statusMessage;
  char *method;
  char *path;
  char *query;
  char *version;
  struct mindex_t *meta;
  struct buf *body;
  struct buf *buf;
  int chunksize;
  int _state;
  void (*onChunk)(struct http_parser_event*);
  void *udata;
};

struct http_parser_pair {
  struct http_parser_message *request;
  struct http_parser_message *response;
  void *udata;
  void (*onRequest)(struct http_parser_event*);
  void (*onResponse)(struct http_parser_event*);
};

// Meta management
const char * http_parser_meta_get(struct http_parser_message *subject, const char *key);
void http_parser_meta_set(struct http_parser_message *subject, const char *key, const char *value);
void http_parser_meta_del(struct http_parser_message *subject, const char *key);

// Header management
const char * http_parser_header_get(struct http_parser_message *subject, const char *key);
void http_parser_header_set(struct http_parser_message *subject ,const char *key, const char *value);
void http_parser_header_del(struct http_parser_message *subject, const char *key);

struct http_parser_pair    * http_parser_pair_init(void *udata);
struct http_parser_message * http_parser_request_init();
struct http_parser_message * http_parser_response_init();

void http_parser_request_data(struct http_parser_message *request, const struct buf *data);
void http_parser_response_data(struct http_parser_message *response, const struct buf *data);

void http_parser_pair_request_data(struct http_parser_pair *pair, const struct buf *data);
void http_parser_pair_response_data(struct http_parser_pair *pair, const struct buf *data);

void http_parser_pair_free(struct http_parser_pair *pair);
void http_parser_message_free(struct http_parser_message *subject);

const char * http_parser_status_message(int status);
struct buf * http_parser_sprint_pair_response(struct http_parser_pair *pair);
struct buf * http_parser_sprint_pair_request(struct http_parser_pair *pair);
struct buf * http_parser_sprint_response(struct http_parser_message *response);
struct buf * http_parser_sprint_request(struct http_parser_message *request);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _HTTP_PARSER_H_
