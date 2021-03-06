#ifndef XON__OBJECT__H
#define XON__OBJECT__H

/** @file */ 


#ifndef __cplusplus
#include <stdbool.h>
#endif 

#include <inttypes.h>

#include <xon/support.h>
#include <xon/status.h>
#include <xon/constants.h>

NAMESPACE_XON_C_API_BEGIN


/*! @brief A pointer to a binary object
 */
typedef void* xon_obj;



/*******************************************************
 *
 * Object Builder
 *
 *******************************************************/

/*! Binary object builder. The layout and content of this struct is an
 *  implementation detail and subject to change without notice.
 */
typedef struct {
  /*! @brief The current buffer, can change if we need to realloc. */
  char* buf;
  /*! @brief The pointer past the end of the currently used buf.
   * 
   *  This is also past padding which may come after the currently
   *  used part of buf. It is always aligned. */
  char* end;
  /*! \brief Size allocated for buf */
  size_t capacity;

  /*! \brief The initial buffer
   *  
   *  Users can specify an initial buffer, which is used for small
   *  objects. May reside on the stack.
   */
  char* initial_buf;
  /*! \brief The size allocated for initial_buf */
  size_t initial_capacity;
  /*! \brief Whether initial_buf should be freed */
  bool free_initial_buffer;
} xon_obj_builder_struct;


/*! @brief Pointer to a xon_obj_builder_struct 
 *
 *  All public functions take / return such pointers, never the actual
 *  struct.
 */
typedef xon_obj_builder_struct *xon_obj_builder;


/*! @brief Create a new object builder 
 */
xon_obj_builder xon_obj_builder_new();


/*! @brief Create a new object builder with an initial buffer 
 *
 *  @param initial_buffer the initial buffer
 *  @param size the size in bytes of the initial buffer
 *  @param free_buffer whether to free() the initial buffer in
 *         xon_obj_builder_delete.
 *
 *  The initial buffer will be used for small objects. 
 */
xon_obj_builder xon_obj_builder_new_with_buf
(void *initial_buffer, size_t size, bool free_buffer);


/*! @brief Destroy an object builder 
 *
 *  @param builder the builder to destroy
 *
 *  You must call this function on every builder that you created.
 */
void xon_obj_builder_delete(xon_obj_builder builder);


/*! @brief Add a string
 *
 *  @param builder the builder
 *  @param key the key under which to add the string
 *  @param value the string to add
 *  
 *  Returns XON_ERROR_MALLOC if the buffer needed to be reallocated
 *  but malloc failed. Nothing is added in this case.
 */
xon_status xon_obj_builder_add_string
(xon_obj_builder builder, const char *key, const char *value);


/*! @brief Add a double
 *
 *  @param builder the builder
 *  @param key the key under which to add the string
 *  @param value the double to add
 *  
 *  Returns XON_ERROR_MALLOC if the buffer needed to be reallocated
 *  but malloc failed. Nothing is added in this case.
 */
xon_status xon_obj_builder_add_double
(xon_obj_builder builder, const char *key, double value);


/*! @brief Add a 32-bit integer
 *
 *  @param builder the builder
 *  @param key the key under which to add the string
 *  @param value the integer to add
 *  
 *  Returns XON_ERROR_MALLOC if the buffer needed to be reallocated
 *  but malloc failed. Nothing is added in this case.
 */
xon_status xon_obj_builder_add_int32
(xon_obj_builder builder, const char *key, int32_t value);


/*! @brief Add a 64-bit integer
 *
 *  @param builder the builder
 *  @param key the key under which to add the string
 *  @param value the integer to add
 *  
 *  Returns XON_ERROR_MALLOC if the buffer needed to be reallocated
 *  but malloc failed. Nothing is added in this case.
 */
xon_status xon_obj_builder_add_int64
(xon_obj_builder builder, const char *key, int64_t value);


/*! @brief Add a boolean
 *
 *  @param builder the builder
 *  @param key the key under which to add the string
 *  @param value the boolean to add
 *  
 *  Returns XON_ERROR_MALLOC if the buffer needed to be reallocated
 *  but malloc failed. Nothing is added in this case.
 */
