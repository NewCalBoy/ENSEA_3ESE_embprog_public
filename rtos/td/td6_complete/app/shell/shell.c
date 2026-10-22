/*
 * shell.c
 *
 *  Created on: Oct 16, 2026
 *      Author: laurentf
 */

#include "shell.h"

#include <stdio.h>

#define SHELL_FUNC_LIST_LEN 32
#define ARGC_MAX 8

typedef struct shell_func_struct {
	char c;
	pfunc_t pfunc;
	char * description;
} shell_func_t;

typedef struct shell_funcs_struct {
	int list_size;
	shell_func_t list[SHELL_FUNC_LIST_LEN];
} shell_funcs_t;

static shell_funcs_t shell_funcs;

static int sh_help(int argc, char ** argv);
static int shell_exec(char * buf);

bool shell_init(shell_t * sh) {
	shell_funcs.list_size = 0;
	shell_add('h', sh_help, "Help");

	sh->cmd_buffer_pos = 0;

	printf("\r\n\r\n===== Monsieur Shell v0.3 =====\r\n");
	printf("> ");
	fflush(stdout);

	return true;
}

bool shell_process_char(shell_t * sh, char c) {
	switch (c) {
	case '\r':	//process RETURN key
		//case '\n':
		printf("\r\n");
		sh->cmd_buffer[sh->cmd_buffer_pos++] = 0;     //add \0 char at end of string
		printf(":%s\r\n", sh->cmd_buffer);
		sh->cmd_buffer_pos = 0;            //reset buffer

		/* Call the function */
		shell_exec(sh->cmd_buffer);

		printf("> ");
		fflush(stdout);
		break;
	case '\b':	//backspace
		if (sh->cmd_buffer_pos > 0) {      //is there a char to delete?
			sh->cmd_buffer_pos--;          //remove it in buffer

			printf("\b \b");				// delete the char on the terminal
			fflush(stdout);
		}
		break;
		//other characters
	default:
		//only store characters if buffer has space
		if (sh->cmd_buffer_pos < CMD_BUFFER_LEN - 1) {
			printf("%c", c);
			fflush(stdout);
			sh->cmd_buffer[sh->cmd_buffer_pos++] = c; //store
		}
	}

	return true;
}

/* Add a function to **every** shell */
bool shell_add(char c, pfunc_t pfunc, char * description) {
	if (shell_funcs.list_size >= SHELL_FUNC_LIST_LEN) {
		return false;
	}

	shell_funcs.list[shell_funcs.list_size].c = c;
	shell_funcs.list[shell_funcs.list_size].pfunc = pfunc;
	shell_funcs.list[shell_funcs.list_size].description = description;
	shell_funcs.list_size++;

	return true;
}

static int sh_help(int argc, char ** argv) {
	for(int i = 0 ; i < shell_funcs.list_size ; i++) {

		printf("%c: %s\r\n", shell_funcs.list[i].c, shell_funcs.list[i].description);
	}

	return 0;
}

static int shell_exec(char * buf) {
	char c = buf[0];

	int argc;
	char * argv[ARGC_MAX];
	char *p;

	for(int i = 0 ; i < shell_funcs.list_size ; i++) {
		if (shell_funcs.list[i].c == c) {
			argc = 1;
			argv[0] = buf;

			for(p = buf ; *p != '\0' && argc < ARGC_MAX ; p++){
				if(*p == ' ') {
					*p = '\0';
					argv[argc++] = p+1;
				}
			}

			return shell_funcs.list[i].pfunc(argc, argv);
		}
	}

	printf("%c: no such command\r\n", c);
	return -1;
}
