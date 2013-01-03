#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "macros.h"
#include "xon/object.h"

NAMESPACE_XON_C_API_BEGIN

/*******************************************************
 *
 * Initialization / Finalization
 *
 *******************************************************/

char magic[2*ALIGN_BYTES];


CONSTRUCTOR_C
void init() 
{
  memset(magic, 'a', 2*ALIGN_BYTES);
  *((int32_t*)magic) = XON_MAGIC;
}


DESTRUCTOR_C
void fini() 
{
}


/*******************************************************
 *
 * Object Builder
 *
 *******************************************************/

EXPORTED_SYMBOL_C
xon_obj_builder xon_obj_builder_new()
{
  const int size = 4096;
  void *buf = malloc(size);
  if (buf == NULL)
    return NULL;
  xon_obj_builder_new_with_buf(buf, size, true);
}


EXPORTED_SYMBOL_C
xon_obj_builder xon_obj_builder_new_with_buf
(void *initial_buffer, size_t size, bool free_buffer)
{
  /* Minimal object: magic + size + zero */
  assert(size >= 3*ALIGN_BYTES);
  xon_obj_builder builder = 
    (xon_obj_builder)malloc(sizeof(xon_obj_builder_struct));
  if (builder == NULL)
    return NULL;
  memcpy(initial_buffer, magic, 2*ALIGN_BYTES);

  builder->initial_buf = (char*)initial_buffer;
  builder->initial_capacity = size;
  builder->free_initial_buffer = free_buffer;

  builder->buf = (char*)initial_buffer;
  builder->end = builder->buf + 2*ALIGN_BYTES;
  builder->capacity = size;

  return builder;
}


EXPORTED_SYMBOL_C
void xon_obj_builder_delete(xon_obj_builder builder)
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


/* Enlarge the buffer to hold at least minimum_size bytes. 
 */
static inline
enum XON_STATUS 
enlarge(xon_obj_builder builder, size_t minimum_size)
{
  if (minimum_size <= builder->capacity)
    return XON_OK;

  ptrdiff_t length = builder->end - builder->buf;
  size_t new_size = suggested_size(new_size);
  void *new_buf;
  if (builder->buf == builder->initial_buf) {
    new_buf = malloc(new_size);
    if (new_buf == NULL)
      return XON_ERROR_MALLOC;
    memcpy(new_buf, builder->buf, length);
    free(builder->buf);
  } else {
    new_buf = realloc(builder->buf, new_size);
    if (new_buf == NULL)
      return XON_ERROR_MALLOC;
  }
  builder->buf = (char*)new_buf;
  builder->end = builder->buf + length;
  builder->capacity = new_size;
  return XON_OK;
}


/* Enlarge the buffer to hold at least minimum_size bytes.
 */
static inline
enum XON_STATUS 
ensure_capacity(xon_obj_builder builder, size_t additional_size) 
{
  ptrdiff_t length = (builder->end - builder->buf) + additional_size;
  return enlarge(builder, length);
}


EXPORTED_SYMBOL_C
enum XON_STATUS 
xon_obj_builder_add_string
(xon_obj_builder builder, const char *key, const char *value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = strlen(value)+1;
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  enum XON_STATUS rc;
  rc = ensure_capacity(builder, ALIGN_BYTES + aligned_key_len + aligned_value_len);
  if (rc != XON_OK)
    return rc;
  
  *((int*)builder->end) = XON_ELEMENT_STRING;
  builder->end += ALIGN_BYTES;
  
  memcpy(builder->end, key, key_len);
  builder->end = builder->end + aligned_key_len;

  memcpy(builder->end, value, value_len);
  builder->end = builder->end + aligned_value_len;
  return XON_OK;
}


EXPORTED_SYMBOL_C
enum XON_STATUS 
xon_obj_builder_add_double
(xon_obj_builder builder, const char *key, double value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = sizeof(double);
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  enum XON_STATUS rc;
  rc = ensure_capacity(builder, ALIGN_BYTES + aligned_key_len + aligned_value_len);
  if (rc != XON_OK)
    return rc;
  
  *((int*)builder->end) = XON_ELEMENT_DOUBLE;
  builder->end += ALIGN_BYTES;
  
  memcpy(builder->end, key, key_len);
  builder->end = builder->end + aligned_key_len;

  *((double*)builder->end) = value;
  builder->end = builder->end + aligned_value_len;
  return XON_OK;
}


EXPORTED_SYMBOL_C
enum XON_STATUS 
xon_obj_builder_add_int32
(xon_obj_builder builder, const char *key, int32_t value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = sizeof(int32_t);
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  enum XON_STATUS rc;
  rc = ensure_capacity(builder, ALIGN_BYTES + aligned_key_len + aligned_value_len);
  if (rc != XON_OK)
    return rc;

  *((int*)builder->end) = XON_ELEMENT_INT32;
  builder->end += ALIGN_BYTES;
  
  memcpy(builder->end, key, key_len);
  builder->end += aligned_key_len;

  *((int32_t*)builder->end) = value;
  builder->end += aligned_value_len;
  return XON_OK;
}


