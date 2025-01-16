//
// Copyright (c) 2024 ImagineOn GmbH. All rights reserved.
//


#include <cmsis_os2.h>
#include <stdlib.h>
#include <string.h>

#include <logging.h>
#include <cli/cli_helper.h>

#include <cli/cli.h>
#include <cli/cli_commands.h>

#include <cli/SEGGER/SEGGER_RTT.h>

#define CLI_LINE_BUF_SIZE               (128)
#define CLI_THREAD_NAME                 "cli_thread"

const osThreadAttr_t cli_thread_attr = {
    .priority = osPriorityNormal,
    .name = CLI_THREAD_NAME,
    .stack_size = 1024
};

static bool exit_cli = false;
static bool line_receveived = false;
static osThreadId_t cli_thread_id = NULL;
static osSemaphoreId_t cli_exit_sem;
static char line_buffer[CLI_LINE_BUF_SIZE] = {0};

LOG_MODULE(CLI)

void cli_thread(void* p);

/**
 * Read characters into buffer until '\n' is received.
 * Note: Using 'send on enter' on terminal might lead to lines not fully being transmitted (depends on upstream buf size)
 * @param buf
 * @return -1 (on too long inputs)
 *         else: length of line
 */
static int cli_get_line(char* buf, uint16_t buflen)
{

  uint16_t read_ix = 0;

  do {
    read_ix += SEGGER_RTT_Read(read_ix, buf+read_ix, buflen - 1);
    for(uint16_t i = 0; i < read_ix; i++)
    {
      if(buf[i] == '\n' || buf[i] == '\r')
      {
        buf[i+1] = '\0';
        return i;
      }
    }
    osDelay(500);
  } while(read_ix < buflen);

  return -1;

}

static void cli_parse_line(const char* line, uint16_t len)
{
  if (len <= 1)
  {
    LOG_DEBUG("Discard line");
    return;
  }

  char* params = NULL;
  uint16_t params_len = 0;
  uint8_t i = 0;
  const cli_command_t* commands = cli_get_command_set();


  if(cli_match_command(line, "exit"))
  {
    exit_cli = true;
    LOG_INFO("Leaving CLI...");
    return;
  }

  while(commands[i].cmd != NULL)
  {
    if(cli_match_command(line, commands[i].cmd))
    {
      if(len > strlen(commands[i].cmd) + 1)
      {
        if(cli_remove_command_string(line, len, &params, &params_len) != 0)
        {
          LOG_DEBUG("Could not parse line");
          return;
        }
      }

      (commands[i].callback)(params, params_len);
      free(params);
      return;
    }

    i++;
  }

  LOG_ERROR("Unknown command: %s", line);

}

void cli_init(uint32_t timeout_ms)
{
  
  cli_exit_sem = osSemaphoreNew(1, 0, 0);
  cli_thread_id = osThreadNew(cli_thread, NULL, &cli_thread_attr); 

  if(timeout_ms != 0 && timeout_ms != osWaitForever)
  {
    // wait for timeout
    osDelay(timeout_ms);
    if(!line_receveived)
    {
      osThreadTerminate(cli_thread_id);
    }
    else
    {
      // wait for exit condition...
      osSemaphoreAcquire(cli_exit_sem, osWaitForever);
    }
  }
}

void cli_thread(void* p)
{
  (void)p;
  int len;

  LOG_INFO("Running");

  while(1)
  {
    len = cli_get_line (line_buffer, sizeof(line_buffer));
    if(len == -1)
    {
      LOG_ERROR("Invalid line (discarded)");
      continue;
    }

    line_receveived = true;

    cli_parse_line (line_buffer, (uint16_t)len);

    if(exit_cli)
    {
      osSemaphoreRelease(cli_exit_sem);
      if(cli_thread_id != NULL)
      {
        osThreadExit();
      } 
      else
      {
        return;
      }
    }

  }

}
