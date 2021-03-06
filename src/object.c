#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <endian.h>
#include <ctype.h>

#include "macros.h"
#include "xon/object.h"
#include "debug.h"

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
#ifdef XON_FILL_PADDING  /* make valgrind happy */
  memset(magic, '.', 2*ALIGN_BYTES);
#endif
  *((int32_t*)magic) = htole32(XON_MAGIC);
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
  return xon_obj_builder_new_with_buf(buf, size, true);
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
#ifdef XON_FILL_PADDING  /* make valgrind happy */
  memset(initial_buffer, '.', size);
#endif
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
xon_status enlarge(xon_obj_builder builder, size_t minimum_size)
{
  if (minimum_size <= builder->capacity)
    return XON_OK;

  ptrdiff_t length = builder->end - builder->buf;
  size_t new_size = suggested_size(minimum_size);
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
#ifdef XON_DEBUG  /* make valgrind happy */
  memset((char*)new_buf+length, 0xff, new_size-length);
#endif
  builder->buf = (char*)new_buf;
  builder->end = builder->buf + length;
  builder->capacity = new_size;
  return XON_OK;
}


/* Enlarge the buffer to hold at least minimum_size bytes.
 */
static inline
xon_status ensure_capacity(xon_obj_builder builder, size_t additional_size) 
{
  ptrdiff_t length = (builder->end - builder->buf) + additional_size;
  return enlarge(builder, length);
}


