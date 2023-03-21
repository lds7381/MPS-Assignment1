#ifndef CONSTANTS
#define CONSTANTS

// This constants relate to the simulation model.
#define STEPS 10000
#define NUMVAR 4            // Number of parameters passed to stepper for soma
#define COMPTIME 100        // Time for model to run, ms
#define VREST -65           // Resting membrane potential
#define INJCURMEAN 100      // Dendrite ijected current mean, pA
#define DENDRCONDCOMP 1000  // Lateral compartmental conductance, nS
#define DENDRCONDDISTR 100  // Deviation of compartmental conductance, nS

#define FNAME_LEN 80        // Filename lengths.

#endif
