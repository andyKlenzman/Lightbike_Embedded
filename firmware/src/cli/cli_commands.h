/*
 * cli_commands.h
 *
 *  Created on: 01.11.2023
 *      Author: Alexander Müller (ImagineOn GmbH)
 */

#ifndef UTIL_CLI_COMMANDS_H_
#define UTIL_CLI_COMMANDS_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef struct cli_command_s
{
  const char* cmd;
  void (*callback)(const char* line, uint16_t len);
} cli_command_t;

const cli_command_t* cli_get_command_set();

#ifdef __cplusplus
}
#endif

#endif /* UTIL_CLI_COMMANDS_H_ */
