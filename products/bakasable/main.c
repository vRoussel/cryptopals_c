#include <stdio.h>
#include "utils/utils.h"

int main(void)
{
    printf("Hello bakasable\n");
    char tmp[512];
    int ret = hex_to_base64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d", tmp);
    printf("%d\n", ret);
    printf("%s\n", tmp);
}
