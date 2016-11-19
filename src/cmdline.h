/* cmdline.h */

/* File autogenerated by gengetopt version 2.10  */

#ifndef CMDLINE_H
#define CMDLINE_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
#define CMDLINE_PARSER_PACKAGE PACKAGE
#endif

#ifndef CMDLINE_PARSER_VERSION
#define CMDLINE_PARSER_VERSION VERSION
#endif

struct gengetopt_args_info
{
  int set_flag;	/* Set/merge the maping table (default=off).  */
  int get_flag;	/* Get the key mapping table (default=off).  */
  int info_flag;	/* Show driver information (default=off).  */
  char * device_arg;	/* Specify the device to use.  */
  int verbose_flag;	/* Enable verbose mode (default=off).  */

  int help_given ;	/* Whether help was given.  */
  int version_given ;	/* Whether version was given.  */
  int set_given ;	/* Whether set was given.  */
  int get_given ;	/* Whether get was given.  */
  int info_given ;	/* Whether info was given.  */
  int device_given ;	/* Whether device was given.  */
  int verbose_given ;	/* Whether verbose was given.  */

} ;

int cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info);

void cmdline_parser_print_help(void);
void cmdline_parser_print_version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CMDLINE_H */