EXPORTED_SYMBOL_C
enum XON_STATUS 
xon_obj_builder_add_int64
(xon_obj_builder builder, const char *key, int64_t value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = sizeof(int64_t);
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  enum XON_STATUS rc;
  rc = ensure_capacity(builder, ALIGN_BYTES + aligned_key_len + aligned_value_len);
  if (rc != XON_OK)
    return rc;
  
  *((int*)builder->end) = XON_ELEMENT_INT64;
  builder->end += ALIGN_BYTES;
  
  memcpy(builder->end, key, key_len);
  builder->end += aligned_key_len;

  *((int64_t*)builder->end) = value;
  builder->end += aligned_value_len;
  return XON_OK;
}


EXPORTED_SYMBOL_C
xon_obj xon_obj_builder_get(xon_obj_builder builder)
{
  ptrdiff_t length = builder->end - builder->buf;
  xon_obj obj = (xon_obj)malloc(length + ALIGN_BYTES);
  if (obj == NULL)
    return obj;
  
  /* Copy the actual data */
  memcpy(obj, builder->buf, length);
  
  /* Set the length field */
  *(int*)((char*)obj + ALIGN_BYTES) = length + ALIGN_BYTES;

  /* Set the end-of-document marker */
  memset((char*)obj + length, 0, ALIGN_BYTES);
  
  /* Get ready for building the next obj */
  if (builder->buf != builder->initial_buf) 
    free(builder->buf);
  builder->buf = builder->initial_buf;
  builder->end = builder->buf + 2*ALIGN_BYTES;
  builder->capacity = builder->initial_capacity;

  return obj;
}





/*******************************************************
 *
 * Object Reader
 *
 *******************************************************/


static
xon_obj_reader reader_new_realloc(xon_obj_reader reader, size_t n)
{
  const int N = 32;
  if (n % N != 0)
    return reader;
  reader->types  = (int*)  realloc(reader->types,  sizeof(int)   * (n+N));
  reader->keys   = (char**)realloc(reader->keys,   sizeof(char*) * (n+N));
  reader->values = (char**)realloc(reader->values, sizeof(char*) * (n+N));
  if (reader->types == NULL || reader->keys == NULL || reader->values == NULL) {
    xon_obj_reader_delete(reader);
    return NULL;
  }
  return reader;
}


EXPORTED_SYMBOL_C
xon_obj_reader xon_obj_reader_new(xon_obj obj) 
{
  char* pos = (char*)obj;
  size_t size = xon_obj_size(obj);
  char* end = (char*)obj + size;
  if (memcmp(pos, magic, ALIGN_BYTES) != 0) {
    fprintf(stderr, "libxon-obj: binary object from different architecture.\n");
    return NULL;
  }
  // skip ahead to actual data
  pos += 2 * ALIGN_BYTES;  

  xon_obj_reader reader = 
    (xon_obj_reader)malloc(sizeof(xon_obj_reader_struct));
  if (reader == NULL)
    return NULL;
  reader->obj = (char*)obj;
  reader->types = NULL;
  reader->keys = NULL;
  reader->values = NULL;
  reader = reader_new_realloc(reader, 0);
  if (reader == NULL)
    return NULL;

  int n = 0;
  while (*pos != 0) {
    int type = *(int*)pos;
    pos += ALIGN_BYTES;
    reader->types[n] = type;
    
    size_t key_len = strnlen(pos, end-pos)+1;
    size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
    size_t value_len, aligned_value_len;
    reader->keys[n] = pos;
    pos += aligned_key_len;
    reader->values[n] = pos;
    switch (type) {
    case XON_ELEMENT_STRING:
      value_len = strnlen(pos, end-pos)+1;
      aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
      pos += aligned_value_len;
      break;
    case XON_ELEMENT_DOUBLE:
      pos += ALIGN_ROUND_UP_SIZE(sizeof(double)); 
      break;
    case XON_ELEMENT_INT32:
      pos += ALIGN_ROUND_UP_SIZE(sizeof(int32_t)); 
      break;
    case XON_ELEMENT_INT64:
      pos += ALIGN_ROUND_UP_SIZE(sizeof(int64_t)); 
      break;
    default:
      fprintf(stderr, "libxon-obj: Unsupported type: %x\n", type);
      xon_obj_reader_delete(reader);
      return NULL;
    }
    // printf("#%d: type=0x%x 0x%x %s 0x%x\n", n, 
    //       reader->types[n], reader->keys[n], reader->keys[n], reader->values[n]);
    n += 1;
    reader = reader_new_realloc(reader, n);
    if (reader == NULL)
      return NULL;

  }
  reader->n_elements = n;
  return reader;
}


EXPORTED_SYMBOL_C
int xon_obj_reader_count(xon_obj_reader reader)
{
  return reader->n_elements;
}


EXPORTED_SYMBOL_C
int xon_obj_reader_type(xon_obj_reader reader, int pos)
{
  return reader->types[pos];
}


EXPORTED_SYMBOL_C
const char* xon_obj_reader_key(xon_obj_reader reader, int pos)
{
  return reader->keys[pos];
}


