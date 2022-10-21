#include "unity.h"
#include "../utils.h"

void setUp() {}
void tearDown() {}

void test_hex_char_to_byte()
{
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('-'), -1);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('G'), -1);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('0'), 0);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('1'), 1);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('2'), 2);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('3'), 3);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('4'), 4);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('5'), 5);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('6'), 6);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('7'), 7);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('8'), 8);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('9'), 9);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('A'), 10);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('B'), 11);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('C'), 12);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('D'), 13);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('E'), 14);
    TEST_ASSERT_EQUAL_INT(hex_char_to_byte('F'), 15);
}

void test_byte_to_hex_char()
{
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(0), '0');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(1), '1');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(2), '2');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(3), '3');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(4), '4');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(5), '5');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(6), '6');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(7), '7');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(8), '8');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(9), '9');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(10), 'A');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(11), 'B');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(12), 'C');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(13), 'D');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(14), 'E');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(15), 'F');
    TEST_ASSERT_EQUAL_CHAR(byte_to_hex_char(16), -1);
}

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
    RUN_TEST(test_hex_char_to_byte);
    RUN_TEST(test_byte_to_hex_char);
    RUN_TEST(test_hex_to_base64_empty);
    RUN_TEST(test_hex_to_base64_wrong_hex);
    RUN_TEST(test_hex_to_base64_cryptopals);
    return UNITY_END();
}
