IC = nvcc
FC = pgfortran

IC_OPT = -lcudart -O3 -use_fast_math
FC_OPT = -Mcuda=cuda3.1 -Minform=inform -fast -O4 -Mcuda=fastmath

F_OP2_LIB = $(OP2)/fortran/lib
F_OP2_MOD = $(OP2)/fortran/mod


# the following generates corresponding intermediate C CUDA code
# -Mcuda=keepgpu

all: airfoil_comp airfoil_link

airfoil_comp: OP2Profiling.F95 constvars.F95 airfoil_seqfun.F95 update_cuda_module.CUF bres_calc_cuda_module.CUF res_calc_cuda_module.CUF adt_calc_cuda_module.CUF input.F95 output.F95 save_soln_cuda_module.CUF airfoil.F95
	$(FC) $(FC_OPT) -module $(F_OP2_MOD) -c OP2Profiling.F95 constantVarsCuda.CUF cudaConfigurationParams.F95 constvars.F95 op2_c.F95 airfoil_seqfun.F95 input.F95 output.F95 update_cuda_module.CUF bres_calc_cuda_module.CUF res_calc_cuda_module.CUF save_soln_cuda_module.CUF adt_calc_cuda_module.CUF airfoil.F95

airfoil_link: OP2Profiling.o constantVarsCuda.o debug.o constvars.o airfoil_seqfun.o input.o output.o update_cuda_module.o bres_calc_cuda_module.o res_calc_cuda_module.o save_soln_cuda_module.o adt_calc_cuda_module.o airfoil.o
	$(FC) $(FC_OPT) -L(F_OP2_LIB) OP2Profiling.o constantVarsCuda.o debug.o constvars.o airfoil_seqfun.o input.o output.o update_cuda_module.o bres_calc_cuda_module.o res_calc_cuda_module.o save_soln_cuda_module.o adt_calc_cuda_module.o airfoil.o -lop2_for_cuda -lop2_for_rt_support -o airfoil_cuda


clean:
	rm -f *.o
	rm -f *.mod
	rm -f airfoil_cuda