#include <metadata.h>
#include <stdio.h>
#include <string>
#include <nlohmann/json.hpp>
#include <format.h>

#include <limits.h>

namespace METADATA{

extern tag_info_t metadata_section_bounds[METADATA_SECTION_COUNT];

/* all data pointer */
struct metadata {
    uint32_t          size;
    uint32_t          version;
    uint32_t          flags;
    uint32_t          entry_count;
    nlohmann::json    json;
};

/* entry data only using private */
typedef struct metadata_buffer_entry {
    uint32_t tag;
    uint32_t count;
    size_t malloc_size;
    union {
        uint8_t* pdata ;
        uint8_t  value[4];
    } data;
    uint8_t  type;
}metadata_buffer_entry_t;

const size_t metadata_type_size[NUM_TYPES] = {
    sizeof(uint8_t),
    sizeof(int32_t),
    sizeof(float),
    sizeof(int64_t),
    sizeof(double),
    INT_MAX
};

const char *metadata_type_names[NUM_TYPES] = {
    "byte",
    "int32",
    "float",
    "int64",
    "double",
    "object"
};

char last_error[256];
static void set_last_error(const std::string& error)
{
    sprintf(last_error,"%s",error.c_str());
}

static uint8_t* get_data_from_addr(const std::string& addr)
{
    uint64_t add_int64 = std::strtoull(addr.c_str(), 0, 16);

    return (uint8_t*)add_int64;
}

static metadata_buffer_entry_t* get_new_metadata_buffer_entry()
{
    return new metadata_buffer_entry_t;
}

static void free_metadata_buffer_entry(metadata_buffer_entry_t* buffer)
{ 
    if(buffer)
    {
        if(buffer->malloc_size > 0 && buffer->data.pdata)
        {
            delete[] buffer->data.pdata;
            buffer->data.pdata = nullptr;
        }
        delete buffer;buffer = nullptr;
    }
}

metadata* init_metadata()
{
    metadata* m = new metadata();
    if(!m)
        return nullptr;

    m->version = 0;
    m->flags = 0;
    m->size = sizeof(metadata);
    m->entry_count = 0;

    return m;
}

int delete_metadata(metadata_t* src)
{
    if(!src)
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    int delete_index = 0;
    for (auto& [key, val] : src->json.items())
    {
        //std::cout << "key: " << key << ", value:" << val << '\n';
        if(!val.is_string())
            continue;

        metadata_buffer_entry_t* buffer = reinterpret_cast<metadata_buffer_entry_t*>(get_data_from_addr(val.get<std::string>())); 
        if(buffer)
        {
            free_metadata_buffer_entry(buffer);
            delete_index ++;
        }
    }

    if(src->entry_count == delete_index)
    {
        delete src;
        src = nullptr;
        return METADATA_OK;
    }

    set_last_error(format("metadata_t delete error"));    
    delete src;
    src = nullptr;
    return METADATA_ERROR;
}

int is_valid_metadata(metadata_t* src)
{
    int a = src->entry_count;
    return src == nullptr ? METADATA_ERROR:METADATA_OK;
}

int32_t get_metadata_entry_count(metadata_t* src)
{
    if (src == nullptr)
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    return src->entry_count;
}

int add_metadata_entry(metadata_t* dst,
        uint32_t tag,
        const void *data,
        uint32_t data_count,
        uint32_t object_size) 
{
    if(!dst)
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    int type = get_local_metadata_tag_type(tag, dst);
    if (type == METADATA_ERROR) {
        return METADATA_ERROR;
    }
    metadata_entry_t entry;
    if(METADATA_OK == find_metadata_entry(dst, tag, entry))
    {
        set_last_error(format("tag has exits"));
        return METADATA_ERROR;
    }

    if (dst == nullptr)
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    } 
    if (data_count && data == NULL) 
    {
        set_last_error(format("data is null"));
        return METADATA_ERROR;
    }

    if (tag >= METADATA_SECTION_COUNT) 
    {
        set_last_error(format("tag out of METADATA_SECTION_COUNT"));
        return METADATA_ERROR;
    }

    metadata_buffer_entry_t* buffer = get_new_metadata_buffer_entry();
    if(!buffer)
    {
        set_last_error(format("malloc metadata_entry_t fail"));
        return METADATA_ERROR;
    }

    size_t malloc_size = calculate_metadata_entry_malloc_size(type,data_count);
    if(type == TYPE_OBJECT)
        malloc_size = static_cast<int32_t>(object_size)*data_count;

    buffer->count = data_count;
    buffer->tag = tag;
    buffer->type = type;
    buffer->malloc_size = malloc_size;
    
    if(malloc_size == 0)//use fix array
        memcpy(buffer->data.value, data, data_count * metadata_type_size[type]);
    else
    {
        buffer->data.pdata = new uint8_t[malloc_size];
        memcpy(buffer->data.pdata, data, malloc_size);
    }
    
