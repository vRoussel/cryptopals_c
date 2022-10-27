#!/bin/bash

ret=$(< input.txt ../decipher_xor_sbk)

if [ "$ret" != "7b5a4215415d544115415d5015455447414c155c46155f4058455c5b523f [key=53] [score=69] -> Now that the party is jumping" ]; then
    exit 1
fi
