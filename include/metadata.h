#ifndef _INCLUDE_METADATA_H_
#define _INCLUDE_METADATA_H_

#include <stdint.h>
#include <string>
#include <metadata_tag.h>

/* Windows */
#if   defined (WIN32)		\
   || defined (_WIN32)		\
   || defined (_WIN64)		\
   || defined (__WIN32__)	\
   || defined (__TOS_WIN__)	\
   || defined (__WINDOWS__)
#define OS_WINDOWS

#if defined (_WIN64)
/* Win64 */
#define OS_WIN64   
#else 
/* Win32 */
#define OS_WIN32
#endif

/* Linux */
#elif defined(linux) \
 || defined(__linux) \
 || defined(__linux__)
#define OS_LINUX
#endif

#if defined(OS_LINUX)
 #define METADATA_API __attribute__((visibility("default")))
#else

#ifndef _INCLUDE_METADATA_H
 #define METADATA_API _declspec(dllimport)
#else
 #define METADATA_API _declspec(dllexport)
#endif
#endif

#define METADATA_OK              0
#define METADATA_ERROR           -1

/**
 * Type definitions for metadata_entry
 * =============================================================================
 */
enum {
    // Unsigned 8-bit integer (uint8_t)
    TYPE_BYTE = 0,
    // Signed 32-bit integer (int32_t)
    TYPE_INT32 = 1,
    // 32-bit float (float)
    TYPE_FLOAT = 2,
    // Signed 64-bit integer (int64_t)
    TYPE_INT64 = 3,
    // 64-bit float (double)
    TYPE_DOUBLE = 4,
    // A 64-bit fraction (metadata_rational_t)
    TYPE_OBJECT = 5,
    // Number of type fields
    NUM_TYPES
};

/**
 * A reference to a metadata entry in a buffer.
 *
 * The data union pointers point to the real data in the buffer, and can be
 * modified in-place if the count does not need to change. The count is the
 * number of entries in data of the entry's type, not a count of bytes.
 */
typedef struct metadata_entry {
    uint32_t tag;
    uint8_t  type;
    uint32_t   count;
    union {
        uint8_t *u8;
        int32_t *i32;
        float   *f;
        int64_t *i64;
        double  *d;
        void *r;
    } data;
} metadata_entry_t;

/* a packet of metadata */
struct metadata;
typedef struct metadata metadata_t;

/* init metadata must be called first */
METADATA_API
metadata* init_metadata();

/* delete metadata must be called last */
METADATA_API
int delete_metadata(metadata_t* src);

/* check if metadata is null */
METADATA_API
int is_valid_metadata(metadata_t* src);

/* get entry count */
METADATA_API
int32_t get_metadata_entry_count(metadata_t* src);

/* add new  entry, if has exist, will return error
   if add a object, must set param "object_size"     
*/
METADATA_API
int add_metadata_entry(metadata_t* dst,
        uint32_t tag,
        const void *data,
        uint32_t data_count,
        uint32_t object_size = 0/*only obejct use*/);

/* updata metada entry, you should find entry first,then modify entry */
METADATA_API
int update_metadata_entry(metadata_t* src,
        const metadata_entry_t& entry);

/* updata metada entry */
METADATA_API
int find_metadata_entry(metadata_t* src,
                const char* name,
                metadata_entry_t& entry);

METADATA_API
int find_metadata_entry(metadata_t* src,
        uint32_t tag,
        metadata_entry_t& entry);

/* merge metada if same entry has exist in dst,it will be Covered by src */
METADATA_API
int merge_metadata(metadata_t* dst,
        metadata_t* src);

/* copy all data from src */
METADATA_API
metadata_t* copy_metadata(metadata_t* src);

/* delete metada entry */
METADATA_API
int delete_metadata_entry(metadata_t* src,
        uint32_t tag);

/* get metada tag data type */
METADATA_API
int get_local_metadata_tag_type(uint32_t tag, 
        const metadata_t* meta);

/* calculate metadata entry size */
METADATA_API
size_t calculate_metadata_entry_malloc_size(uint8_t type,
        size_t data_count);

/* dump metadata export json format*/
METADATA_API
int dump_metadata(metadata_t* src,std::string& str);

#endif