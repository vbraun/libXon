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

char magic[ALIGN_BYTES];


CONSTRUCTOR
void init() 
{
  memset(magic, 0, ALIGN_BYTES);
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
  return xon_obj_builder_new_with_buf(buf, size, true);
}


EXPORTED_SYMBOL
xon_obj_builder xon_obj_builder_new_with_buf
(void *initial_buffer, size_t size, bool free_buffer)
{
  assert(size >= ALIGN_BYTES);
  xon_obj_builder builder = malloc(sizeof(xon_obj_builder_struct));
  memcpy(magic, initial_buffer, ALIGN_BYTES);
  builder->buf = (char*)initial_buffer;
  builder->end = builder->buf + ALIGN_BYTES;
  builder->capacity = size;
  builder->free_buffer = free_buffer;
  return builder;
}


EXPORTED_SYMBOL
void xon_obj_builder_free(xon_obj_builder builder)
{
  free(builder->buf);
  free(builder);
}


inline size_t suggest_size(size_t actual_size)
{
  assert(actual_size > 0);
  return actual_size * 2;
}


enum XON_STATUS 
enlarge(xon_obj_builder builder, size_t minimum_size)
{
  if (minimum_size <= builder->capacity)
    return XON_OK;
  // TODO: try realloc
  
  size_t new_size = suggest_size(new_size);
  void *new_buf = malloc(new_size);
  ptrdiff_t length = builder->end - builder->buf;
  memcpy(new_buf, builder->buf, length);
  if (builder->free_buffer) 
    free(builder->buf);
  else 
    builder->free_buffer = true;
  builder->buf = (char*)new_buf;
  builder->end = builder->buf + length;
  builder->capacity = new_size;
  return XON_OK;
}


EXPORTED_SYMBOL
enum XON_STATUS 
xon_obj_builder_add
(xon_obj_builder builder, const char *key, const char *value)
{
}


EXPORTED_SYMBOL
xon_obj xon_obj_builder_finish(xon_obj_builder builder)
{
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