    //save object addr
    dst->json[metadata_section_bounds[tag].tag_name] = format("%p",buffer);

    dst->entry_count ++;

    return METADATA_OK;
}

int update_metadata_entry(metadata_t* src,
        const metadata_entry_t& entry) 
{
    if(!src)
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    if (src == nullptr)
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    if(!src->json[metadata_section_bounds[entry.tag].tag_name].is_string())
    {
        set_last_error(format("tag %ud not exist",entry.tag));
        return METADATA_ERROR;
    }

    //get addr
    std::string add_str = src->json[metadata_section_bounds[entry.tag].tag_name].get<std::string>();

    metadata_buffer_entry_t* buffer = reinterpret_cast<metadata_buffer_entry_t*>(get_data_from_addr(add_str));  
    if(!buffer)
    {
        set_last_error(format("metadata_entry_t is null"));
        return METADATA_ERROR;
    }

    size_t malloc_size = calculate_metadata_entry_malloc_size(entry.type,entry.count);
    if(entry.type == TYPE_OBJECT)
        malloc_size = static_cast<int32_t>(buffer->malloc_size/buffer->count)*entry.count;

    if(entry.count != buffer->count && malloc_size != 0)
    {
        delete[] buffer->data.pdata;buffer->data.pdata = nullptr;
        buffer->data.pdata = new uint8_t[malloc_size];
    } 

    buffer->count = entry.count;
    buffer->tag = entry.tag;
    buffer->type = entry.type;
    buffer->malloc_size = malloc_size;
    
    if(malloc_size == 0)//use fix array
        memcpy(buffer->data.value, entry.data.u8, entry.count * metadata_type_size[entry.type]);
    else
    {
        memcpy(buffer->data.pdata, entry.data.u8, malloc_size);
    }
    
    return METADATA_OK;
}

int find_metadata_entry_name(metadata_t* src,
                const char* name,
                metadata_entry_t& entry)
{
    if(!src->json[name].is_string())
    {
        set_last_error(format("name %s not exist",name));
        return METADATA_ERROR;
    }

    //get addr
    std::string add_str = src->json[name].get<std::string>();

    metadata_buffer_entry_t* buffer = reinterpret_cast<metadata_buffer_entry_t*>(get_data_from_addr(add_str));  

    entry.tag = buffer->tag;
    entry.type = buffer->type;
    entry.count = buffer->count;
    if(buffer->malloc_size == 0)
        entry.data.u8 = buffer->data.value;
    else
        entry.data.u8 = buffer->data.pdata;
    
    return METADATA_OK;
}


int find_metadata_entry(metadata_t* src,
        uint32_t tag,
        metadata_entry_t& entry) {
    if (src == NULL) 
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    if (tag >= METADATA_SECTION_COUNT) 
    {
        set_last_error(format("tag out of METADATA_SECTION_COUNT"));
        return METADATA_ERROR;
    }

    int type = get_local_metadata_tag_type(tag, src);
    if (type == METADATA_ERROR) 
    {
        return METADATA_ERROR;
    }

    return find_metadata_entry_name(src,
                metadata_section_bounds[tag].tag_name,
                entry);
}

int delete_metadata_entry(metadata_t* src,
        uint32_t tag)
{
    if (src == NULL) 
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    if (tag >= METADATA_SECTION_COUNT) {
        set_last_error(format("tag out of METADATA_SECTION_COUNT"));
        return METADATA_ERROR;
    }

    int type = get_local_metadata_tag_type(tag, src);
    if (type == METADATA_ERROR) {
        return METADATA_ERROR;
    }

    if(!src->json[metadata_section_bounds[tag].tag_name].is_string())
    {
        set_last_error(format("tag %ud not exist",tag));
        return METADATA_ERROR;
    }

    metadata_buffer_entry_t* buffer = reinterpret_cast<metadata_buffer_entry_t*>(get_data_from_addr(src->json[metadata_section_bounds[tag].tag_name].get<std::string>())); 
    free_metadata_buffer_entry(buffer);
    
    src->json.erase(metadata_section_bounds[tag].tag_name);
    src->entry_count --;

    return METADATA_OK;
}

int get_local_metadata_tag_type(uint32_t tag, const metadata_t* meta) 
{
    if (meta == NULL) 
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    if (tag >= METADATA_SECTION_COUNT) {
        set_last_error(format("tag out of METADATA_SECTION_COUNT"));
        return METADATA_ERROR;
    }
    return metadata_section_bounds[tag].tag_type;
}

size_t calculate_metadata_entry_malloc_size(uint8_t type,
        size_t data_count) {
    if (type >= NUM_TYPES) return 0;

    size_t data_bytes = data_count *
            metadata_type_size[type];

    return data_bytes <= 4 ? 0 : data_bytes + 1;
}

