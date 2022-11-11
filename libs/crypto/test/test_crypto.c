#include "unity.h"
#include "crypto/crypto.h"
#include "encoding/encoding.h"

#include <string.h>

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

void setUp() {}
void tearDown() {}

void test_xor()
{
    uint8_t input1[] = {0x1c, 0x01, 0x11, 0x00, 0x1f, 0x01, 0x01, 0x00, 0x06, 0x1a, 0x02, 0x4b, 0x53, 0x53, 0x50, 0x09, 0x18, 0x1c};
    uint8_t input2[] = {0x68, 0x69, 0x74, 0x20, 0x74, 0x68, 0x65, 0x20, 0x62, 0x75, 0x6c, 0x6c, 0x27, 0x73, 0x20, 0x65, 0x79, 0x65};
    uint8_t expected12[] = {0x74, 0x68, 0x65, 0x20, 0x6B, 0x69, 0x64, 0x20, 0x64, 0x6F, 0x6E, 0x27, 0x74, 0x20, 0x70, 0x6C, 0x61, 0x79};


    uint8_t tmp[512];
    ssize_t ret;

    ret = xor(input1, ARRAY_SIZE(input1), input2, ARRAY_SIZE(input2), tmp);
    TEST_ASSERT_EQUAL_INT(ret, ARRAY_SIZE(expected12));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(tmp, expected12, ARRAY_SIZE(expected12));

    uint8_t input3[] = {0x00, 0x11, 0x01};
    uint8_t input4[] = {0xAB, 0xAA, 0xCC};
    uint8_t expected34[] = {0xAB, 0xBB, 0xCD};
    ret = xor(input3, ARRAY_SIZE(input3), input4, ARRAY_SIZE(input4), tmp);
    TEST_ASSERT_EQUAL_INT(ret, ARRAY_SIZE(expected34));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(tmp, expected34, ARRAY_SIZE(expected34));
}

//void test_xor_repeated_key()
//{
//    char *input = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
//    char *key = "ICE";
//    char tmp[512];
//    ssize_t ret;
//
//    ret = xor_repeated_key(input, strlen(input), key, strlen(key), tmp);
//    TEST_ASSERT_EQUAL_INT(ret, 6);
//    TEST_ASSERT_EQUAL_STRING(tmp, "ABBBCD");
//}

void test_xor_repeated_key_str_to_hex()
{
    char *input = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    char *key = "ICE";
    uint8_t expected[] = {
        0x0B, 0x36, 0x37, 0x27, 0x2A, 0x2B, 0x2E, 0x63,
        0x62, 0x2C, 0x2E, 0x69, 0x69, 0x2A, 0x23, 0x69,
        0x3A, 0x2A, 0x3C, 0x63, 0x24, 0x20, 0x2D, 0x62,
        0x3D, 0x63, 0x34, 0x3C, 0x2A, 0x26, 0x22, 0x63,
        0x24, 0x27, 0x27, 0x65, 0x27, 0x2A, 0x28, 0x2B,
        0x2F, 0x20, 0x43, 0x0A, 0x65, 0x2E, 0x2C, 0x65,
        0x2A, 0x31, 0x24, 0x33, 0x3A, 0x65, 0x3E, 0x2B,
        0x20, 0x27, 0x63, 0x0C, 0x69, 0x2B, 0x20, 0x28,
        0x31, 0x65, 0x28, 0x63, 0x26, 0x30, 0x2E, 0x27,
        0x28, 0x2F
    };

    uint8_t tmp[512];
    ssize_t ret;

    ret = xor_repeated_key((uint8_t*)input, strlen(input), (uint8_t*)key, strlen(key), tmp);
    TEST_ASSERT_EQUAL_INT(ret, ARRAY_SIZE(expected));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(tmp, expected, ARRAY_SIZE(expected));
}

void test_decipher_xor_single_byte_key()
{
    uint8_t ciphered[] = {
        0x1b, 0x37, 0x37, 0x33, 0x31, 0x36, 0x3f, 0x78,
        0x15, 0x1b, 0x7f, 0x2b, 0x78, 0x34, 0x31, 0x33,
        0x3d, 0x78, 0x39, 0x78, 0x28, 0x37, 0x2d, 0x36,
        0x3c, 0x78, 0x37, 0x3e, 0x78, 0x3a, 0x39, 0x3b,
        0x37, 0x36};
    char *expected = "Cooking MC's like a pound of bacon";
    uint8_t expected_key = 'X';
    char tmp[512];

    ssize_t ret;
    uint8_t key;
    unsigned int score;

    ret = decipher_xor_single_byte_key(ciphered, ARRAY_SIZE(ciphered), tmp, &key, &score);
    TEST_ASSERT_EQUAL_INT(ret, strlen(expected));
    TEST_ASSERT_EQUAL_INT(key, expected_key);
    TEST_ASSERT_EQUAL_STRING(tmp, expected);
}

void test_decipher_aes_128_ecb()
{
    uint8_t *key = (uint8_t*)"YELLOW SUBMARINE";
    char input[4096];
    FILE *f = fopen("test_decipher_aes_128_ecb_input.txt", "r");
    TEST_ASSERT_NOT_NULL(f);
    char* fgets_ret = fgets(input, 4096, f);
    TEST_ASSERT_NOT_NULL(fgets_ret);

    size_t input_len = strlen(input);
    TEST_ASSERT_EQUAL_CHAR(input[input_len-1], '\n');
    input[--input_len] = '\0';


    char expected[4096];
    FILE *f2 = fopen("test_decipher_aes_128_ecb_output.txt", "r");
    TEST_ASSERT_NOT_NULL(f2);
    size_t bytes_read = fread(expected, sizeof(char), sizeof(expected), f2);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(0, bytes_read);
    expected[bytes_read] = '\0';


    uint8_t binary[4096];
    ssize_t decode_ret = decode_b64(input, binary);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(0, decode_ret);
    size_t binary_len = decode_ret;

    uint8_t output[4096];
    ssize_t decipher_ret = decipher_aes_128_ecb(binary, binary_len, key, output);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(0, decipher_ret);
    TEST_ASSERT_EQUAL_STRING(output, expected);

    fclose(f);
    fclose(f2);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_xor);
    RUN_TEST(test_decipher_xor_single_byte_key);
    RUN_TEST(test_xor_repeated_key_str_to_hex);
    RUN_TEST(test_decipher_aes_128_ecb);
    return UNITY_END();
}
