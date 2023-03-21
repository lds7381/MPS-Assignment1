#include "plot.h"

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void plotData( PlotInfo *pinfo, char *data_name, char *image_name )
{
  FILE *pipe = popen("gnuplot -persist","w");
  if (pipe == NULL) {
    // Something went wrong.
    fprintf( stderr, "Could not start gnuplot process!\n" );
    return;
  }

  if (image_name) {
    fprintf(pipe, "set terminal png\n");
    fprintf(pipe, "set output '%s'\n", image_name);
  }

  fprintf( pipe, "set title \"Membrane Potential\\n"
                 "Simulation time: %d ms, Integration step: %f ms,\\n"
                 "Compartments: %d, Dendrites: %d, Execution time: %f s,\\n"
                 "Slave processes: %d\"\n",
           pinfo->sim_time, pinfo->int_step,
           pinfo->num_comps, pinfo->num_dendrs,
           pinfo->exec_time, pinfo->slaves );
  fprintf( pipe, "set xlabel 'Time, ms'\n" );
  fprintf( pipe, "set ylabel 'Vm, mV'\n" );
  fprintf( pipe, "unset key\n" );
  fprintf( pipe, "plot '%s' using 1:2 with lines\n", data_name );
  pclose( pipe );
}
