//
// Copyright (c) 2024 ImagineOn GmbH. All rights reserved.
//

#ifndef UTIL_CLI_HELPER_H_
#define UTIL_CLI_HELPER_H_

#include <stdbool.h>
#include <stdint.h>

#define CLI_OK              (0)
#define CLI_ERROR           (-1)


int cli_check_for_keyword(char* line, uint16_t line_len, uint8_t n, char** keywords, uint8_t keyword_cnt);

int cli_get_uint8(const char* line, uint16_t line_len, uint8_t* val, uint8_t n);

int cli_get_uint16(const char* line, uint16_t line_len, uint16_t* val, uint8_t n);

int cli_get_uint32(const char* line, uint16_t line_len, uint32_t* val, uint8_t n);

int cli_get_string(const char* line, uint16_t line_len, char* buf, uint16_t buf_len, uint8_t n);

bool cli_match_command(const char* line, const char* cmd);

int cli_remove_command_string(const char* line, uint16_t line_len, char** out, uint16_t* out_len);


#endif /* UTIL_CLI_HELPER_H_ */
