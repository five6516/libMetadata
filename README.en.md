# libMetadata

#### Description
元数据存储，用于全局数据管理，类似AOSP  libcamera_metadata

#### Software Architecture
1. Use CMAKE build compilation
2. Require C ++ 17
3. Dynamic libraries for Window and Linux
4. Support storage type
    - INT8_T
    - INT32_T
    - Float
    - INT64_T
    - Double
    - Custom data

#### Compile note

1. cd libMetadata
2. mkdir build; cd build
3. cmake ..
4. make

#### Instructions for use

- c接口调用方式
```c++
//包含头文件 metadata.h
#include <metadata.h>

//声明命名空间
using namespace METADATA;

//初始化
metadata_t*  m = init_metadata();

//增加数据
int32_t a = 123;
add_metadata_entry(m, METADATA_MAIN1, &a, 1);

//查询数据
metadata_entry_t entry;
find_metadata_entry(m, METADATA_MAIN1, entry);
printf("%d",entry.data.i32[0]);

//释放
delete_metadata(m)
```

- Class call mode
```c++
//Contains header files metadata.h
#include <metadata.h>

//Get class pointer
CMetaData* pMetadata = CreateMetaData();

//Increasing data
int32_t a = 123;
pMetadata->add_metadata_entry(METADATA_MAIN1, &a, 1);

//Query data
metadata_entry_t entry;
pMetadata->find_metadata_entry(METADATA_MAIN1, entry);
printf("%d",entry.data.i32[0]);

//free
DestroyMetaData(pMetadata);
```

Specific reference libMetadata/test/test.cpp

- Add data node
`metadata_tag.h`Self-modification`metadata_section` struct
`metadata_tag_info.cpp`Self-modification`metadata_section_bounds`,Pay attention