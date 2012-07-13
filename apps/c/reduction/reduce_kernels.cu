//
// auto-generated by op2.m on 09-Jul-2012 15:36:25
//

// header

#include "op_lib_cpp.h"

#include "op_cuda_rt_support.h"
#include "op_cuda_reduction.h"
// global constants

#ifndef MAX_CONST_SIZE
#define MAX_CONST_SIZE 128
#endif


void op_decl_const_char(int dim, char const *type,
            int size, char *dat, char const *name){
  cutilSafeCall(cudaMemcpyToSymbol(name, dat, dim*size));
}

// user kernel files

#include "count_kernel.cu"
#include "addto_kernel.cu"
#include "addto_id_kernel.cu"