#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "buffer.h"
#include "align.h"


/*! @brief The buffer data
 *
 *  This opaque struct contains the implementation details of the
 *  buffer.
 */
struct buffer_t {

  /*! @brief The current buffer, can change if we need to realloc. */
  char *begin;

  /*! @brief The pointer past the end of the currently used buf.
   * 
   *  This is also past padding which may come after the currently
   *  used part of buf. It is always aligned. */
  char *end;

  /*! \brief Size allocated for begin */
  size_t capacity;

  /*! \brief The initial memory area
   *  
   *  Users can specify an initial buffer, which is used for small
   *  objects. May reside on the stack.
   */
  char* initial;
  /*! \brief The size allocated for initial_buf */
  size_t initial_capacity;
  /*! \brief Whether initial_buf should be freed */
  bool free_initial_buffer;
};


buffer_t buffer_new_with_initial
(void *initial, size_t size, bool needs_free)
{
  buffer_t buffer = (buffer_t)
    malloc(sizeof(struct buffer_t));
  if (buffer == NULL)
    return NULL;
  buffer->initial = (char*)initial;
  buffer->initial_capacity = size;
  buffer->free_initial_buffer = needs_free;
  buffer->begin = (char*)initial;
  buffer->end = (char*)initial;
  buffer->capacity = size;
  return buffer;
}


buffer_t buffer_new(buffer_t buffer)
{
  const int size = 4096;
  void *data = malloc(size);
  if (data == NULL)
    return NULL;
  return buffer_new_with_initial(data, size, true);
}


void buffer_delete(buffer_t buffer)
{
  if (buffer->begin != buffer->initial)
    free(buffer->begin);
  if (buffer->free_initial_buffer)
    free(buffer->initial);
  free(buffer);
}


char* buffer_begin(buffer_t buffer)
{
  return buffer->begin;
}


char* buffer_end(buffer_t buffer)
{
  return buffer->end;
}


size_t buffer_size(buffer_t buffer)
{
  return buffer->end - buffer->begin;
}


size_t buffer_capacity(buffer_t buffer)
{
  return buffer->capacity;
}


void buffer_reset(buffer_t buffer)
{
  if (buffer->begin != buffer->initial) 
    free(buffer->begin);
  buffer->begin = buffer->initial;
  buffer->end = buffer->begin;
  buffer->capacity = buffer->initial_capacity;
}


char* buffer_get(buffer_t buffer)
{
  ptrdiff_t length = buffer_size(buffer);
  void *data = malloc(length);
  if (data == NULL)
    return data;
  memcpy(data, buffer->begin, length);
  buffer_reset(buffer);
  return data;
}



static
inline size_t suggested_size(size_t actual_size)
{
  return ALIGN_ROUND_UP_SIZE(actual_size * 2);
}

xon_status buffer_ensure_total(buffer_t buffer, size_t new_capacity)
{
  if (new_capacity <= buffer->capacity)
    return XON_OK;

  ptrdiff_t length = buffer_size(buffer);
  size_t size = suggested_size(new_capacity);
  void *data;
  if (buffer->begin == buffer->initial) {
    data = malloc(size);
    if (data == NULL)
      return XON_ERROR_MALLOC;
    memcpy(data, buffer->begin, length);
    free(buffer->begin);
  } else {
    data = realloc(buffer->begin, size);
    if (data == NULL)
      return XON_ERROR_MALLOC;
  }
  buffer->begin = (char*)data;
  buffer->end = buffer->begin + length;
  buffer->capacity = size;
  return XON_OK;
}


xon_status buffer_ensure_additional(buffer_t buffer, size_t extra)
{
  ptrdiff_t length = (buffer->end - buffer->begin) + extra;
  return buffer_ensure_total(buffer, length);
}


xon_status buffer_append(buffer_t buffer, void* data, size_t size)
{
  xon_status rc = buffer_ensure_additional(buffer, size);
  if (rc != XON_OK)
    return rc;
  memcpy(buffer->end, data, size);
  buffer->end += size;
  return XON_OK;
}


xon_status buffer_fill(buffer_t buffer, char value, size_t size)
{
  xon_status rc = buffer_ensure_additional(buffer, size);
  if (rc != XON_OK)
    return rc;
  memset(buffer->end, value, size);
  buffer->end += size;
  return XON_OK;
}



const buffer_interface buffer = {
  .new = buffer_new,
  .new_with_initial = buffer_new_with_initial,
  .delete = buffer_delete,
  .append = buffer_append,
  .fill = buffer_fill,
  .begin = buffer_begin,
  .end = buffer_end,
  .get = buffer_get,
  .reset = buffer_reset,
  .size = buffer_size,
  .capacity = buffer_capacity,
  .ensure_total = buffer_ensure_total,
  .ensure_additional = buffer_ensure_additional,
};


