#ifndef XON_OBJECT__H
#define XON_OBJECT__H

#ifdef __cplusplus
extern "C" {
#endif


enum XON_STATUS {
  XON_OK = 0,
  XON_ERROR_MALLOC = 1,
};


typedef void* xon_obj;

  
/* http://bsonspec.org/#/specification */

#define XON_ELEMENT_FLOATING_POINT           0x01
#define XON_ELEMENT_STRING                   0x02
#define XON_ELEMENT_DOCUMENT                 0x03
#define XON_ELEMENT_ARRAY                    0x04
#define XON_ELEMENT_BINARY                   0x05
#define XON_ELEMENT_UNDEF_DEPRECATED         0x06
#define XON_ELEMENT_OBJECT_ID                0x07
#define XON_ELEMENT_FALSE                    0x08
#define XON_ELEMENT_TRUE                     0x09
#define XON_ELEMENT_UTC_DATETIME             0x0a
#define XON_ELEMENT_NULL                     0x0b
#define XON_ELEMENT_REGEX                    0x0c
#define XON_ELEMENT_DBPOINTER_DEPRECATED     0x0d
#define XON_ELEMENT_JAVASCRIPT               0x0e
#define XON_ELEMENT_JAVASCRIPT_SCOPE         0x0f
#define XON_ELEMENT_INT32                    0x10
#define XON_ELEMENT_INT64                    0x11
#define XON_ELEMENT_TIME                     0x12
#define XON_ELEMENT_MIN_KEY                  0xff
#define XON_ELEMENT_MAX_KEY                  0x7f





typedef struct {
  /*! The current buffer, can change if we need to realloc. */
  char* buf;
  /*! The pointer past the end (plus padding) of the currently used
   *  part of buf. It is always aligned. */
  char* end;
  /*! Size allocated for buf */
  size_t capacity;
  /*! Whether buf should be freed */
  bool free_buffer;

  /*! Users can specify an initial buffer, which is used for small objects */
  char* initial_buf;
  /*! The size allocated for initial_buf */
  size_t initial_capacity;
  /*! Whether initial_buf should be freed */
  bool free_initial_buffer;
} xon_obj_builder_struct;


typedef xon_obj_builder_struct *xon_obj_builder;


xon_obj_builder xon_obj_builder_new();

xon_obj_builder xon_obj_builder_new_with_buf
(void *initial_buffer, size_t size, bool free_buffer);

void xon_obj_builder_free(xon_obj_builder builder);

enum XON_STATUS 
xon_obj_builder_add_string
(xon_obj_builder builder, const char *key, const char *value);

xon_obj xon_obj_builder_finish(xon_obj_builder builder);



void xon_obj_print(xon_obj obj);




#ifdef __cplusplus
}
#endif

#endif /* XON_OBJECT__H */
