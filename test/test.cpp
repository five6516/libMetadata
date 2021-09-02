#include <iostream>
#include <metadata.h>
#include <format.h>

#if defined(OS_WINDOWS)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#define USE_DOC_TEST 1

#if USE_DOC_TEST
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include "doctest.h"
#else
#define TEST_CASE void test_case
void CHECK(bool result)
{
    if(!result)
        std::cout << "error\n"; 
}
#endif

using namespace METADATA;

class testClass
{
private:
    int m_a;
    float m_b;
public:
    void setA(int a){m_a = a;}
    void setB(float b){m_b = b;}
    int getA(){return m_a;}
    float getB(){return m_b;}
};

#if USE_DOC_TEST
TEST_CASE("test")
#else
void test()
#endif
{      
    /* example */
    {
        metadata_t*  m = init_metadata();
        CHECK(METADATA_OK == is_valid_metadata(m));

        int32_t a = 123;
        CHECK(add_metadata_entry(m, METADATA_MAIN1, &a, 1) == METADATA_OK);

        CHECK(add_metadata_entry(m, METADATA_MAIN1, &a, 1) != METADATA_OK);

        CHECK(get_metadata_entry_count(m) == 1);

        metadata_entry_t entry;
        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN1, entry));
        CHECK(entry.data.i32[0] == 123);

        entry.data.i32[0] = 234;
        CHECK(METADATA_OK == update_metadata_entry(m, entry));

        metadata_entry_t entry2;
        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN1, entry2));
        CHECK(entry2.data.i32[0] == 234);

        CHECK(METADATA_OK == delete_metadata_entry(m, METADATA_MAIN1));
        CHECK(METADATA_OK != find_metadata_entry(m, METADATA_MAIN1, entry));

        CHECK(get_metadata_entry_count(m) == 0);

        CHECK(METADATA_OK == delete_metadata(m));
    }

    /* add data test */
    {
        metadata_t* m = init_metadata();
        
        int8_t data_8 = 5;
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN, &data_8, 1));

        int32_t data_32 = 55;
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN1, &data_32, 1));

        float data_float = 555.5;
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN2, &data_float, 1));

        int64_t data_64 = 5555;
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN3, &data_64, 1));

        double data_double = 5555.55;
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN4, &data_double, 1));

        testClass testclass;
        testclass.setA(3);
        testclass.setB(3.33f);
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN5, &testclass, 1, sizeof(testClass)));

        std::string sss;
        CHECK(METADATA_OK == dump_metadata(m,sss));
        std::cout << sss << std::endl;

        metadata_t* m2 = copy_metadata(m);
        CHECK(METADATA_OK == dump_metadata(m2,sss));
        std::cout << sss << std::endl;

        metadata_entry_t entry;
        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN, entry));
        CHECK(entry.data.u8[0] == 5);

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN1, entry));
        CHECK(entry.data.i32[0] == 55);

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN2, entry));
        CHECK(entry.data.f[0] == 555.5);

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN3, entry));
        CHECK(entry.data.i64[0] == 5555);

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN4, entry));
        CHECK(entry.data.d[0] == 5555.55);

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN5, entry));
        testClass* ptestClass = reinterpret_cast<testClass*>(entry.data.r);
        CHECK(ptestClass->getA() == 3);
        CHECK(ptestClass->getB() == 3.33f);

        CHECK(get_metadata_entry_count(m) == 6);

        CHECK(METADATA_OK == delete_metadata(m));
        CHECK(METADATA_OK == delete_metadata(m2));
    }

    /* array test */
    {
        metadata_t* m = init_metadata();

        int8_t data_8[3] = {5,5,5};
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN, data_8, 3));

        int32_t data_32[3] = {55,55,55};
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN1, data_32, 3));

        float data_float[3] = {555.5,555.5,555.5};
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN2, data_float, 3));

        int64_t data_64[3] = {5555,5555,5555};
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN3, data_64, 3));

        double data_double[3] = {5555.55,5555.55,5555.55};
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN4, data_double, 3));

        testClass testclass[3];
        testclass[0].setA(3);
        testclass[0].setB(3.33f);
        testclass[1].setA(3);
        testclass[1].setB(3.33f);
        testclass[2].setA(3);
        testclass[2].setB(3.33f);
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN5, testclass, 3, sizeof(testClass)));

        std::string sss;
        CHECK(METADATA_OK == dump_metadata(m,sss));
        std::cout << sss << std::endl;

        metadata_entry_t entry;
        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN, entry));
        CHECK((entry.data.u8[0] == 5 && entry.data.u8[1] == 5 && entry.data.u8[2] == 5));

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN1, entry));
        CHECK((entry.data.i32[0] == 55 && entry.data.i32[1] == 55 && entry.data.i32[2] == 55));

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN2, entry));
        CHECK((entry.data.f[0] == 555.5 && entry.data.f[1] == 555.5 && entry.data.f[2] == 555.5));

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN3, entry));
        CHECK((entry.data.i64[0] == 5555 && entry.data.i64[1] == 5555 && entry.data.i64[2] == 5555));

        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN4, entry));
        CHECK((entry.data.d[0] == 5555.55 && entry.data.d[1] == 5555.55 && entry.data.d[2] == 5555.55));

        CHECK(METADATA_OK == delete_metadata(m));
    }

    //large data
    {
        metadata_t* m = init_metadata();
        
        int8_t* data_8 = new int8_t[1000];
        for (size_t i = 0; i < 1000; i++)
        {
            data_8[i] = 5;
        }
        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN, data_8, 1000));

        metadata_entry_t entry;
        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN, entry));
        for (size_t i = 0; i < 1000; i++)
        {
            CHECK(entry.data.u8[i] == 5);
        }

        std::string sss;
        CHECK(METADATA_OK == dump_metadata(m,sss));
        std::cout << sss << std::endl;
        
        delete[] data_8;
        CHECK(METADATA_OK == delete_metadata(m));
    }

    //string data
    {
        metadata_t* m = init_metadata();
        
        int8_t data_8[]{"hello , world"};

        CHECK(METADATA_OK == add_metadata_entry(m, METADATA_MAIN, data_8, sizeof(data_8)/sizeof(int8_t)));

        metadata_entry_t entry;
        CHECK(METADATA_OK == find_metadata_entry(m, METADATA_MAIN, entry));
        CHECK(0 == strcmp((const char*)data_8,(const char*)entry.data.u8));
        
        std::string sss;
        CHECK(METADATA_OK == dump_metadata(m,sss));
        std::cout << sss << std::endl;

        CHECK(METADATA_OK == delete_metadata(m));
    }

    //merge data
    {
        std::string sss;

        metadata_t* m1 = init_metadata();
        int8_t data_8[]{"hello , world"};
        CHECK(METADATA_OK == add_metadata_entry(m1, METADATA_MAIN, data_8, sizeof(data_8)/sizeof(int8_t)));
        int64_t data_64 = 5555;
        CHECK(METADATA_OK == add_metadata_entry(m1, METADATA_MAIN3, &data_64, 1));

        CHECK(METADATA_OK == dump_metadata(m1,sss));
        std::cout << sss << std::endl;

        metadata_t* m2 = init_metadata();
        int8_t data_8_2[]{"hello "};
        CHECK(METADATA_OK == add_metadata_entry(m2, METADATA_MAIN, data_8_2, sizeof(data_8_2)/sizeof(int8_t)));
        int32_t data_32 = 55;
        CHECK(METADATA_OK == add_metadata_entry(m2, METADATA_MAIN1, &data_32, 1));

        CHECK(METADATA_OK == dump_metadata(m2,sss));
        std::cout << sss << std::endl;
        
        CHECK(METADATA_OK == merge_metadata(m2, m1));

        CHECK(METADATA_OK == dump_metadata(m2,sss));
        std::cout << sss << std::endl;

        metadata_entry_t entry;
        CHECK(METADATA_OK == find_metadata_entry(m2, METADATA_MAIN3, entry));
        CHECK(entry.data.i64[0] == 5555);

        metadata_t* m3 = copy_metadata(m1);

        CHECK(METADATA_OK == delete_metadata(m1));
        CHECK(METADATA_OK == delete_metadata(m2));
        CHECK(METADATA_OK == delete_metadata(m3));
    }

    //class test
    {
        CMetaData* pMetadata = CreateMetaData();

        int32_t a = 123;
        CHECK(pMetadata->add_metadata_entry(METADATA_MAIN1, &a, 1) == METADATA_OK);

        CHECK(pMetadata->add_metadata_entry(METADATA_MAIN1, &a, 1) != METADATA_OK);

        CHECK(pMetadata->get_metadata_entry_count() == 1);

        metadata_entry_t entry;
        CHECK(METADATA_OK == pMetadata->find_metadata_entry(METADATA_MAIN1, entry));
        CHECK(entry.data.i32[0] == 123);

        entry.data.i32[0] = 234;
        CHECK(METADATA_OK == pMetadata->update_metadata_entry(entry));

        metadata_entry_t entry2;
        CHECK(METADATA_OK == pMetadata->find_metadata_entry(METADATA_MAIN1, entry2));
        CHECK(entry2.data.i32[0] == 234);

        CMetaData* pMetadata2 = pMetadata->copy_metadata();
        metadata_entry_t entry3;
        CHECK(METADATA_OK == pMetadata2->find_metadata_entry(METADATA_MAIN1, entry3));
        CHECK(entry3.data.i32[0] == 234);
        DestroyMetaData(pMetadata2);

        CHECK(METADATA_OK == pMetadata->delete_metadata_entry(METADATA_MAIN1));
        CHECK(METADATA_OK != pMetadata->find_metadata_entry(METADATA_MAIN1, entry));

        CHECK(pMetadata->get_metadata_entry_count() == 0);
        
        DestroyMetaData(pMetadata);
    }
}

#if !USE_DOC_TEST

void main()
{       
    test();
    
#if defined(OS_WINDOWS)
    _CrtDumpMemoryLeaks();
#endif
}
#endif