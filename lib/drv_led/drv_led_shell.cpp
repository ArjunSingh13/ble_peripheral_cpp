#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <version.h>
#include <zephyr/logging/log.h>
#include <stdint.h>
#include <zephyr/posix/unistd.h>


LOG_MODULE_REGISTER(drv_led_shell, LOG_LEVEL_INF);

static int shell_led_on(const struct shell *sh, size_t argc,
                        char **argv)
{
    //shell_print(sh, "Print simple text.");
    //shell_warn(sh, "Print warning text.");
    //shell_error(sh, "Print error text.");
    shell_print(sh,"Shell LED ON command\r\n");
    return 0;
}

static int shell_led_off(const struct shell *sh, size_t argc,
                        char **argv)
{
    // ARG_UNUSED(argc);
    // ARG_UNUSED(argv);
    //shell_print(sh, "Print simple text.");
    //shell_warn(sh, "Print warning text.");
    //shell_error(sh, "Print error text.");
    shell_print(sh,"Shell LED OFF command\r\n");
    return 0;
}
/* Creating subcommands (level 1 command) array for command "demo". */
SHELL_STATIC_SUBCMD_SET_CREATE(led_cmd,
        SHELL_CMD_ARG(on, 
                    NULL, 
                    "Turn ON an LED <Led Number>. Led number value range 0-1.",
                    shell_led_on,
                    2,
                    0),

        SHELL_CMD_ARG(off,
                   NULL, 
                   "Turn OFF an LED <Led Number. Led number value range 0-1." , 
                   shell_led_off,
                   2,
                   0),

        SHELL_SUBCMD_SET_END
);

/* Creating root (level 0) command "drv_led" */
SHELL_CMD_REGISTER(drv_led, &led_cmd, "Drv led shell commands", NULL);