xon_status xon_obj_builder_add_bool
(xon_obj_builder builder, const char *key, bool value);


/*! @brief Create a binary object
 *
 *  @param builder the builder
 *  @return the object created by the key/value pairs put into the
 *          builder.
 *  
 *  This function returns a newly malloc'ed memory block containing
 *  the binary object. It is your responsibility to free() it when you
 *  are done with it.
 *
 *  Returns NULL if malloc failed.
 */
xon_obj xon_obj_builder_get(xon_obj_builder builder);





/*******************************************************
 *
 * Object Reader
 *
 *******************************************************/

/*! Binary object reader. The layout and content of this struct is an
 *  implementation detail and subject to change without notice.
 */
typedef struct {
  /*! @brief The object we are reading */
  char* obj;
  /*! @brief Number of elements
   * 
   *  This is the number of key/value pairs (excluding those of
   *  contained sub-documents or arrays).
   */
  int n_elements;
  int * types;     /*!< Array of the element types */
  char **keys;     /*!< Array of pointers to the keys */
  char **values;   /*!< Array of pointers to the values */
} xon_obj_reader_struct;


/*! @brief Pointer to a xon_obj_reader_struct 
 *
 *  All public functions take / return such pointers, never the actual
 *  struct.
 */
typedef xon_obj_reader_struct *xon_obj_reader;


/*! @brief Create a new reader
 *
 *  @param obj the object to deserialize. Must not be changed during
 *             the lifetime of the reader.
 *  
 *  The reader is a set of functions that can (quickly) retrieve
 *  elements of the binary object.
 */
xon_obj_reader xon_obj_reader_new(xon_obj obj);


/*! @brief Get the number of entries
 *
 *  @param reader the reader (deserializer)
 *  @return the number of key/value pairs
 */
int xon_obj_reader_count(xon_obj_reader reader);
  

/*! @brief Get the internal position of the key
 *
 *  @param reader the reader (deserializer)
 *  @param key the key
 *  @return internal position of the key
 * 
 *  If you want to look up the type and value of an entry, it is
 *  faster to find the index of the key once and then access the
 *  entries by position instead of by key.
 */
int xon_obj_reader_find(xon_obj_reader reader, const char *key);

/*! @brief Get the internal position of the key with specific type
 *
 *  @param reader the reader (deserializer)
 *  @param type the entry type you are looking for
 *  @param key the key
 *  @return internal position of the key
 * 
 *  If you want to look up the type and value of an entry, it is
 *  faster to find the index of the key once and then access the
 *  entries by position instead of by key.
 *
 *  See the XON_ELEMENT_* constants for the possible type values.
 */
int xon_obj_reader_find_type(xon_obj_reader reader, int type, const char *key);

/*! @brief Get the type of the entry at position pos
 *
 *  @param reader the reader (deserializer)
 *  @param pos the position of the entry
 *  @return the type of the element (just as in BSON).
 * 
 *  See the XON_ELEMENT_* constants for the possible values.
 */
int xon_obj_reader_type(xon_obj_reader reader, int pos);

 
/*! @brief Get the key of the entry at position pos
 *
 *  @param reader the reader (deserializer)
 *  @param pos the position of the entry
 *  @return the key
 */
const char* xon_obj_reader_key(xon_obj_reader reader, int pos);

/*! @brief Whether the given key exists
 *
 *  @param reader the reader (deserializer)
 *  @param key the key
 *  @return boolean. True if key has a value.
 */
bool xon_obj_reader_has_key(xon_obj_reader reader, const char *key);


/*! @brief Get the value of the entry at position pos
 *
 *  @param reader the reader (deserializer)
 *  @param pos the position of the entry
 *  @return a pointer to the value.
 */
void* xon_obj_reader_get_value_pos(xon_obj_reader reader, int pos);
/*! @brief Like xon_obj_reader_get_value_pos, but with key instead of position */
void* xon_obj_reader_get_value_key(xon_obj_reader reader, const char *key);


/*! @brief Get the string value at position pos
 *
 *  @param reader the reader (deserializer)
 *  @param pos the position of the entry
 * 
 *  It is your responsibility to ensure that the type of the value
 *  actually is a string. See also xon_obj_reader_type.
 */
