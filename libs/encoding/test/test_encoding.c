#include "unity.h"
#include "encoding/encoding.h"

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

void test_hex_to_base64()
{
    char tmp[512];
    int ret;
    ret = hex_to_base64("AAG", tmp);
    TEST_ASSERT_EQUAL_INT(ret, -1);

    ret = hex_to_base64("", tmp);
    TEST_ASSERT_EQUAL_INT(ret, 0);
    TEST_ASSERT_EQUAL_STRING(tmp, "");

    ret = hex_to_base64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d", tmp);
    TEST_ASSERT_EQUAL_INT(ret, 64);
    TEST_ASSERT_EQUAL_STRING(tmp, "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
}

void test_decode_encode_b64()
{
    uint8_t expected_bytes[] = {0x96, 0x8a, 0xf7, 0x98, 0x8a, 0x6c, 0x52, 0x6d, 0x76, 0xdf, 0xdf, 0x3b, 0xfb};
    char input[] = "lor3mIpsUm123987+w==";

    uint8_t tmp_bytes[512];
    char tmp_str[512];
    int ret;

    ret = decode_b64(input, tmp_bytes);
    TEST_ASSERT_EQUAL_INT(ret, 13);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_bytes, tmp_bytes, 13);

    ret = encode_b64(tmp_bytes, 13, tmp_str);
    TEST_ASSERT_EQUAL_INT(ret, 20);
    TEST_ASSERT_EQUAL_STRING(tmp_str, input);

    ret = decode_b64("", tmp_bytes);
    TEST_ASSERT_EQUAL_INT(ret, 0);

    ret = decode_b64("abc!", tmp_bytes);
    TEST_ASSERT_EQUAL_INT(ret, -1);

    ret = decode_b64("abcde===", tmp_bytes);
    TEST_ASSERT_EQUAL_INT(ret, -1);
}


int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_hex_char_to_byte);
    RUN_TEST(test_byte_to_hex_char);
    RUN_TEST(test_hex_to_base64);
    RUN_TEST(test_decode_encode_b64);
    return UNITY_END();
}
