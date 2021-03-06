//
// auto-generated by op2.m on 28-Jun-2012 11:01:03
//

// user function

#include "res_calc.h"


// x86 kernel function

void op_x86_res_calc(
  int    blockIdx,
  float *ind_arg0,
  float *ind_arg1,
  float *ind_arg2,
  float *ind_arg3,
  int   *ind_map,
  short *arg_map,
  int   *ind_arg_sizes,
  int   *ind_arg_offs,
  int    block_offset,
  int   *blkmap,
  int   *offset,
  int   *nelems,
  int   *ncolors,
  int   *colors,
  int   set_size) {

  float arg6_l[4];
  float arg7_l[4];

  int   *ind_arg0_map, ind_arg0_size;
  int   *ind_arg1_map, ind_arg1_size;
  int   *ind_arg2_map, ind_arg2_size;
  int   *ind_arg3_map, ind_arg3_size;
  float *ind_arg0_s;
  float *ind_arg1_s;
  float *ind_arg2_s;
  float *ind_arg3_s;
  int    nelem, offset_b;

  char shared[128000];

  if (0==0) {

    // get sizes and shift pointers and direct-mapped data

    int blockId = blkmap[blockIdx + block_offset];
    nelem    = nelems[blockId];
    offset_b = offset[blockId];

    ind_arg0_size = ind_arg_sizes[0+blockId*4];
    ind_arg1_size = ind_arg_sizes[1+blockId*4];
    ind_arg2_size = ind_arg_sizes[2+blockId*4];
    ind_arg3_size = ind_arg_sizes[3+blockId*4];

    ind_arg0_map = &ind_map[0*set_size] + ind_arg_offs[0+blockId*4];
    ind_arg1_map = &ind_map[2*set_size] + ind_arg_offs[1+blockId*4];
    ind_arg2_map = &ind_map[4*set_size] + ind_arg_offs[2+blockId*4];
    ind_arg3_map = &ind_map[6*set_size] + ind_arg_offs[3+blockId*4];

    // set shared memory pointers

    int nbytes = 0;
    ind_arg0_s = (float *) &shared[nbytes];
    nbytes    += ROUND_UP(ind_arg0_size*sizeof(float)*2);
    ind_arg1_s = (float *) &shared[nbytes];
    nbytes    += ROUND_UP(ind_arg1_size*sizeof(float)*4);
    ind_arg2_s = (float *) &shared[nbytes];
    nbytes    += ROUND_UP(ind_arg2_size*sizeof(float)*1);
    ind_arg3_s = (float *) &shared[nbytes];
  }

  // copy indirect datasets into shared memory or zero increment

  for (int n=0; n<ind_arg0_size; n++)
    for (int d=0; d<2; d++)
      ind_arg0_s[d+n*2] = ind_arg0[d+ind_arg0_map[n]*2];

  for (int n=0; n<ind_arg1_size; n++)
    for (int d=0; d<4; d++)
      ind_arg1_s[d+n*4] = ind_arg1[d+ind_arg1_map[n]*4];

  for (int n=0; n<ind_arg2_size; n++)
    for (int d=0; d<1; d++)
      ind_arg2_s[d+n*1] = ind_arg2[d+ind_arg2_map[n]*1];

  for (int n=0; n<ind_arg3_size; n++)
    for (int d=0; d<4; d++)
      ind_arg3_s[d+n*4] = ZERO_float;


  // process set elements

  for (int n=0; n<nelem; n++) {

    // initialise local variables

    for (int d=0; d<4; d++)
      arg6_l[d] = ZERO_float;
    for (int d=0; d<4; d++)
      arg7_l[d] = ZERO_float;




    // user-supplied kernel call


    res_calc(  ind_arg0_s+arg_map[0*set_size+n+offset_b]*2,
               ind_arg0_s+arg_map[1*set_size+n+offset_b]*2,
               ind_arg1_s+arg_map[2*set_size+n+offset_b]*4,
               ind_arg1_s+arg_map[3*set_size+n+offset_b]*4,
               ind_arg2_s+arg_map[4*set_size+n+offset_b]*1,
               ind_arg2_s+arg_map[5*set_size+n+offset_b]*1,
               arg6_l,
               arg7_l );

    // store local variables

    int arg6_map = arg_map[6*set_size+n+offset_b];
    int arg7_map = arg_map[7*set_size+n+offset_b];

    for (int d=0; d<4; d++)
      ind_arg3_s[d+arg6_map*4] += arg6_l[d];

    for (int d=0; d<4; d++)
      ind_arg3_s[d+arg7_map*4] += arg7_l[d];
  }

  // apply pointered write/increment

  for (int n=0; n<ind_arg3_size; n++)
    for (int d=0; d<4; d++)
      ind_arg3[d+ind_arg3_map[n]*4] += ind_arg3_s[d+n*4];

}


// host stub function

void op_par_loop_res_calc(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7 ){


  int    nargs   = 8;
  op_arg args[8];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;

  int    ninds   = 4;
  int    inds[8] = {0,0,1,1,2,2,3,3};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: res_calc\n");
  }

  // get plan

  #ifdef OP_PART_SIZE_2
    int part_size = OP_PART_SIZE_2;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timers_core(&cpu_t1, &wall_t1);

  if (set->size >0) {


    op_plan *Plan = op_plan_get(name,set,part_size,nargs,args,ninds,inds);
    // execute plan

    int block_offset = 0;

    for (int col=0; col < Plan->ncolors; col++) {
      if (col==Plan->ncolors_core) op_mpi_wait_all(nargs, args);

      int nblocks = Plan->ncolblk[col];

#pragma omp parallel for
      for (int blockIdx=0; blockIdx<nblocks; blockIdx++)
      op_x86_res_calc( blockIdx,
         (float *)arg0.data,
         (float *)arg2.data,
         (float *)arg4.data,
         (float *)arg6.data,
         Plan->ind_map,
         Plan->loc_map,
         Plan->ind_sizes,
         Plan->ind_offs,
         block_offset,
         Plan->blkmap,
         Plan->offset,
         Plan->nelems,
         Plan->nthrcol,
         Plan->thrcol,
         set_size);

      block_offset += nblocks;
    }

  op_timing_realloc(2);
  OP_kernels[2].transfer  += Plan->transfer;
  OP_kernels[2].transfer2 += Plan->transfer2;

  }


  // combine reduction data

  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  op_timing_realloc(2);
  OP_kernels[2].name      = name;
  OP_kernels[2].count    += 1;
  OP_kernels[2].time     += wall_t2 - wall_t1;
}

