#ifndef DART_UTIL_RINGBUFFER_H_
#define DART_UTIL_RINGBUFFER_H_

typedef struct {
	int head;
	int tail;
	int size;
} ringbuffer_t;

#include "../../../chickenFlap/dart/util/error.h"

#ifdef __cplusplus
extern "C" {
#endif

ringbuffer_t rb_create(int size);
void rb_clear(ringbuffer_t* buffer);
bool rb_isempty(ringbuffer_t* buffer);
int rb_count(ringbuffer_t* buffer);
bool rb_isfull(ringbuffer_t* buffer);
bool rb_isfull_len(ringbuffer_t* buffer, int len);
int rb_put(ringbuffer_t* buffer);
int rb_put_len(ringbuffer_t* buffer, int len);
int rb_peek(ringbuffer_t* buffer);
int rb_get(ringbuffer_t* buffer);
int rb_get_len(ringbuffer_t* buffer, int len);

#ifdef __cplusplus
}
#endif

#endif
