#include "unity.h"
#include "../utils.h"

void setUp() {}
void tearDown() {}

void test_hex_to_base64_wrong_hex()
{
    char tmp[512];
    int ret = hex_to_base64("AAG", tmp);
    TEST_ASSERT_EQUAL_INT(ret, -1);
    TEST_ASSERT_EQUAL_STRING(tmp, "");
}

void test_hex_to_base64_empty()
{
    char tmp[512];
    int ret = hex_to_base64("", tmp);
    TEST_ASSERT_EQUAL_INT(ret, 0);
    TEST_ASSERT_EQUAL_STRING(tmp, "");
}

void test_hex_to_base64_cryptopals()
{
    char tmp[512];
    int ret = hex_to_base64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d", tmp);
    TEST_ASSERT_EQUAL_INT(ret, 64);
    TEST_ASSERT_EQUAL_STRING(tmp, "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
}


int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_hex_to_base64_empty);
    RUN_TEST(test_hex_to_base64_wrong_hex);
    RUN_TEST(test_hex_to_base64_cryptopals);
    return UNITY_END();
}