const char* xon_obj_reader_get_string_pos(xon_obj_reader reader, int pos);
/*! @brief Like xon_obj_reader_get_string_pos, but with key instead of position */
const char* xon_obj_reader_get_string_key(xon_obj_reader reader, const char *key);


/*! @brief Get the double at position pos
 *
 *  @param reader the reader (deserializer)
 *  @param pos the position of the entry
 * 
 *  It is your responsibility to ensure that the type of the value
 *  actually is a 64-bit floating point number. See also
 *  xon_obj_reader_type.
 */
double xon_obj_reader_get_double_pos(xon_obj_reader reader, int pos);
/*! @brief Like xon_obj_reader_get_double_pos, but with key instead of position */
double xon_obj_reader_get_double_key(xon_obj_reader reader, const char *key);


/*! @brief Get the 32-bit int at position pos
 *
 *  @param reader the reader (deserializer)
 *  @param pos the position of the entry
 * 
 *  It is your responsibility to ensure that the type of the value
 *  actually is 32-bit int. See also xon_obj_reader_type.
 */
int32_t xon_obj_reader_get_int32_pos(xon_obj_reader reader, int pos);
/*! @brief Like xon_obj_reader_get_int32_pos, but with key instead of position */
int32_t xon_obj_reader_get_int32_key(xon_obj_reader reader, const char *key);


/*! @brief Get the 64-bit int at position pos
 *
 *  @param reader the reader (deserializer)
 *  @param pos the position of the entry
 * 
 *  It is your responsibility to ensure that the type of the value
 *  actually is 64-bit int. See also xon_obj_reader_type.
 */
int64_t xon_obj_reader_get_int64_pos(xon_obj_reader reader, int pos);
/*! @brief Like xon_obj_reader_get_int64_pos, but with key instead of position */
int64_t xon_obj_reader_get_int64_key(xon_obj_reader reader, const char *key);


/*! @brief Get the boolean at position pos
 *
 *  @param reader the reader (deserializer)
 *  @param pos the position of the entry
 * 
 *  It is your responsibility to ensure that the type of the value
 *  actually is boolean. See also xon_obj_reader_type.
 */
bool xon_obj_reader_get_bool_pos(xon_obj_reader reader, int pos);
/*! @brief Like xon_obj_reader_get_bool_pos, but with key instead of position */
bool xon_obj_reader_get_bool_key(xon_obj_reader reader, const char *key);



/*! @brief Destroy the reader
 *
 *  @param reader the reader (deserializer)
 */
void xon_obj_reader_delete(xon_obj_reader reader);







/*******************************************************
 *
 * Object 
 *
 *******************************************************/

/*! @brief Test whether achitecture matches
 *
 *  @param obj the binary object
 *  @return true if the binary object is either architecture
 *          independent or of the host architecture.
 */
bool xon_obj_verify_architecture(xon_obj obj);


/*! @brief Test whether object is BSON
 *
 *  @param obj the binary object
 *  @return true if the binary object is BSON
 */
bool xon_obj_is_bson(xon_obj obj);


/*! @brief Return the size
 *
 *  @param obj the binary object
 *  @return the size of obj in bytes, including possible padding
 *          at the end.
 */
size_t xon_obj_size(xon_obj obj);


/*! @brief Print obj to stdout
 *
 *  @param obj the binary object
 */
void xon_obj_print(xon_obj obj);


/*! @brief Debug print obj to stdout
 *
 *  @param obj the binary object
 */
void xon_obj_hexdump(xon_obj obj);


/*! @brief Print obj to stdout
 *
 *  @param obj the binary object
 *  @return the C string representation. It is your responsibity to
 *          free it.
 */
char* xon_obj_string(xon_obj obj);


/*! @brief Print obj to stdout with indent
 *
 *  @param obj the binary object
 *  @param address whether to start each line with the hex address
 *  @param prefix a string to print between the address and the obj data
 *  @return the C string representation. It is your responsibity to
 *          free it.
 */
char* xon_obj_string_indent(xon_obj obj, bool address, const char *prefix);



NAMESPACE_XON_C_API_END

#endif /* XON__OBJECT__H */

