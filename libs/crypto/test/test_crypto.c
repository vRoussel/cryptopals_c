#include "unity.h"
#include "crypto/crypto.h"

void setUp() {}
void tearDown() {}

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

void test_decipher_single_byte_xor()
{
    char *ciphered = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char tmp[512];
    ssize_t ret;
    uint8_t key;
    unsigned int score;

    ret = decipher_single_byte_xor(ciphered, tmp, &key, &score);
    TEST_ASSERT_EQUAL_INT(ret, 34);
    TEST_ASSERT_EQUAL_INT(key, 88);
    TEST_ASSERT_EQUAL_STRING(tmp, "Cooking MC's like a pound of bacon");
}


int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_xor);
    RUN_TEST(test_decipher_single_byte_xor);
    return UNITY_END();
}
