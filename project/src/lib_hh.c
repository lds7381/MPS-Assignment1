/*
  Multiple Processor Systems. Spring 2018
  Professor Muhammad Shaaban
  Author: Dmitri Yudanov (update: Dan Brandt)

  These are the main worker methods for Hodgkin Huxley (HH) simplified
  compartamental neuron model.

  Methods rk4Step and soma were originally
  developed by Stewart RD, Bair W. (2009)
  Spiking neural network simulation: numerical integration with the
  Parker-Sochacki method. J Comput Neurosci. ISSN: 0929-5313 (Print)
  1573-6873 (Online) DOI: 10.1007/s10827-008-0131-5 Springer Netherlands
  SpringerLink Date: Saturday, January 17, 2009. Posted on:
  http://senselab.med.yale.edu/ModelDB/ShowModel.asp?model=117361
*/

#include "lib_hh.h"
#include "constants.h"

#include <math.h>
#include <float.h>
#include <stdlib.h>

// Parameters for cell of 20,000 micometer surface area (2e-4 cm^2)
#define gL  10      // Leak soma conductance, nS
#define gLd 0.01    // Leak dendrite compartment conductance, nS
#define gK  6000    // K soma conductance, nS
#define gNa 20000   // Na soma conductance, nS
#define Cs  200     // Soma capacitance, pF
#define Cd  0.1     // Compartment dendrite capacitance, pF
#define ENa 50      // Na reversal potential, mV
#define EK -90      // K reversal potential, mV
#define EL -65      // Leak reversal potential, mV
#define Vr -65      // Resting membrane potential, mV

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
double dendriteStep( double *v_d, int seed, int num_comps, double delta_t,
                     double v_m )
{
  int i;
  double current, cur, temp[1], paramD[6], *vddt;

  vddt = (double*) malloc( sizeof(double) * (num_comps - 1) );
  paramD[0] = delta_t;

  srand(seed);

  // Current injected at the tip of the dendrite
  cur = INJCURMEAN + INJCURMEAN*0.1 -
        2*INJCURMEAN*0.1*((double)rand()/((double)RAND_MAX));
  // Update somatic potential = potential of the last compartment
  v_d[num_comps-1] = v_m;

  // Loop over compartments
  for( i=0; i < num_comps-2; i++ )
  {/*This loop computes lateral dVm for 1st interation in RK4*/

    if( i == 0 )
    {// First compartment: inject current, doesn't have resistance from the left
      paramD[1] = cur;
      paramD[2] = 0;
      paramD[3] = DENDRCONDCOMP + DENDRCONDDISTR/(num_comps-2-i);
    }
    else
    {// For all others: inj cur = 0, gradualy rised conductance towards soma
      paramD[1] = 0;
      paramD[2] = DENDRCONDCOMP + DENDRCONDDISTR/(num_comps-1-i);
      paramD[3] = DENDRCONDCOMP + DENDRCONDDISTR/(num_comps-2-i);
    }
    // obtaind first Vm
    paramD[4]= v_d[i];
    paramD[5]=  v_d[i+2];
    dendrite((vddt+i),(v_d+i+1),paramD);
  }

  // Loop over compartments
  for( i=0; i < num_comps-2; i++ )
  {/*This loops performs bulk RK4 and increment lateral Vm*/
    if( i == 0 )
    {// First compartment: inject current, doesn't have resistance from the left
      paramD[1] = cur;
      paramD[2] = 0;
      paramD[3] = DENDRCONDCOMP + DENDRCONDDISTR/(num_comps-2-i);
    }
    else
    {// For all others: inj cur = 0, gradualy rised conductance towards soma
      paramD[1] = 0;
      paramD[2] = DENDRCONDCOMP + DENDRCONDDISTR/(num_comps-1-i);
      paramD[3] = DENDRCONDCOMP + DENDRCONDDISTR/(num_comps-2-i);
    }
    // perform RK4 on Vm for this compartment
    paramD[4]= v_d[i];
    paramD[5]=  v_d[i+2];
    temp[0]=v_d[i+1];
    rk4Step((v_d+i+1),temp,(vddt+i),1,paramD,1,dendrite);
  }
  // Calculate current injected by this dendrite into soma
  current = paramD[3]*(v_d[i] - v_m);

  // Free malloced memory.
  free( vddt );

  return current;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void rk4Step( double *y, double *y0, double *dydt0, int nv, double *fp,
              double dt, void (*derivs)(double*, double*, double*) )
{
    int i;
    double const dt2 = dt/2;
    double const dt6 = dt/6;
    double *rk1,*rk2,*rk3,*dydt;

    rk1  = malloc( nv * sizeof(double) );
    rk2  = malloc( nv * sizeof(double) );
    rk3  = malloc( nv * sizeof(double) );
    dydt = malloc( nv * sizeof(double) );

    for (i = 0; i < nv; i++) { // 1
      rk1[i] = dydt0[i];
      y[i] = y0[i] + dt2*dydt0[i];
    }
    derivs(dydt, y, fp); // 2

    for (i = 0; i < nv; i++) {
      rk2[i] = dydt[i];
      y[i] = y0[i] + dt2*dydt[i];
    }
    derivs(dydt, y, fp); // 3

    for (i = 0; i < nv; i++) {
      rk3[i] = dydt[i];
      y[i] = y0[i] + dt*dydt[i];
    }
    derivs(dydt, y, fp); // 4

    for (i = 0; i < nv; i++) {
      y[i] = y0[i] + dt6*(rk1[i]+dydt[i]+2*(rk2[i]+rk3[i]));
    }

    free(rk1); free(rk2); free(rk3); free(dydt);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void soma( double *dydx, double *y, double *param )
{
  double alpha_n, beta_n, alpha_m, beta_m, alpha_h, beta_h;
  double const E_alpha_n = Vr + 15;
  double const E_beta_n  = Vr + 10;
  double const E_alpha_m = Vr + 13;
  double const E_beta_m  = Vr + 40;
  double const E_alpha_h = Vr + 17;
  double const E_beta_h  = Vr + 40;

  double v = y[0];
  double n = y[1];
  double m = y[2];
  double h = y[3];
  double n4 = n*n*n*n;
  double m3h = m*m*m*h;
  double dt = param[0];
  double I_inj = param[1];
  double I_dendr = param[2];

  dydx[0] = dt*(I_inj + I_dendr - gK*n4*(v-EK) -
            gNa*m3h*(v-ENa) - gL*(v-EL))/Cs;

  if (v == E_alpha_n) {   // protect against div by zero
    alpha_n = 0.032*5;
  } else {
    alpha_n = 0.032 * (E_alpha_n-v)/(exp((E_alpha_n-v)/5) - 1);
  }

  beta_n  = 0.5*exp((E_beta_n-v)/40);
  dydx[1] = dt*(alpha_n*(1-n) - beta_n*n);

  if (v == E_alpha_m) {   // protect against div by zero
    alpha_m = 0.32*4;
  } else {
    alpha_m = 0.32 * (E_alpha_m-v)/(exp((E_alpha_m-v)/4) - 1);
  }

  if (v == E_beta_m) {    // protect against div by zero
    beta_m = 0.28*5;
  } else {
    beta_m = 0.28 * (v-E_beta_m)/(exp((v-E_beta_m)/5) - 1);
  }

  dydx[2] = dt*(alpha_m*(1-m) - beta_m*m);
  alpha_h = 0.128 * exp((E_alpha_h-v)/18);
  beta_h  = 4 / (exp((E_beta_h-v)/5)+1);
  dydx[3] = dt*(alpha_h*(1-h) - beta_h*h);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void dendrite( double *dydx, double *y, double *param )
{
  double const dt       = param[0];
  double const I_inj    = param[1];
  double const gBefore  = param[2];
  double const gAfter   = param[3];
  double const yBefore  = param[4];
  double const yAfter   = param[5];

//*dydx = dt*(I_inj + gBefore*yBefore - (gBefore + gAfter)**y + gAfter*yAfter -
//        (gLd/Cscale)*(*y-EL))/(Cd/Cscale);
  *dydx = dt*(I_inj + gBefore*yBefore - (gBefore + gAfter)**y + gAfter*yAfter -
          (gLd)*(*y-EL))/(Cd);
}
