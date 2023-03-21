#ifndef PLOT_H
#define PLOT_H

/**
 * Container for information included in plot.
 */
typedef struct PlotInfo {
  int sim_time;     // How much time was simulated (in seconds).
  double int_step;  // The length of a simulation step (in milliseconds).
  int num_comps;    // The number of compartments simulated.
  int num_dendrs;   // The number of dendrites simulated.
  double exec_time; // How long the program took to execute.
  int slaves;       // How many slave processes were involved.
} PlotInfo;

/**
 * Name: plotData
 *
 * Description:
 * Plots data contained in given file. The file is expected to be formatted to
 * be processed by gnuplot.
 *
 * If `image_name' is NULL, gnuplot will plot to a window, otherwise, gnuplot
 * will plot to the filename specified by `image_name'.
 *
 * Parameters:
 * @param data_name     name of the data file to plot
 * @param image_name    name of the image file to plot to
 */
void plotData( PlotInfo *pinfo, char *data_name, char *image_name );

#endif
