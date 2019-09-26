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

#ifndef DRO_LORA_TEST_PARSER_H
#define DRO_LORA_TEST_PARSER_H

#include "stddef.h"

size_t count_tokens(char* buf, const char* tokens);
void parse_string(char* buf, const char* tokens, size_t argc, char **argv);

#endif //DRO_LORA_TEST_PARSER_H
