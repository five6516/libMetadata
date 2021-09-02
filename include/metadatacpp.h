#ifndef _INCLUDE_METADATACPP_H_
#define _INCLUDE_METADATACPP_H_

#include <metadata.h>

class CMetaDataRaw : public CMetaData
{
private:
    METADATA::metadata_t* m;

public:
    CMetaDataRaw(/* args */);
    CMetaDataRaw(METADATA::metadata_t* mm);
    ~CMetaDataRaw() ;

    METADATA::metadata_t* get_metadata();

    int32_t get_metadata_entry_count() override;
    
    int add_metadata_entry(uint32_t tag,
            const void *data,
            uint32_t data_count,
            uint32_t object_size = 0/*only obejct use*/) override;
    
    int update_metadata_entry(const METADATA::metadata_entry_t& entry) override;
    
    int find_metadata_entry_name(const char* name,
                    METADATA::metadata_entry_t& entry) override;
    
    int find_metadata_entry(uint32_t tag,
            METADATA::metadata_entry_t& entry) override;
    
    int merge_metadata(CMetaData* src) override;
    
    CMetaData* copy_metadata() override;
    
    int delete_metadata_entry(uint32_t tag) override;
    
    int get_local_metadata_tag_type(uint32_t tag) override;
    
    size_t calculate_metadata_entry_malloc_size(uint8_t type,
            size_t data_count) override;
    
    int dump_metadata(std::string& str) override;
};


#endif

