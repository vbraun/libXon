#ifndef XON_OBJECT__H
#define XON_OBJECT__H

#ifdef __cplusplus
extern "C" {
#endif


enum XON_STATUS {
  XON_OK = 0
};


typedef void* xon_obj;


typedef struct {
  char* buf;
  char* end;
  size_t capacity;
  bool free_buffer;
} xon_obj_builder_struct;


typedef xon_obj_builder_struct *xon_obj_builder;


xon_obj_builder xon_obj_builder_new();

xon_obj_builder xon_obj_builder_new_with_buf
(void *initial_buffer, size_t size, bool free_buffer);

void xon_obj_builder_free(xon_obj_builder builder);

enum XON_STATUS 
xon_obj_builder_add
(xon_obj_builder builder, const char *key, const char *value);

xon_obj xon_obj_builder_finish(xon_obj_builder builder);



void xon_obj_print(xon_obj obj);




#ifdef __cplusplus
}
#endif

#endif /* XON_OBJECT__H */
