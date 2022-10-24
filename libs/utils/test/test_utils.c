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

void test_hex_to_base64()
{
    char tmp[512];
    int ret;
    ret = hex_to_base64("AAG", tmp);
    TEST_ASSERT_EQUAL_INT(ret, -1);
    TEST_ASSERT_EQUAL_STRING(tmp, "");

    ret = hex_to_base64("", tmp);
    TEST_ASSERT_EQUAL_INT(ret, 0);
    TEST_ASSERT_EQUAL_STRING(tmp, "");

    ret = hex_to_base64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d", tmp);
    TEST_ASSERT_EQUAL_INT(ret, 64);
    TEST_ASSERT_EQUAL_STRING(tmp, "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
}

void test_xor()
{
    char *hex1 = "1c0111001f010100061a024b53535009181c";
    char *hex2 = "686974207468652062756c6c277320657965";
    char tmp[512];
    ssize_t ret;

    ret = xor_hex(hex1, hex2, tmp);
    TEST_ASSERT_EQUAL_INT(ret, 36);
    TEST_ASSERT_EQUAL_STRING(tmp, "746865206B696420646F6E277420706C6179");

    hex1 = "001101";
    hex2 = "ABAACC";
    ret = xor_hex(hex1, hex2, tmp);
    TEST_ASSERT_EQUAL_INT(ret, 6);
    TEST_ASSERT_EQUAL_STRING(tmp, "ABBBCD");

    hex1 = "110G";
    hex2 = "ABAA";
    ret = xor_hex(hex1, hex2, tmp);
    TEST_ASSERT_EQUAL_INT(ret, -1);
}


void test_decipher_single_byte_xor()
{
    char *ciphered = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char tmp[512];
    ssize_t ret;

    ret = decipher_single_byte_xor(ciphered, tmp);
    TEST_ASSERT_EQUAL_INT(ret, 34);
    TEST_ASSERT_EQUAL_STRING(tmp, "Cooking MC's like a pound of bacon");
}


int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_hex_char_to_byte);
    RUN_TEST(test_byte_to_hex_char);
    RUN_TEST(test_hex_to_base64);
    RUN_TEST(test_xor);
    RUN_TEST(test_decipher_single_byte_xor);
    return UNITY_END();
}
