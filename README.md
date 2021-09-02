# libMetadata

#### 介绍
元数据存储，用于全局数据管理，类似AOSP  libcamera_metadata

#### 软件架构
1. 使用cmake构建编译
2. 要求c++17
3. 适用于window和linux的动态库
4. 支持存储类型
    - int8_t
    - int32_t
    - float
    - int64_t
    - double
    - 自定义数据

#### 编译说明

1. cd libMetadata
2. mkdir build; cd build
3. cmake ..
4. make 

#### 使用说明

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

- 类调用方式
```c++
//包含头文件 metadata.h
#include <metadata.h>

//获取类指针
CMetaData* pMetadata = CreateMetaData();

//增加数据
int32_t a = 123;
pMetadata->add_metadata_entry(METADATA_MAIN1, &a, 1);

//查询数据
metadata_entry_t entry;
pMetadata->find_metadata_entry(METADATA_MAIN1, entry);
printf("%d",entry.data.i32[0]);

//释放
DestroyMetaData(pMetadata);
```

具体可参考 libMetadata/test/test.cpp

- 数据节点的添加
`metadata_tag.h`中自行修改`metadata_section`结构体
`metadata_tag_info.cpp`中自行修改`metadata_section_bounds`,注意对应