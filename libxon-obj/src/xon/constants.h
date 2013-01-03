#ifndef XON_CONSTANTS__H
#define XON_CONSTANTS__H



/* http://bsonspec.org/#/specification */

#define XON_ELEMENT_DOUBLE                   0x01  /*!< @brief floating point */
#define XON_ELEMENT_STRING                   0x02  /*!< @brief UTF-8 string */
#define XON_ELEMENT_DOCUMENT                 0x03  /*!< @brief embedded binary object */
/*! @brief array
 * 
 *  As in BSON, an array is a binary object whose keys are stringified
 *  integers in ascending numerical order. For example, the array
 *  ['red', 'blue'] would be encoded as the document {'0': 'red', '1':
 *  'blue'}.
 */
#define XON_ELEMENT_ARRAY                    0x04
#define XON_ELEMENT_BINARY                   0x05  /*!< @brief binary data */
#define XON_ELEMENT_UNDEF_DEPRECATED         0x06  /*!< @brief Deprecated */
#define XON_ELEMENT_OBJECT_ID                0x07  /*!< @brief MongoDB ObjectID */
#define XON_ELEMENT_BOOLEAN                  0x08  /*!< @brief boolean */
/*! @brief UTC datetime 
 *  
 *  An int64_t containing the milliseconds since the Unix epoch
 */
#define XON_ELEMENT_UTC_DATETIME             0x09
#define XON_ELEMENT_NULL                     0x0a  /*!< @brief NULL */
/*! @brief Regular expression
 *
 *  As in BSON: the first cstring is the regex pattern, the second is
 *  the regex options string.
 */
#define XON_ELEMENT_REGEX                    0x0b
#define XON_ELEMENT_DBPOINTER_DEPRECATED     0x0c  /*!< @brief Deprecated */
#define XON_ELEMENT_JAVASCRIPT               0x0d  /*!< @brief Javascript code */
#define XON_ELEMENT_SYMBOL_DEPRECATED        0x0e  /*!< @brief Deprecated */
#define XON_ELEMENT_JAVASCRIPT_SCOPE         0x0f  /*!< @brief Javascript code with scope */
#define XON_ELEMENT_INT32                    0x10  /*!< @brief An int32_t integer */
#define XON_ELEMENT_INT64                    0x12  /*!< @brief An int64_t integer */ 
#define XON_ELEMENT_TIME                     0x13  /*!< @brief MongoDB timestamp */
#define XON_ELEMENT_MIN_KEY                  0xff  /*!< @brief BSON min key */
#define XON_ELEMENT_MAX_KEY                  0x7f  /*!< @brief BSON max key */



#endif /* XON_CONSTANTS__H */
