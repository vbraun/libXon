#ifndef XON__BUFFER__H
#define XON__BUFFER__H

/** @file */ 

#include <stdbool.h>
#include <xon/status.h>


/*! @brief The type of a buffer
 *
 *  This opaque struct contains the implementation details of the
 *  buffer. It is declared in buffer.c but you should never have to
 *  use the actual implementation.
 */
typedef struct buffer_t *buffer_t;



/*! @brief Interface for buffer
 * 
 *  This declares the interface for the buffer that is used
 *  internally in the xon library.
 */
typedef struct {

  /*! @brief Create new buffer
   */
  buffer_t (*new)();

  /*! @brief Create new buffer with an initial memory area
   *
   *  @param initial the initial buffer
   *  @param size the size in bytes of the initial buffer
   *  @param needs_free whether to free() the initial buffer in
   *         buffer.delete
   *
   *  The initial buffer will be used for small objects. 
   */
  buffer_t (*new_with_initial)(void *initial, size_t size, bool needs_free);

  /*! @brief Destroy buffer
   *
   *  @param buffer the buffer to destroy
   *
   *  You must call this function on every buffer that you created, or
   *  you will leak memory. 
   * 
   *  buffer may be NULL, in which case nothing is done.
   */
  void (*delete)(buffer_t buffer);

  /*! @brief Append to buffer
   *
   *  @param buffer the buffer to append to
   *  @param data the data to append
   *  @param size the size of the data in bytes
   *  @return XON_OK if successful, XON_ERROR_MALLOC if buffer needed
   *          more memory but malloc failed.
   */
  xon_status (*append)(buffer_t buffer, void* data, size_t size);

  /*! @brief Fill buffer with a character
   *
   *  @param buffer the buffer to fill
   *  @param value the byte to fill the buffer with
   *  @param size how many bytes to append
   *  @return XON_OK if successful, XON_ERROR_MALLOC if buffer needed
   *          more memory but malloc failed.
   */
  xon_status (*fill)(buffer_t buffer, char value, size_t size);

  /*! @brief Start of buffer
   *
   *  @param buffer the buffer
   *  @return a char pointer to the beginning of the buffer
   * 
   *  It is safe to read/write from begin (inclusive) to
   *  begin+capacity (exclusive).
   */
  char* (*begin)(buffer_t buffer);

  /*! @brief Past the end of buffer
   *
   *  @param buffer the buffer
   *  @return a char pointer to the first byte past the end
   * 
   *  It is safe to read/write past the end up to end+capacity-length
   *  (exclusive).
   */
  char* (*end)(buffer_t buffer);

  /*! @brief Construct a memory object from the buffer and reset
   *
   *  @param buffer the buffer
   *  @return a pointer to a newly malloc'ed area of length
   *          bytes. NULL if malloc failed.
   * 
   *  The buffer is reset and empty after calling this method.
   */
  char* (*get)(buffer_t buffer);

  /*! @brief Reset buffer
   *
   *  @param buffer the buffer
   *
   *  This is like get but the content of the buffer is not copied & returned.
   */
  void (*reset)(buffer_t buffer);

  /*! @brief The current size of the buffer
   *   
   *  @param buffer the buffer
   *  @return how many bytes are currently stored in the buffer
   *
   *  The buffer might have allocated additional space so you can
   *  write past the end, see capacity. This function returns
   *  end-begin.
   */
  size_t (*size)(buffer_t buffer);

  /*! @brief The current capacity of the buffer
   *   
   *  @param buffer the buffer
   *  @return how many bytes are available to read/write
   *
   *  It is safe to read/write from begin to begin+capacity. There
   *  might be less bytes actually stored in the buffer. See size.
   */
  size_t (*capacity)(buffer_t buffer);

  /*! @brief Ensure that the buffer has a certain capacity
   *   
   *  @param buffer the buffer
   *  @param new_capacity the minimum capacity desired, in bytes.
   *  @return XON_OK if resizing the buffer succeeded.
   *
   *  This function ensures that capacity is larger or equal than new_size.
   */
  xon_status (*ensure_total)(buffer_t buffer, size_t new_capacity);

  /*! @brief Ensure that the buffer has space past the end
   *   
   *  @param buffer the buffer
   *  @param new_size the minimum space (in bytes) past the end of the buffer
   *  @return XON_OK if resizing the buffer succeeded.
   *
   *  This function ensures that capacity-size is larger or equal than extra.
   */
  xon_status (*ensure_additional)(buffer_t buffer, size_t extra);
} buffer_interface;


/*! @brief The buffer interface
 *
 * This struct implements a buffer and is used in various places in
 * the xon library. To use it, call the function pointers defined in
 * it. See buffer_interface for details.
 */
extern const buffer_interface buffer;


#endif /* BUFFER__H */
