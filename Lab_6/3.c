#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "list.h"

typedef struct command {
	const char* executable_name;
	const char** arguments;
} command;

static const char* const argument_delimiters = " \r\n";

static char* get_input_line();
static command parse_command(const char* input);
static void execute_command(command command);
static void free_command(command* command);

int main(int argc, char** argv) {
	while (true) {
		char* input_line = get_input_line();
		command command = parse_command(input_line);
		execute_command(command);

		free(input_line);
		free_command(&command);
	}
}

char* get_input_line() {
	char* input_line = NULL;
	size_t input_line_length;
	while (true) {
		ssize_t bytes_read = getline(&input_line, &input_line_length, stdin);
		if (bytes_read != -1)
			return input_line;

		free(input_line);
	}
}

command parse_command(const char* input) {
	list arguments = create_list(2);
	char* input_copy = strdup(input);

	char* argument = strtok(input_copy, argument_delimiters);
	while (argument != NULL) {
		append_to_list(&arguments, argument);

		argument = strtok(NULL, argument_delimiters);
	}
	append_to_list(&arguments, NULL);
	if (*arguments.elements == NULL) {
		command empty;
		return empty;
	}

	command command;
	command.executable_name = *arguments.elements;
	command.arguments = (const char**)arguments.elements;
	return command;
}

void execute_command(command command) {
	pid_t child_pid = fork();
	if (child_pid < 0) {
		fprintf(stderr, "Can't create child process to execute command\n");
		return;
	}

	if (child_pid > 0) {
		// waitpid(child_pid, NULL, 0);
		return;
	}

	execvp(command.executable_name, (char* const*)command.arguments);
	fprintf(stderr, "Can't execute \"%s\"\n", command.executable_name);
	exit(1);
}

void free_command(command* command) {
	// As we parsed command with strtok, we actually got one string with many '\0' symbols
	// instead of many individual strings, so we should call free only for the first argument
	free((void*)*command->arguments);
	free(command->arguments);

	command->executable_name = NULL;
	command->arguments = NULL;
}