EXPORTED_SYMBOL_C
void* xon_obj_reader_get_value(xon_obj_reader reader, int pos)
{
  return reader->values[pos];
}


static inline 
bool check_type(xon_obj_reader reader, int pos, int type)
{
  if (reader->types[pos] == type) 
    return true;
  fprintf(stderr, "libxon-obj: Wrong type at #%d: expected 0x%x, got 0x%x.\n", 
          pos, type, xon_obj_reader_type(reader, pos));
  return false;
}


EXPORTED_SYMBOL_C
const char* xon_obj_reader_get_string(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_STRING);
  return (char*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
double xon_obj_reader_get_double(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_DOUBLE);
  return *(double*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
int32_t xon_obj_reader_get_int32(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_INT32);
  return *(int32_t*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
int64_t xon_obj_reader_get_int64(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_INT64);
  return *(int64_t*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
void xon_obj_reader_delete(xon_obj_reader reader)
{
  free(reader->types);
  free(reader->keys);
  free(reader->values);
  free(reader);
}


/*******************************************************
 *
 * Object 
 *
 *******************************************************/

EXPORTED_SYMBOL_C
void xon_obj_print(xon_obj obj)
{
  char *output = xon_obj_string(obj);
  if (output == NULL) {
    fprintf(stderr, "libxon-obj: Could not stringify object.\n");
    return;
  }
  printf("%s\n", output);
  free(output);
}


EXPORTED_SYMBOL_C
size_t xon_obj_size(xon_obj obj)
{
  return *(int*)((char*)obj + ALIGN_BYTES);
}


EXPORTED_SYMBOL_C
char* xon_obj_string(xon_obj obj)
{
  return xon_obj_string_indent(obj, true, "");
}


EXPORTED_SYMBOL_C
char* xon_obj_string_indent
(xon_obj obj, bool address, const char *prefix)
{
  const char *crlf = "\n";
  const int max_len = 79; /* maximum line length */

  xon_obj_reader reader = xon_obj_reader_new(obj);
  if (reader == NULL)
    return NULL;
  int n = xon_obj_reader_count(reader);
  char **lines = (char**)malloc(sizeof(char*) * (n + 2));
  if (lines == NULL)
    return NULL;
  for (int i=0; i<=n+1; i++)
    lines[i] = (char*)malloc(max_len+1);
  
  if (address)
    snprintf(lines[0], max_len, "0x%.8x %s{", (char*)obj, prefix);
  else
    snprintf(lines[0], max_len, "%s{", (char*)obj, prefix);

  char addr[max_len+1];
  for (int i=0; i<n; i++) {
    char* line = lines[i+1];
    if (line == NULL) 
      continue;
    int type = xon_obj_reader_type(reader, i);
    const char *key = xon_obj_reader_key(reader, i);
    if (address)
      snprintf(addr, max_len, "0x%.8x ", key);
    else
      strncpy(addr, prefix, max_len);

    switch (type){
    case XON_ELEMENT_STRING:
      snprintf(line, max_len, "%s %s \"%s\": \"%s\",", 
               addr, prefix, key, xon_obj_reader_get_string(reader, i));
      break;
    case XON_ELEMENT_DOUBLE:
      snprintf(line, max_len, "%s %s \"%s\": %g,", 
               addr, prefix, key, xon_obj_reader_get_double(reader, i));
      break;
    case XON_ELEMENT_INT32:
      snprintf(line, max_len, "%s %s \"%s\": %i,", 
               addr, prefix, key, xon_obj_reader_get_int32(reader, i));
      break;
    case XON_ELEMENT_INT64:
      snprintf(line, max_len, "%s %s \"%s\": %li,", 
               addr, prefix, key, xon_obj_reader_get_int64(reader, i));
      break;
    default:
      snprintf(line, max_len, "%s %s \"%s\": unknown type at %p,", 
               addr, prefix, key, xon_obj_reader_get_value(reader, i));
    }
  }

  char *end_marker = (char*)obj + (xon_obj_size(obj) - ALIGN_BYTES);
  if (address)
    snprintf(lines[n+1], max_len, "0x%.8x %s}", end_marker, prefix);
  else
    snprintf(lines[n+1], max_len, "%s}", end_marker, prefix);
  
  int len = 1;
  for (int i=0; i<=n+1; i++) {
    char* line = lines[i];
    if (line == NULL)
      continue;
    len += strlen(line) + strlen(crlf);
  }

  char *output = (char*)malloc(len);
  char *pos = output;
  if (output != NULL)
    for (int i=0; i<=n+1; i++) {
      char* line = lines[i];
      if (line == NULL)
        continue;
      strcpy(pos, line);
      pos += strlen(line);
      if (i == n+1)
        break;
      strcpy(pos, crlf);
      pos += strlen(crlf);
    }

  for (int i=0; i<=n+1; i++)
    free(lines[i]);
  free(lines);
  xon_obj_reader_delete(reader);
  return output;
}


NAMESPACE_XON_C_API_END