int merge_metadata(metadata_t* dst,
        metadata_t* src)
{
    if(!src || !dst)
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    for (auto& [key, val] : src->json.items())
    {
        //std::cout << "key: " << key << ", value:" << val << '\n';
        if(!val.is_string())
            continue;

        metadata_buffer_entry_t* buffer = reinterpret_cast<metadata_buffer_entry_t*>(get_data_from_addr(val.get<std::string>())); 
        if(buffer)
        {
            metadata_entry_t entry;
            entry.tag = buffer->tag;
            entry.type = buffer->type;
            entry.count = buffer->count;
            if(buffer->malloc_size == 0)
                entry.data.u8 = buffer->data.value;
            else
                entry.data.u8 = buffer->data.pdata;

            metadata_entry_t entry_r;
            if(METADATA_OK == find_metadata_entry(dst, buffer->tag, entry_r))
            {
                if(METADATA_OK != update_metadata_entry(dst,entry))
                {
                    return METADATA_ERROR; 
                }
            }
            else
            {
                if(METADATA_OK != add_metadata_entry(dst,
                                    buffer->tag,
                                    buffer->malloc_size == 0?buffer->data.value:buffer->data.pdata,
                                    buffer->count,
                                    buffer->malloc_size/buffer->count))
                {
                    return METADATA_ERROR; 
                }
            }
        }
    }

    return METADATA_OK;
}

metadata_t* copy_metadata(metadata_t* src)
{
    if(!src)
    {
        set_last_error(format("metadata_t is null"));
        return nullptr;
    }

    metadata_t* dst = init_metadata();
    if(!dst)
    {
        set_last_error(format("init metadata_t is null"));
        return nullptr;
    }

    for (auto& [key, val] : src->json.items())
    {
        //std::cout << "key: " << key << ", value:" << val << '\n';
        if(!val.is_string())
            continue;

        metadata_buffer_entry_t* buffer = reinterpret_cast<metadata_buffer_entry_t*>(get_data_from_addr(val.get<std::string>())); 
        if(buffer)
        {
            metadata_entry_t entry;
            entry.tag = buffer->tag;
            entry.type = buffer->type;
            entry.count = buffer->count;
            if(buffer->malloc_size == 0)
                entry.data.u8 = buffer->data.value;
            else
                entry.data.u8 = buffer->data.pdata;

            if(METADATA_OK != add_metadata_entry(dst,
                                    buffer->tag,
                                    buffer->malloc_size == 0?buffer->data.value:buffer->data.pdata,
                                    buffer->count,
                                    buffer->malloc_size/buffer->count))
            {
                return nullptr; 
            }
        }
    }

    return dst;
}

/* todo: show string value */
int dump_metadata(metadata_t* src,std::string& str)
{
    if(!src)
    {
        set_last_error(format("metadata_t is null"));
        return METADATA_ERROR;
    }

    nlohmann::json j;

    for (auto& [key, val] : src->json.items())
    {
        //std::cout << "key: " << key << ", value:" << val << '\n';
        if(!val.is_string())
            continue;

        metadata_buffer_entry_t* buffer = reinterpret_cast<metadata_buffer_entry_t*>(get_data_from_addr(val.get<std::string>())); 
        if(buffer)
        {
            int32_t malloc_size = buffer->malloc_size == 0?sizeof(buffer->data.value):buffer->malloc_size;
            int8_t* data = new int8_t[malloc_size];
            if(!data) break;

            if(buffer->malloc_size == 0)
                    memcpy(data,buffer->data.value,malloc_size);
                else
                    memcpy(data,buffer->data.pdata,malloc_size);

            switch (buffer->type)
            {
            case TYPE_BYTE:
            {   
                for (size_t i = 0; i < buffer->count; i++)
                {
                    j[key][i] = ((int8_t*)data)[i];
                }
                break;
            }  
            case TYPE_INT32:
            {   
                for (size_t i = 0; i < buffer->count; i++)
                {
                    j[key][i] = ((int32_t*)data)[i];
                }
                break;
            } 
            case TYPE_FLOAT:
            {   
                for (size_t i = 0; i < buffer->count; i++)
                {
                    j[key][i] = ((float*)data)[i];
                }
                break;
            } 
            case TYPE_INT64:
            {   
                for (size_t i = 0; i < buffer->count; i++)
                {
                    j[key][i] = ((int64_t*)data)[i];
                }
                break;
            } 
            case TYPE_DOUBLE:
            {   
                for (size_t i = 0; i < buffer->count; i++)
                {
                    j[key][i] = ((double*)data)[i];
                }
                break;
            } 
            case TYPE_OBJECT:
            {   
                j[key] = val;
                
                break;
            } 
            default:
                break;
            }
            delete[] data;data=nullptr;
        }
    }

    str = j.dump();
    return METADATA_OK;
}

}