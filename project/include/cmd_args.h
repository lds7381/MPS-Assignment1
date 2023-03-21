#ifndef CMD_ARGS_H
#define CMD_ARGS_H

/**
 * Container for values given in the command line.
 */
typedef struct CmdArgs {
  int num_dendrs; // The number of dendrites to simulate.
  int num_comps;  // The number of compartments per dendrite.
} CmdArgs;

/**
 * Name: usage
 *
 * Description:
 * Prints a simple usage statement for the program.
 *
 * Parameters:
 * @param name      the name used to call this program (i.e., argv[0])
 */
void usage( char *name );

/**
 * Name: parseArgs
 *
 * Description:
 * Parses the command line arguments.
 *
 * Parameters:
 * @param cmd_args  where to store extracted arguments
 * @param argc      the argc parameter (number of arguments)
 * @param argv      the arguments themselves
 *
 * Returns:
 * @return int      0 if there was a problem, nonzero otherwise
 */
int parseArgs( CmdArgs *cmd_args, int argc, char **argv ); 

#endif
