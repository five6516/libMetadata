/*
 *
 * author five6516@126.com
 * 
 * Main Header
 *
 */

/**
 * !! Do not include this file directly !!
 *
 * Include metadata.h instead.
 */

#ifndef _INCLUDE_METADATA_VENDOR_TAGS_H
#define _INCLUDE_METADATA_VENDOR_TAGS_H

#include <stdint.h>
/** TODO: Nearly every enum in this file needs a description */

/** Tag information */
typedef struct tag_info {
    const char *tag_name;
    uint8_t     tag_type;
} tag_info_t;

typedef enum metadata_section {
    METADATA_MAIN,
    METADATA_MAIN1,
    METADATA_MAIN2,
    METADATA_MAIN3,
    METADATA_MAIN4,
    METADATA_MAIN5,
    METADATA_SECTION_COUNT,
} metadata_section_t;

/**
 * Enumeration definitions for the various entries that need them
 */

// METADATA_MAIN2_VALUE1
typedef enum metadata_enum_value1_mode {
    METADATA_MAIN_VALUE1_MODE1                              ,
    METADATA_MAIN_VALUE1_MODE2                              ,
    METADATA_MAIN_VALUE1_MODE3                              ,
} metadata_enum_value1_mode_t;

#endif