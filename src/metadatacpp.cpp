#include "metadatacpp.h"

CMetaData::~CMetaData(){}

CMetaData* CreateMetaData()
{
    return new CMetaDataRaw();
}

void DestroyMetaData(CMetaData* pCMetaData)
{
    if(pCMetaData) 
    {
        delete pCMetaData;pCMetaData = nullptr;
    }
}

CMetaDataRaw::CMetaDataRaw(/* args */)
{
    m = METADATA::init_metadata();
}

CMetaDataRaw::CMetaDataRaw(METADATA::metadata_t* mm)
{
    m = METADATA::copy_metadata(mm);
}

CMetaDataRaw::~CMetaDataRaw()
{
    METADATA::delete_metadata(m);
}

METADATA::metadata_t* CMetaDataRaw::get_metadata()
{
    return m;
}

int32_t CMetaDataRaw::get_metadata_entry_count()
{
    return METADATA::get_metadata_entry_count(m);
}
    
int CMetaDataRaw::add_metadata_entry(uint32_t tag,
        const void *data,
        uint32_t data_count,
        uint32_t object_size)
{
    return METADATA::add_metadata_entry(m, tag, data, data_count, object_size);
}

int CMetaDataRaw::update_metadata_entry(const METADATA::metadata_entry_t& entry)
{
    return METADATA::update_metadata_entry(m, entry);
}

int CMetaDataRaw::find_metadata_entry_name(const char* name,
                METADATA::metadata_entry_t& entry)
{
    return METADATA::find_metadata_entry_name(m, name, entry);
}

int CMetaDataRaw::find_metadata_entry(uint32_t tag,
        METADATA::metadata_entry_t& entry)
{
    return METADATA::find_metadata_entry(m, tag, entry);
}

int CMetaDataRaw::merge_metadata(CMetaData* src)
{
    return METADATA::merge_metadata(m, ((CMetaDataRaw*)src)->get_metadata());
}

CMetaData* CMetaDataRaw::copy_metadata()
{   
    return new CMetaDataRaw(m);
}

int CMetaDataRaw::delete_metadata_entry(uint32_t tag)
{
    return METADATA::delete_metadata_entry(m, tag);
}

int CMetaDataRaw::get_local_metadata_tag_type(uint32_t tag)
{
    return METADATA::get_local_metadata_tag_type(tag, m);
}

size_t CMetaDataRaw::calculate_metadata_entry_malloc_size(uint8_t type,
        size_t data_count)
{
    return METADATA::calculate_metadata_entry_malloc_size(type, data_count);
}

int CMetaDataRaw::dump_metadata(std::string& str)
{
    return METADATA::dump_metadata(m, str);
}