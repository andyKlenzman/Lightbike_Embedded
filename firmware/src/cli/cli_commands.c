/*
 * cli_commands.c
 *
 *  Created on: 01.11.2023
 *      Author: Alexander Müller (ImagineOn GmbH)
 */



#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <cmsis_os2.h>

#include <logging.h>
#include "cli_helper.h"
#include "cli_commands.h"
#include "cli_command_definitions.h"



LOG_MODULE(CLI);

void cli_info(const char* line, uint16_t len);
// void cli_reset(const char* line, uint16_t len);

// void cli_vgear_info(const char* line, uint16_t len);


static const cli_command_t commands[] = {
    {CLI_INFO, cli_info},
    // {CLI_RESET, cli_reset},
    {NULL, NULL}
};

const cli_command_t* cli_get_command_set()
{
  return commands;
}

void cli_info(const char* line, uint16_t len)
{
  LOG_DEBUG("INFO: ");
  LOG_INFO("None yet :)");

  osDelay(50);

}

void cli_light_mode(const char* line, uint8_t data_source, uint8_t filter)
{
    LOG_INFO("CLI: LightMode");

    // virtual_gear_recall_scene(addr_type, addr, scene_id);
}


//if(cli_get_uint8(line, len, &addr_type, 0) != 0) return;
//
//uint8_t addr;
//if(cli_get_uint8(line, len, &addr, 1) != 0) return;
//
//uint8_t scene_id;
//if(cli_get_uint8(line, len, &scene_id, 2) != 0) return;
