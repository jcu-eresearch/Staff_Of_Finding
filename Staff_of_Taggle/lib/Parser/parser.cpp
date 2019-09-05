//
// Created by nigelb on 6/12/2018.
//

#include "parser.h"
#include "string.h"

size_t count_tokens(char* buf, const char* tokens)
{
    size_t length = strlen(buf);
    size_t tok_length = strlen(tokens);
    size_t token_count = 1;
    for(size_t i = 0; i < length; i++)
    {
        for(size_t j = 0; j < tok_length; j ++)
        {
            if(buf[i] == tokens[j])
            {
                token_count++;
            }
        }
    }
    return token_count;
}

void parse_string(char* buf, const char* tokens, size_t argc, char **argv)
{
    size_t length = strlen(buf);
    size_t tok_length = strlen(tokens);

    size_t argv_pos = 1;
    argv[0] = buf;

    for(size_t i = 0; i < length; i++)
    {
        for(size_t j = 0; j < tok_length; j ++)
        {
            if(buf[i] == tokens[j])
            {
                argv[argv_pos++] = buf + i + 1;
                buf[i] = '\0';
            }
        }
    }
}