/*
  Multiple Processor Systems. Spring 2018
  Professor Muhammad Shaaban
  Author: Dmitri Yudanov (update: Dan Brandt)

  Header file to accompany lib_hh.c
*/

#ifndef LIB_HH_H
#define LIB_HH_H

/**
 * Name: dendriteStep
 *
 * Description:
 * ??? This method perform simulation of a single dendritic compartment ???
 *
 * On entry `v_d' must point to the membrane potential from the previous step.
 * On exit, `v_d' will point to the membrane potential from this step.
 *
 * Parameters:
 * @param v_d           (INOUT) membrane potential
 * @param seed          (INPUT) seed for random number generator
 * @param num_comps     (INPUT) number of compartments in dendrite
 * @param delta_t       (INPUT) integration time step size
 * @param v_m           (INPUT) ??? 'Vm of compartment of this dendrite is
 *                              actually soma Vm' ???
 * Returns:
 * @return double       current injected by this dendrite into soma
 */
double dendriteStep( double *v_d, int seed, int num_comps, double delta_t,
                     double v_m );

/**
 * Name: rk4Step
 *
 * Description:
 * ??? Non-adaptive 4th order Runge-Kutta stepper ???
 *
 * Though the 'y0' parameter is supposed to be a copy of the current value of
 * the 'y' parameter, it is not safe to call this function with the same value
 * for 'y' and 'y0', e.g.
 *
 *  rk4Step( foo, foo, ...
 *
 * will give invalid results.
 *
 * Parameters:
 * @param y         (OUTPUT) ??? parameters subject to dy+ ???
 * @param y0        (INPUT) ??? copy of 'y' ???
 * @param dydt0     (INPUT) ??? storage for dy ???
 * @param nv        (INPUT) size of y, y0, and dydt0
 * @param fp        (INPUT) ??? model parameters ???
 * @param dt        (INPUT) ??? integration step ???
 * @param derivs    (INPUT) ??? model computation method ???
 */
void rk4Step( double *y, double *y0, double *dydt0, int nv, double *fp,
              double dt, void (*derivs)(double *,double *,double *) );

/**
 * Name: soma
 *
 * Description:
 * ??? Computes soma parameters for HH model ???
 *
 * Parameters:
 * @param dydx    (OUTPUT) ??? where to store dydx ???
 * @param y       (INPUT)  ??? model parameters subject to dy ???
 * @param param   (INPUT)  ??? model parameters ???
 */
void soma( double *dydx, double *y, double *param );

/**
 * Name: dendrite
 *
 * Description:
 * ??? Computes single compartment dendrite parameters ???
 *
 * Parameters:
 * @param dydx    (OUTPUT) ??? where to store dydx ???
 * @param y       (INPUT)  ??? model parameters subject to dy ???
 * @param param   (INPUT)  ??? model parameters ???
 */
void dendrite( double *y, double *dydx, double *param );

#endif
