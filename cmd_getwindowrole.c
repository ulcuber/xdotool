#include "xdo_cmd.h"

int cmd_getwindowrole(context_t *context) {
  char *cmd = context->argv[0];

  int c;
  static struct option longopts[] = {
    { "help", no_argument, NULL, 'h' },
    { 0, 0, 0, 0 },
  };
  static const char *usage =
    "Usage: %s [window=%1]\n"
    HELP_SEE_WINDOW_STACK;
  int option_index;

  while ((c = getopt_long_only(context->argc, context->argv, "+h",
                               longopts, &option_index)) != -1) {
    switch (c) {
      case 'h':
        printf(usage, cmd);
        consume_args(context, context->argc);
        return EXIT_SUCCESS;
        break;
      default:
        fprintf(stderr, usage, cmd);
        return EXIT_FAILURE;
    }
  }

  consume_args(context, optind);

  const char *window_arg = "%1";
  if (!window_get_arg(context, 0, 0, &window_arg)) {
    fprintf(stderr, usage, cmd);
    return EXIT_FAILURE;
  }

  unsigned char *role;
  int role_len;
  int role_type;

  window_each(context, window_arg, {
    xdo_get_window_role(context->xdo, window, &role, &role_len, &role_type);
    xdotool_output(context, "%.*s", role_len, role);
    XFree(role);
  }); /* window_each(...) */
  return EXIT_SUCCESS;
}

