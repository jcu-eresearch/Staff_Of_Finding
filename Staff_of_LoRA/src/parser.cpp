/*
    Staff_Of_Lora sends GPS positions via a Multitech mDot to be used to
    perform site surveys.
    Copyright (C) 2019  Nigel Bajema eResearch Center James Cook University

    This program is free software: you can redistribute it and/or modify
            it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
            but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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