#include "../../../chickenFlap/dart/util/ringbuffer.h"

ringbuffer_t rb_create(int size) {
	ringbuffer_t rb;
	rb.head = 0;
	rb.tail = 0;
	rb.size = size;
	return rb;
}

void rb_clear(ringbuffer_t* buffer) {
	DART_NOT_NULL(buffer, DART_ERROR_UTIL);
	buffer->head = 0;
	buffer->tail = 0;
}

bool rb_isempty(ringbuffer_t* buffer) {
	DART_NOT_NULL_RETURN(buffer, DART_ERROR_UTIL, false);
	return buffer->head == buffer->tail;
}

int rb_count(ringbuffer_t* buffer) {
	DART_NOT_NULL_RETURN(buffer, DART_ERROR_UTIL, 0);
	if (buffer->head < buffer->tail)
		return buffer->size - (buffer->tail - buffer->head);
	return buffer->head - buffer->tail;
}

bool rb_isfull(ringbuffer_t* buffer) {
	return rb_isfull_len(buffer, 1);
}

bool rb_isfull_len(ringbuffer_t* buffer, int len) {
	DART_NOT_NULL_RETURN(buffer, DART_ERROR_UTIL, false);
	return (buffer->size - rb_count(buffer)) < len;
}

int rb_put(ringbuffer_t* buffer) {
	return rb_put_len(buffer, 1);
}

int rb_put_len(ringbuffer_t* buffer, int len) {
	if (rb_isfull_len(buffer, len))
		return 0;
	int i = buffer->head;
	buffer->head = (buffer->head + len) % buffer->size;
	return i;
}

int rb_peek(ringbuffer_t* buffer) {
	if (rb_isempty(buffer))
		return 0;

	return buffer->tail;
}

int rb_get(ringbuffer_t* buffer) {
	return rb_get_len(buffer, 1);
}

int rb_get_len(ringbuffer_t* buffer, int len) {
	if (rb_count(buffer) < len)
		return 0;

	int i = buffer->tail;
	buffer->tail = (buffer->tail + len) % buffer->size;
	return i;
}
