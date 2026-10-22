/*
 * shell.h
 *
 *  Created on: Oct 16, 2026
 *      Author: laurentf
 */

#ifndef TD6_COMPLETE_APP_SHELL_H_
#define TD6_COMPLETE_APP_SHELL_H_

#include <stdbool.h>
#include <stdint.h>

#define CMD_BUFFER_LEN 40

typedef int (* pfunc_t)(int argc, char ** argv);

typedef struct shell_struct {
	char cmd_buffer[CMD_BUFFER_LEN];
	int cmd_buffer_pos;
} shell_t;

bool shell_init(shell_t * sh);
bool shell_process_char(shell_t * sh, char c);

/* Add a function to **every** shell */
bool shell_add(char c, pfunc_t pfunc, char * description);

#endif /* TD6_COMPLETE_APP_SHELL_H_ */
