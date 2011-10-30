/*********************************************************************
 *
 * Copyright 2011 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *********************************************************************/


/*
 * Top-level application for accessing almost of apitrace
 * functionality.
 */

#include <string.h>

#include <iomanip>
#include <iostream>

#include "cli.hpp"

#define ARRAY_SIZE(arr) (sizeof (arr) / sizeof (arr[0]))

static const char *help_synopsis = "Print detailed help for the given command.";

static void list_commands(void);

static void
help_usage()
{
    std::cout
        << "usage: apitrace help [<command>]\n"
        << help_synopsis << "\n"
        "\n";

    list_commands();
}

static int
help_command(int argc, char *argv[], int first_command_arg);

const Command help = {
    "help",
    help_synopsis,
    help_usage,
    help_command
};

static const Command * commands[] = {
    &dump,
    &help,
};

static void
usage(void)
{
    std::cout <<
        "Usage: apitrace <command> [<args> ...]\n"
        "Top-level command line frontend to apitrace.\n"
        "\n";

    list_commands();
}

static void
list_commands(void) {
    const Command *command;
    int i, max_width = 0;

    std::cout << "The available commands are as follows:\n\n";

    std::cout << std::setiosflags(std::ios::left);

    for (i = 0; i < ARRAY_SIZE(commands); i++) {
        command = commands[i];
        if (strlen(command->name) > max_width) {
            max_width = strlen(command->name);
        }
    }

    for (i = 0; i < ARRAY_SIZE(commands); i++) {
        command = commands[i];

        std::cout << "    " << std::setw(max_width + 2) << command->name
                  << " " << command->synopsis << "\n";
    }

    std::cout << "\n"
        "Use \"apitrace help <command>\" for more details on each command.\n";
}


static int
help_command(int argc, char *argv[], int first_arg_command)
{
    const Command *command;
    int i;

    if (first_arg_command == argc) {
        help_usage();
        return 0;
    }

    char *command_name = argv[first_arg_command];

    for (i = 0; i < ARRAY_SIZE(commands); i++) {
        command = commands[i];

        if (strcmp(command_name, command->name) == 0) {
            (command->usage) ();
            return 0;
        }
    }

    std::cerr << "Error: Unknown command: " << command_name
              << " (see \"apitrace help\").\n";

    return 1;
}

int
main(int argc, char **argv)
{
    const char *command_name = "trace";
    const Command *command;
    int i, first_command_arg;

    if (argc == 1) {
        usage();
        return 1;
    }

    for (i = 1; i < argc; ++i) {
        const char *arg = argv[i];

        if (arg[0] != '-') {
            break;
        }

        if (strcmp(arg, "--help") == 0) {
            return help_command (0, NULL, 0);
        } else {
            std::cerr << "Error: unknown option " << arg << "\n";
            usage();
            return 1;
        }
    }
    first_command_arg = i;

    if (first_command_arg < argc) {
        command_name = argv[first_command_arg];
        first_command_arg++;
    }

    for (i = 0; i < ARRAY_SIZE(commands); i++) {
        command = commands[i];

        if (strcmp(command_name, command->name) == 0)
            return (command->function) (argc, argv, first_command_arg);
    }

    std::cerr << "Error: unknown command " << command_name
              << " (see \"apitrace help\").\n";

    return 1;
}
