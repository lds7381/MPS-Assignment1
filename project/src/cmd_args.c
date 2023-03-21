#include "cmd_args.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void usage( char *name )
{
  printf(
"USAGE:\n"
"  %s [-h] [-d NUM_DENDR] [-c NUM_COMPARTMENTS]\n"
"\n"
"DESCRIPTION:\n"
"  Simulates a neuron using a Hodgkin Huxley simplified compartamental neuron\n"
"  model.\n"
"\n"
"  Simulation data will be stored in the `data/' directory. If this directory\n"
"  does not exist, it will be created. Results are stored in such a way that\n"
"  they can be easily plotted with gnuplot. The filename for the results is\n"
"  determined from the time at which the simulation was run.\n"
"\n"
"  The results filename will be of the form:\n"
"     data/pWWdXXcYY_MMDDYY_HHMMSS.dat\n"
"  where 'WW' is the number of processes used, 'XX' the number of dendrites,\n"
"  'YY' the number of compartments, and 'MMDDYY_...' the time at which the\n"
"  simulation was run.\n"
"\n"
#ifdef PLOT_SCREEN
"  Simulation results will also be plotted, using gnuplot, to the screen.\n"
"\n"
#endif
#ifdef PLOT_PNG
"  Simulation results will also be printed to a PNG file, using the same\n"
"  filename as the results data file, and stored under the `graphs/'\n"
"  directory.\n"
"\n"
#endif
"OPTIONS:\n"
"  -h, --help\n"
"    Print this usage statement and exit.\n"
"\n"
"  -d, --dendrites\n"
"    The number of dendrites to simulate. Must be greater than 0. Defaults to\n"
"    one.\n"
"\n"
"  -c, --compartments\n"
"    The number of compartments per dendrite. Must be greater than 0. Default\n"
"    is one.\n"
"\n"
, name );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int parseArgs( CmdArgs *cmd_args, int argc, char **argv )
{
  int i;

  // Setup default values.
  cmd_args->num_dendrs = 1;
  cmd_args->num_comps  = 1;

  // Define a macro to make checking parameters easier.
  #define PARAM_EQUALS( sn, ln ) (strcmp( (sn), argv[i] ) == 0 ||\
                                  strcmp( (ln), argv[i] ) == 0)

  // See if the help flag was given.
  for (i = 1; i < argc; i++) {
    if (PARAM_EQUALS( "-h", "--help" )) {
      usage( argv[0] );
      return 0;
    }
  }

  // Extract and verify parameters.
  for (i = 1; i < argc;) {
    if (PARAM_EQUALS( "-d", "--dendrites")) {
      cmd_args->num_dendrs = atoi( argv[i+1] );

      if (cmd_args->num_dendrs <= 0) {
        fprintf(stderr, "Number of dendrites must be greater than 0!\n");
        fprintf(stderr, "Number of dendrites default to 1!\n");
        cmd_args->num_dendrs = 1;
      }

      i += 2;
    } else if (PARAM_EQUALS( "-c", "--compartments" )) {
      cmd_args->num_comps = atoi( argv[i+1] );

      if (cmd_args->num_comps <= 0) {
        fprintf(stderr, "Number of compartments must be greater than 0!\n");
        fprintf(stderr, "Number of compartments default to 1!\n");
        cmd_args->num_comps = 1;
      }

      i += 2;
    } else {
      // Unknown parameter.
      usage( argv[0] );
      return 0;
    }
  }

  // Everything seems hunky dorey.
  return 1;
}