EXPORTED_SYMBOL_C
xon_status xon_obj_builder_add_string
(xon_obj_builder builder, const char *key, const char *value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = strlen(value)+1;
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  xon_status rc;
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
xon_status xon_obj_builder_add_double
(xon_obj_builder builder, const char *key, double value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = sizeof(double);
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  xon_status rc;
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
xon_status xon_obj_builder_add_int32
(xon_obj_builder builder, const char *key, int32_t value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = sizeof(int32_t);
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  xon_status rc;
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
xon_status xon_obj_builder_add_int64
(xon_obj_builder builder, const char *key, int64_t value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = sizeof(int64_t);
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  xon_status rc;
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
xon_status xon_obj_builder_add_bool
(xon_obj_builder builder, const char *key, bool value)
{
  size_t key_len = strlen(key)+1;
  size_t value_len = sizeof(bool);
  size_t aligned_key_len = ALIGN_ROUND_UP_SIZE(key_len);
  size_t aligned_value_len = ALIGN_ROUND_UP_SIZE(value_len);
  xon_status rc;
  rc = ensure_capacity(builder, ALIGN_BYTES + aligned_key_len + aligned_value_len);
  if (rc != XON_OK)
    return rc;

  *((int*)builder->end) = XON_ELEMENT_BOOLEAN;
  builder->end += ALIGN_BYTES;
  
  memcpy(builder->end, key, key_len);
  builder->end += aligned_key_len;

  *((bool*)builder->end) = value;
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
  if (!xon_obj_verify_architecture(obj)) {
    error_printf("libxon-obj: binary object from different architecture.\n");
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
    case XON_ELEMENT_BOOLEAN:
      pos += ALIGN_ROUND_UP_SIZE(sizeof(bool)); 
      break;
    default:
      error_printf("libxon-obj: Unsupported type: 0x%x\n", (unsigned int)type);
      xon_obj_reader_delete(reader);
      return NULL;
    }
    n += 1;
    reader = reader_new_realloc(reader, n);
    if (reader == NULL)
      return NULL;

  }
  reader->n_elements = n;
  assert(pos + ALIGN_BYTES == end);
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
bool xon_obj_reader_has_key(xon_obj_reader reader, const char *key)
{
  for (int i=0; i < reader->n_elements; i++)
    if (strcmp(key, reader->keys[i]) == 0)
      return true;
  return false;
}


EXPORTED_SYMBOL_C
void* xon_obj_reader_get_value_pos(xon_obj_reader reader, int pos)
{
  return reader->values[pos];
}


EXPORTED_SYMBOL_C
void* xon_obj_reader_get_value_key(xon_obj_reader reader, const char *key)
{
  for (int i = 0; i < reader->n_elements; i++)
    if (strcmp(key, reader->keys[i]) == 0)
      return reader->values[i];
  return NULL;
}


static inline 
bool check_type(xon_obj_reader reader, int pos, int type)
{
  assert((pos >=0) && (pos < reader->n_elements));
  if (reader->types[pos] == type) 
    return true;
  error_printf("libxon-obj: Wrong type at #%d: expected 0x%x, got 0x%x.\n", 
          pos, type, xon_obj_reader_type(reader, pos));
  return false;
}


EXPORTED_SYMBOL_C
int xon_obj_reader_find(xon_obj_reader reader, const char *key)
{
  for (int i = 0; i < reader->n_elements; i++)
    if (strcmp(key, reader->keys[i]) == 0)
      return i;
  return -1;
}

EXPORTED_SYMBOL_C
int xon_obj_reader_find_type(xon_obj_reader reader, int type, const char *key)
{
  for (int i = 0; i < reader->n_elements; i++)
    if ((reader->types[i] == type) && (strcmp(key, reader->keys[i]) == 0))
      return i;
  return -1;
}


static inline
int find_position(xon_obj_reader reader, int type, const char *key)
{
  for (int i = 0; i < reader->n_elements; i++)
    if ((reader->types[i] == type) && (strcmp(key, reader->keys[i]) == 0))
      return i;
  error_printf("libxon-obj: Key \"%s\" with type 0x%x not found.\n", 
               key, type);
  return -1;
}

EXPORTED_SYMBOL_C
const char* xon_obj_reader_get_string_pos(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_STRING);
  return (char*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
const char* xon_obj_reader_get_string_key(xon_obj_reader reader, const char *key)
{
  const int pos = find_position(reader, XON_ELEMENT_STRING, key);
  return (char*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
double xon_obj_reader_get_double_pos(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_DOUBLE);
  return *(double*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
double xon_obj_reader_get_double_key(xon_obj_reader reader, const char *key)
{
  const int pos = find_position(reader, XON_ELEMENT_DOUBLE, key);
  return *(double*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
int32_t xon_obj_reader_get_int32_pos(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_INT32);
  return *(int32_t*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
int32_t xon_obj_reader_get_int32_key(xon_obj_reader reader, const char *key)
{
  const int pos = find_position(reader, XON_ELEMENT_INT32, key);
  return *(int32_t*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
int64_t xon_obj_reader_get_int64_pos(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_INT64);
  return *(int64_t*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
int64_t xon_obj_reader_get_int64_key(xon_obj_reader reader, const char *key)
{
  const int pos = find_position(reader, XON_ELEMENT_INT64, key);
  return *(int64_t*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
bool xon_obj_reader_get_bool_pos(xon_obj_reader reader, int pos)
{
  check_type(reader, pos, XON_ELEMENT_BOOLEAN);
  return *(bool*)(reader->values[pos]);
}


EXPORTED_SYMBOL_C
bool xon_obj_reader_get_bool_key(xon_obj_reader reader, const char *key)
{
  const int pos = find_position(reader, XON_ELEMENT_BOOLEAN, key);
  return *(bool*)(reader->values[pos]);
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
bool xon_obj_verify_architecture(xon_obj obj) 
{
  return xon_obj_is_bson(obj) || (memcmp(obj, magic, sizeof(int32_t)) == 0);
}


EXPORTED_SYMBOL_C
bool xon_obj_is_bson(xon_obj obj) 
{
  int32_t size = le32toh(*(int32_t*)obj);
  return size > 0;
}


EXPORTED_SYMBOL_C
void xon_obj_print(xon_obj obj)
{
  char *output = xon_obj_string(obj);
  if (output == NULL) {
    error_printf("libxon-obj: Could not stringify object.\n");
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


static inline
char *safe_strdup(const char *string)
{
  char *str = strdup(string);
  if (str == NULL)
    return NULL;
  for (size_t i=0; i<strlen(str); i++)
    if (!isprint(str[i]))
      str[i] = '.';
  return str;
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
    snprintf(lines[0], max_len, "%p %s{", (void*)obj, prefix);
  else
    snprintf(lines[0], max_len, "%s{", prefix);

  char addr[max_len+1];
  for (int i=0; i<n; i++) {
    char* line = lines[i+1];
    if (line == NULL) 
      continue;
    int type = xon_obj_reader_type(reader, i);
    const char *key = xon_obj_reader_key(reader, i);
    if (address)
      snprintf(addr, max_len, "%p %s", (void*)key, prefix);
    else
      strncpy(addr, prefix, max_len);

    char *value_str;
    switch (type){
    case XON_ELEMENT_STRING:
      value_str = safe_strdup(xon_obj_reader_get_string_pos(reader, i));
      snprintf(line, max_len, "%s %s \"%s\": \"%s\",", 
               addr, prefix, key, value_str);
      free(value_str);
      break;
    case XON_ELEMENT_DOUBLE:
      snprintf(line, max_len, "%s %s \"%s\": %g,", 
               addr, prefix, key, xon_obj_reader_get_double_pos(reader, i));
      break;
    case XON_ELEMENT_INT32:
      snprintf(line, max_len, "%s %s \"%s\": %i,", 
               addr, prefix, key, xon_obj_reader_get_int32_pos(reader, i));
      break;
    case XON_ELEMENT_INT64:
      snprintf(line, max_len, "%s %s \"%s\": %li,", 
               addr, prefix, key, xon_obj_reader_get_int64_pos(reader, i));
      break;
    case XON_ELEMENT_BOOLEAN:
      snprintf(line, max_len, "%s %s \"%s\": %s,", 
               addr, prefix, key, 
               xon_obj_reader_get_bool_pos(reader, i) ? "true" : "false" );
      break;
    default:
      snprintf(line, max_len, "%s %s \"%s\": unknown type at %p,", 
               addr, prefix, key, xon_obj_reader_get_value_pos(reader, i));
    }
  }

  char *end_marker = (char*)obj + (xon_obj_size(obj) - ALIGN_BYTES);
  if (address)
    snprintf(lines[n+1], max_len, "%p %s}", (void*)end_marker, prefix);
  else
    snprintf(lines[n+1], max_len, "%s}", prefix);
  
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


EXPORTED_SYMBOL_C
void xon_obj_hexdump(xon_obj obj)
{
  char *begin = (char*) obj;
  char *end   = begin + xon_obj_size(obj);

  const char* address = "0x%.8x:  ";
  for (char *p = begin; p != end; p++) {

    if ((p-begin) % 16 == 0) {
      if (p != begin)
        printf("\n");
      printf(address, p-begin);
    } else if ((p-begin) % 8 == 0) {
      printf(" :");
    }

    unsigned char ch = *p;
    if (isprint(ch))
      printf("  %c", ch);
    else
      printf(" %.2x", ch);
  }
  printf("\n");
}



NAMESPACE_XON_C_API_END
