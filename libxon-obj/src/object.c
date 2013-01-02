#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "xon/object.h"
#include "macros.h"

/*******************************************************
 *
 * Initialization / Finalization
 *
 *******************************************************/

char magic[2*ALIGN_BYTES];


CONSTRUCTOR
void init() 
{
  memset(magic, 0, 2*ALIGN_BYTES);
  *((int32_t*)magic) = XON_MAGIC;
}


DESTRUCTOR
void fini() 
{
}


/*******************************************************
 *
 * Object Builder
 *
 *******************************************************/

EXPORTED_SYMBOL
xon_obj_builder xon_obj_builder_new()
{
  const int size = 4096;
  void *buf = malloc(size);
  if (buf == NULL)
    return NULL;
  return xon_obj_builder_new_with_buf(buf, size, true);
}


EXPORTED_SYMBOL
xon_obj_builder xon_obj_builder_new_with_buf
(void *initial_buffer, size_t size, bool free_buffer)
{
  assert(size >= 2*ALIGN_BYTES);
  xon_obj_builder builder = malloc(sizeof(xon_obj_builder_struct));
  memcpy(magic, initial_buffer, 2*ALIGN_BYTES);

  builder->initial_buf = (char*)initial_buffer;
  builder->initial_capacity = size;
  builder->free_initial_buffer = free_buffer;

  builder->buf = (char*)initial_buffer;
  builder->end = builder->buf + 2*ALIGN_BYTES;
  builder->capacity = size;
  builder->free_buffer = false;

  return builder;
}


EXPORTED_SYMBOL
void xon_obj_builder_free(xon_obj_builder builder)
{
  if (builder->buf != builder->initial_buf)
    free(builder->buf);
  if (builder->free_initial_buffer)
    free(builder->initial_buf);
  free(builder);
}


static
inline size_t suggested_size(size_t actual_size)
{
  return ALIGN_ROUND_UP_SIZE(actual_size * 2);
}


static inline
enum XON_STATUS 
enlarge(xon_obj_builder builder, size_t minimum_size)
{
  if (minimum_size <= builder->capacity)
    return XON_OK;

  // TODO: try realloc

  size_t new_size = suggested_size(new_size);
  void *new_buf = malloc(new_size);
  if (new_buf == NULL)
    return XON_ERROR_MALLOC;

  ptrdiff_t length = builder->end - builder->buf;
  memcpy(new_buf, builder->buf, length);
  if (builder->buf != builder->initial_buf) 
    free(builder->buf);

  builder->buf = (char*)new_buf;
  builder->end = builder->buf + length;
  builder->capacity = new_size;
  return XON_OK;
}


static inline
enum XON_STATUS 
ensure_capacity(xon_obj_builder builder, size_t additional_size) 
{
  ptrdiff_t length = (builder->end - builder->buf) + additional_size;
  return enlarge(builder, length);
}


EXPORTED_SYMBOL
enum XON_STATUS 
xon_obj_builder_add_string
(xon_obj_builder builder, const char *key, const char *value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = strlen(value)+1;
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(key_len);
  enum XON_STATUS rc;
  rc = ensure_capacity(builder, aligned_key_len + aligned_value_len);
  if (rc != XON_OK)
    return rc;

  memcpy(builder->end, key, key_len);
  builder->end = ALIGN_ROUND_UP_PTR(builder->end);
  assert(builder->end == builder->buf + aligned_key_len);

  memcpy(builder->end, value, value_len);
  builder->end = ALIGN_ROUND_UP_PTR(builder->end);
  assert(builder->end == builder->buf + aligned_key_len | aligned_value_len);
  return XON_OK;
}


EXPORTED_SYMBOL
xon_obj xon_obj_builder_finish(xon_obj_builder builder)
{
  ptrdiff_t length = builder->end - builder->buf;
  xon_obj obj = (xon_obj)malloc(length);
  memcpy(obj, builder->buf, length);
  return obj;
}




/*******************************************************
 *
 * Object 
 *
 *******************************************************/

EXPORTED_SYMBOL
void xon_obj_print(xon_obj obj)
{
  printf("Object at %x\n", obj);
}

