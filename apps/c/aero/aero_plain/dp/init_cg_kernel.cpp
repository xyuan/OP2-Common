//
// auto-generated by op2.m on 12-Jun-2012 19:13:14
//

// user function

#include "init_cg.h"


// x86 kernel function

void op_x86_init_cg(
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


    init_cg(  arg0+n*1,
              arg1,
              arg2+n*1,
              arg3+n*1,
              arg4+n*1 );
  }
}


// host stub function

void op_par_loop_init_cg(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4 ){

  double *arg1h = (double *)arg1.data;

  int    nargs   = 5;
  op_arg args[5];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  init_cg\n");
  }

  op_mpi_halo_exchanges(set, nargs, args);

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

  double arg1_l[1+64*64];
  for (int thr=0; thr<nthreads; thr++)
    for (int d=0; d<1; d++) arg1_l[d+thr*64]=ZERO_double;

  if (set->size >0) {


  // execute plan

#pragma omp parallel for
  for (int thr=0; thr<nthreads; thr++) {
    int start  = (set->size* thr   )/nthreads;
    int finish = (set->size*(thr+1))/nthreads;
    op_x86_init_cg( (double *) arg0.data,
                    arg1_l + thr*64,
                    (double *) arg2.data,
                    (double *) arg3.data,
                    (double *) arg4.data,
                    start, finish );
  }

  }


  // combine reduction data

  for (int thr=0; thr<nthreads; thr++)
    for(int d=0; d<1; d++) arg1h[d] += arg1_l[d+thr*64];

  op_mpi_reduce(&arg1,arg1h);

  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  op_timing_realloc(2);
  OP_kernels[2].name      = name;
  OP_kernels[2].count    += 1;
  OP_kernels[2].time     += wall_t2 - wall_t1;
  OP_kernels[2].transfer += (float)set->size * arg0.size;
  OP_kernels[2].transfer += (float)set->size * arg2.size;
  OP_kernels[2].transfer += (float)set->size * arg3.size;
  OP_kernels[2].transfer += (float)set->size * arg4.size;
}

