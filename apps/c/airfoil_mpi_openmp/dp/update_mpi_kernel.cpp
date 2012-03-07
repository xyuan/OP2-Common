//
// auto-generated by op2.m on 30-May-2011 22:03:11
//

#include <op_rt_support.h>

// user function

#include "update.h"


// x86 kernel function

void op_x86_update(
  double *arg0,
  double *arg1,
  double *arg2,
  double *arg3,
  double *arg4,
  int   start,
  int   finish ) {


  // process set elements

  for (int n=start; n<finish; n++) {

    // user-supplied kernel call

    update( arg0+n*4,
            arg1+n*4,
            arg2+n*4,
            arg3+n*1,
            arg4 );
  }
}


// host stub function

void op_par_loop_update(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4 ){

  int ninds   = 0;
  int nargs   = 5;
  op_arg args[5] = {arg0,arg1,arg2,arg3,arg4};

  double *arg4h = (double *)arg4.data;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  update \n");
  }

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timers_core(&cpu_t1, &wall_t1);

  // set number of threads

#ifdef _OPENMP
  int nthreads = omp_get_max_threads( );
#else
  int nthreads = 1;
#endif

  // allocate and initialise arrays for global reduction

  double arg4_l[1+64*64];
  for (int thr=0; thr<nthreads; thr++)
    for (int d=0; d<1; d++) arg4_l[d+thr*64]=ZERO_double;

  // execute plan

#pragma omp parallel for
  for (int thr=0; thr<nthreads; thr++) {
    int start  = (set->size* thr   )/nthreads;
    int finish = (set->size*(thr+1))/nthreads;
    op_x86_update( (double *) arg0.data,
                   (double *) arg1.data,
                   (double *) arg2.data,
                   (double *) arg3.data,
                   arg4_l + thr*64,
                   start, finish );
  }

  // combine reduction data

  for (int thr=0; thr<nthreads; thr++)
    for(int d=0; d<1; d++) arg4h[d] += arg4_l[d+thr*64];

  //set dirty bit on direct/indirect datasets with access OP_INC,OP_WRITE, OP_RW
  for(int i = 0; i<nargs; i++)
      if(args[i].argtype == OP_ARG_DAT)
        set_dirtybit(args[i]);

  //performe any global operations
  for(int i = 0; i<nargs; i++)
      if(args[i].argtype == OP_ARG_GBL)
        global_reduce(&args[i]);



  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  op_timing_realloc(4);
  OP_kernels[4].name      = name;
  OP_kernels[4].count    += 1;
  OP_kernels[4].time     += wall_t2 - wall_t1;
  OP_kernels[4].transfer += (double)set->size * arg0.size;
  OP_kernels[4].transfer += (double)set->size * arg1.size;
  OP_kernels[4].transfer += (double)set->size * arg2.size * 2.0f;
  OP_kernels[4].transfer += (double)set->size * arg3.size;
}

