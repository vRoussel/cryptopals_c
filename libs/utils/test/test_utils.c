#include "unity.h"
#include "utils/utils.h"
#include "utils/hashmap.h"

#include <stdbool.h>
#include <string.h>

void setUp() {}
void tearDown() {}

void test_swap()
{
    int i1 = 1;
    int i2 = 2;
    SWAP(i1,i2, int);
    TEST_ASSERT_EQUAL_INT(i1, 2);
    TEST_ASSERT_EQUAL_INT(i2, 1);

    char c1 = 'a';
    char c2 = 'k';
    SWAP(c1, c2, char);
    TEST_ASSERT_EQUAL_CHAR(c1, 'k');
    TEST_ASSERT_EQUAL_CHAR(c2, 'a');

    char *s1 = "hello";
    char *s2 = "world";
    SWAP(s1, s2, char*);
    TEST_ASSERT_EQUAL_STRING(s1, "world");
    TEST_ASSERT_EQUAL_STRING(s2, "hello");
}

void test_hamming_distance_str()
{
    char *s1 = "this is a test";
    char *s2 = "wokka wokka!!!";
    unsigned int hd = hamming_distance_str(s1, s2);
    TEST_ASSERT_EQUAL_UINT(hd, 37);
}

void test_hashmap()
{
    bool found;
    char *tmp;
    hashmap_s hm;
    hashmap_init(&hm);

    hashmap_set(&hm, "hello", "world", strlen("world") + 1);
    tmp = hashmap_get(&hm, "hello", &found);
    TEST_ASSERT_TRUE(found);
    TEST_ASSERT_EQUAL_STRING(tmp, "world");

    hashmap_set(&hm, "hello", "new world", strlen("new world") + 1);
    tmp = hashmap_get(&hm, "hello", &found);
    TEST_ASSERT_TRUE(found);
    TEST_ASSERT_EQUAL_STRING(tmp, "new world");

    hashmap_set(&hm, "just_a_key", NULL, 0);
    tmp = hashmap_get(&hm, "just_a_key", &found);
    TEST_ASSERT_TRUE(found);
    TEST_ASSERT_EQUAL_STRING(tmp, NULL);

    tmp = hashmap_get(&hm, "wrong_key", &found);
    TEST_ASSERT_FALSE(found);
    TEST_ASSERT_EQUAL_STRING(tmp, NULL);

    hashmap_finalize(&hm);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_swap);
    RUN_TEST(test_hamming_distance_str);
    RUN_TEST(test_hashmap);
    return UNITY_END();
}
