/*
  File autogenerated by gengetopt version 2.10
  generated with the following command:
  gengetopt 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "getopt.h"

#include "cmdline.h"

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n", CMDLINE_PARSER_PACKAGE, CMDLINE_PARSER_VERSION);
}

void
cmdline_parser_print_help (void)
{
  cmdline_parser_print_version ();
  printf("\n"
  "Purpose:\n"
  "  Linux input layer scancode remapper\n"
  "\n"
  "Usage: %s [OPTIONS]...\n", CMDLINE_PARSER_PACKAGE);
  printf("   -h         --help           Print help and exit\n");
  printf("   -V         --version        Print version and exit\n");
  printf("   -s         --set            Set/merge the maping table (default=off)\n");
  printf("   -g         --get            Get the key mapping table (default=off)\n");
  printf("   -i         --info           Show driver information (default=off)\n");
  printf("   -dSTRING   --device=STRING  Specify the device to use\n");
  printf("   -v         --verbose        Enable verbose mode (default=off)\n");
}


static char *gengetopt_strdup (const char *s);

/* gengetopt_strdup() */
/* strdup.c replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info)
{
  int c;	/* Character of the parsed option.  */
  int missing_required_options = 0;

  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->set_given = 0 ;
  args_info->get_given = 0 ;
  args_info->info_given = 0 ;
  args_info->device_given = 0 ;
  args_info->verbose_given = 0 ;
#define clear_args() { \
  args_info->set_flag = 0;\
  args_info->get_flag = 0;\
  args_info->info_flag = 0;\
  args_info->device_arg = NULL; \
  args_info->verbose_flag = 0;\
}

  clear_args();

  optarg = 0;
  optind = 1;
  opterr = 1;
  optopt = '?';

  while (1)
    {
      int option_index = 0;
      char *stop_char;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "set",	0, NULL, 's' },
        { "get",	0, NULL, 'g' },
        { "info",	0, NULL, 'i' },
        { "device",	1, NULL, 'd' },
        { "verbose",	0, NULL, 'v' },
        { NULL,	0, NULL, 0 }
      };

      stop_char = 0;
      c = getopt_long (argc, argv, "hVsgid:v", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          clear_args ();
          cmdline_parser_print_help ();
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          clear_args ();
          cmdline_parser_print_version ();
          exit (EXIT_SUCCESS);

        case 's':	/* Set/merge the maping table.  */
          if (args_info->set_given)
            {
              fprintf (stderr, "%s: `--set' (`-s') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->set_given = 1;
          args_info->set_flag = !(args_info->set_flag);
          break;

        case 'g':	/* Get the key mapping table.  */
          if (args_info->get_given)
            {
              fprintf (stderr, "%s: `--get' (`-g') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->get_given = 1;
          args_info->get_flag = !(args_info->get_flag);
          break;

        case 'i':	/* Show driver information.  */
          if (args_info->info_given)
            {
              fprintf (stderr, "%s: `--info' (`-i') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->info_given = 1;
          args_info->info_flag = !(args_info->info_flag);
          break;

        case 'd':	/* Specify the device to use.  */
          if (args_info->device_given)
            {
              fprintf (stderr, "%s: `--device' (`-d') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->device_given = 1;
          args_info->device_arg = gengetopt_strdup (optarg);
          break;

        case 'v':	/* Enable verbose mode.  */
          if (args_info->verbose_given)
            {
              fprintf (stderr, "%s: `--verbose' (`-v') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->verbose_given = 1;
          args_info->verbose_flag = !(args_info->verbose_flag);
          break;


        case 0:	/* Long option with no short option */

        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          exit (EXIT_FAILURE);

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c\n", CMDLINE_PARSER_PACKAGE, c);
          abort ();
        } /* switch */
    } /* while */


  if ( missing_required_options )
    exit (EXIT_FAILURE);

  return 0;
}
