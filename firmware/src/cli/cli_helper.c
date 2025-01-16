//
// Copyright (c) 2024 ImagineOn GmbH. All rights reserved.
//

#include <stdlib.h>
#include <string.h>

#include <cli/cli_helper.h>
#include <logging.h>

#define MIN(a,b)    ((a < b)? a : b)

LOG_MODULE("CLI:Helper")

static int cli_find_parameter(const char* line, uint16_t line_len, uint8_t n, uint16_t* start_index)
{

  uint8_t cnt = 1;
  if(n == 0 && line[0] != ' ')
  {
    *start_index = 0;
    return CLI_OK;
  }

  // find beginning of parameter
  for(uint16_t i = 0; i < line_len; i++)
  {
    // find white space (or start at beginning
    if((line[i] == ' ' && line[i+1] != ' '))
    {
      // skip until desired parameter is found
      if(cnt < n)
      {
        cnt++;
        continue;
      }
      *start_index = i+1;
      return CLI_OK;
    }
  }

  return CLI_ERROR;

}

int cli_check_for_keyword(char* line, uint16_t line_len, uint8_t n, char** keywords, uint8_t keyword_cnt)
{

  uint16_t len = 0;
  bool match = true;
  uint16_t start_index = 0;

  if(cli_find_parameter(line, line_len, n, &start_index) != 0)
  {
    return CLI_OK;
  }

  for(uint8_t i = 0; i < keyword_cnt; i++)
  {
    len = MIN(line_len - start_index, (uint16_t)strlen(keywords[i]));
    match = true;

    for(uint16_t j = 0; j < len; j++)
    {
      if(keywords[i][j] != line[j + start_index])
      {
        match = false;
        break;
      }
    }

    if(match)
    {
      return i;
    }

  }

  return CLI_ERROR;

}

static int cli_check_for_int(const char* line, uint16_t line_len, uint16_t start_index, bool* isHex)
{
  for(uint16_t i = start_index; i < line_len; i++)
  {
    // check for whitespace (end of parameter)
    if(line[i] == ' ')
    {
      break;
    }
    // check for hex
    else if((line[i] >= 'a' && line[i] <= 'f') || (line[i] >= 'A' && line[i] <= 'F'))
    {
      *isHex = true;
    }
    else if((line[i] == 'x' || line[i] == 'X') && i > start_index)
    {
      if(line[i-1] == '0')
      {
        *isHex = true;
      }
    }
    // check for invalid character
    else if(!(line[i] >= '0' && line[i] <= '9'))
    {
      LOG_DEBUG("Invalid char [0-9] @ %d", i);
      return CLI_ERROR;
    }
  }

  // reached end of line or whitespace without finding invalid character
  return CLI_OK;

}

static int cli_get_int(const char* line, uint16_t line_len, uint8_t n, bool* isHex, uint16_t* start_index)
{
  if(line_len == 0)
  {
    return CLI_ERROR;
  }

  if(cli_find_parameter(line, line_len, n, start_index) != 0)
  {
    return CLI_ERROR;
  }

  if(cli_check_for_int(line, line_len, *start_index, isHex) != 0)
  {
    return CLI_ERROR;
  }

  return CLI_OK;

}

/**
 * Parse uint8_t parameter from CLI string.
 *
 * Note: parameters must be separated by whitespaces
 * @param line  - CLI string
 * @param line  - len of CLI string
 * @param val   - output variable
 * @param n     - number of parameter (e.g. dapc <addr> <lvl> -> addr: n = 0, lvl: n = 1)
 * @return 0    - on success
 *        -1    - else
 */
int cli_get_uint8(const char* line, uint16_t line_len, uint8_t* val, uint8_t n)
{

  bool isHex = false;
  uint16_t start_index = 0;

  if(cli_get_int(line, line_len, n, &isHex, &start_index) != 0)
  {
    return CLI_ERROR;
  }

  if(isHex)
  {
    *val = (uint8_t)strtol(line + start_index, NULL, 16);
  }
  else
  {
    *val = (uint8_t)strtol(line + start_index, NULL, 10);
  }

  return CLI_OK;

}

int cli_get_uint16(const char* line, uint16_t line_len, uint16_t* val, uint8_t n)
{

  bool isHex = false;
  uint16_t start_index = 0;

  if(cli_get_int(line, line_len, n, &isHex, &start_index) != 0)
  {
    return CLI_ERROR;
  }

  if(isHex)
  {
    *val = (uint16_t)strtol(line + start_index, NULL, 16);
  }
  else
  {
    *val = (uint16_t)strtol(line + start_index, NULL, 10);
  }

  return CLI_OK;

}

int cli_get_uint32(const char* line, uint16_t line_len, uint32_t* val, uint8_t n)
{

  bool isHex = false;
  uint16_t start_index = 0;

  if(cli_get_int(line, line_len, n, &isHex, &start_index) != 0)
  {
    return CLI_ERROR;
  }

  if(isHex)
  {
    *val = (uint32_t)strtol(line + start_index, NULL, 16);
  }
  else
  {
    *val = (uint32_t)strtol(line + start_index, NULL, 10);
  }

  return CLI_OK;
}

int cli_get_string(const char* line, uint16_t line_len, char* buf, uint16_t buf_len, uint8_t n)
{

  uint16_t start_index = 0;
  uint16_t end_index = 0;

  if(line_len == 0)
  {
    return CLI_ERROR;
  }

  // get start of parameter
  if(cli_find_parameter(line, line_len, n, &start_index) != CLI_OK)
  {
    return CLI_ERROR;
  }

  // get end of parameter
  if(cli_find_parameter(line, line_len, n+1, &end_index) != CLI_OK)
  {
    end_index = line_len;
  }
  else
  {
    end_index -= 1;
  }

  uint16_t len = end_index - start_index;
  if(len > buf_len)
  {
    return CLI_ERROR;
  }

  memmove(buf, line + start_index, len);
  return CLI_OK;

}


bool cli_match_command(const char* line, const char* cmd)
{
  size_t cmd_len = strlen(cmd);
  size_t line_len = strlen(line);

  if(line_len < cmd_len)
  {
    return false;
  }

  // if contents of line and cmd are not identical for length of line, go to next option
  if(strncmp(cmd, line, cmd_len) != 0)
  {
    return false;
  }

  // catch substring matches in command sets (e.g. "set" and "set-prop" need to be distinguishable)
  if(line_len > cmd_len)
  {

    // check if line is terminated at the length of command
    if (line[cmd_len] != ' ' && line[cmd_len] != '\r' && line[cmd_len] != '\n')
    {
      return false;
    }
  }

  return true;
}

int cli_remove_command_string(const char* line, uint16_t line_len, char** out, uint16_t* out_len)
{
  for(uint16_t i = 0; i < line_len; i++)
  {
    if(line[i] == ' ' && line[i+1] != ' ')
    {
      // get len of cut line
      *out_len = line_len - i - 1;
      // allocate memory
      *out = (char*)malloc(sizeof(char) * (*out_len));
      // copy contents
      strncpy(*out, line+i+1, *out_len);
      (*out)[*out_len] = '\0';

      return CLI_OK;
    }
  }

  return CLI_ERROR;
  
}
