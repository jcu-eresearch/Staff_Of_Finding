//
// Created by nigelb on 6/12/2018.
//

#ifndef DRO_LORA_TEST_PARSER_H
#define DRO_LORA_TEST_PARSER_H

#include "stddef.h"

size_t count_tokens(char* buf, const char* tokens);
void parse_string(char* buf, const char* tokens, size_t argc, char **argv);

#endif //DRO_LORA_TEST_PARSER_H
