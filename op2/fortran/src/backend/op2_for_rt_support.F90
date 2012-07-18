! This module defines the interoperable data structures of the OP2 RT support
! (i.e. plan struct) and defines the interface for the C plan function

module OP2_Fortran_RT_Support
#ifdef OP2_WITH_CUDAFOR
  use cudafor
#endif

  use, intrinsic :: ISO_C_BINDING

  integer(kind=c_int), parameter :: F_OP_ARG_DAT = 0
  integer(kind=c_int), parameter :: F_OP_ARG_GBL = 1

  type, BIND(C) :: op_plan

    ! input arguments
    type(c_ptr) ::         name
    type(c_ptr) ::         set
    integer(kind=c_int) :: nargs, ninds, part_size
    type(c_ptr) ::         in_maps
    type(c_ptr) ::         dats
    type(c_ptr) ::         idxs
    type(c_ptr) ::         accs

    ! execution plan
#ifdef OP2_WITH_CUDAFOR
    type(c_devptr) ::      nthrcol ! number of thread colors for each block
    type(c_devptr) ::      thrcol  ! thread colors
    type(c_devptr) ::      offset  ! offset for primary set
#else
    type(c_ptr) ::         nthrcol ! number of thread colors for each block
    type(c_ptr) ::         thrcol  ! thread colors
    type(c_ptr) ::         offset  ! offset for primary set
#endif

#ifdef OP2_WITH_CUDAFOR
    type(c_devptr) ::      ind_map ! concatenated pointers for indirect datasets
#else
    type(c_ptr) ::         ind_map ! concatenated pointers for indirect datasets
#endif

    type(c_ptr) ::         ind_maps ! pointers for indirect datasets
#ifdef OP2_WITH_CUDAFOR
    type(c_devptr) ::      ind_offs ! offsets for indirect datasets
    type(c_devptr) ::      ind_sizes ! offsets for indirect datasets
#else
    type(c_ptr) ::         ind_offs ! offsets for indirect datasets
    type(c_ptr) ::         ind_sizes ! offsets for indirect datasets
#endif
    type(c_ptr) ::         nindirect ! size of ind_maps (for Fortran)

#ifdef OP2_WITH_CUDAFOR
    type(c_devptr) ::      loc_map ! concatenated maps to local indices, renumbered as needed 
#else
    type(c_ptr) ::         loc_map ! concatenated maps to local indices, renumbered as needed 
#endif

    type(c_ptr) ::         maps ! maps to local indices, renumbered as needed
    integer(kind=c_int) :: nblocks ! number of blocks (for Fortran)
#ifdef OP2_WITH_CUDAFOR
    type(c_devptr) ::      nelems ! number of elements in each block
#else
    type(c_ptr) ::         nelems ! number of elements in each block
#endif
    integer(kind=c_int) :: ncolors_core ! mumber of core colors in MPI
    integer(kind=c_int) :: ncolors_owned ! number of colors in MPI for blocks that only have owned elements
    integer(kind=c_int) :: ncolors ! number of block colors
    type(c_ptr) ::         ncolblk  ! number of blocks for each color

#ifdef OP2_WITH_CUDAFOR
    type(c_devptr) ::      blkmap ! block mapping
#else
    type(c_ptr) ::         blkmap ! block mapping
#endif

    type(c_ptr) ::         nsharedCol ! bytes of shared memory required per block colour
    integer(kind=c_int) :: nshared ! bytes of shared memory required
    real(kind=c_float) ::  transfer ! bytes of data transfer per kernel call
    real(kind=c_float) ::  transfer2 ! bytes of cache line per kernel call
    integer(kind=c_int) :: count ! number fo times called (should not work for fortran?)
  end type op_plan


  interface

    ! C wrapper to plan function for Fortran
    type(c_ptr) function FortranPlanCaller (name, set, partitionSize, argsNumber, args, indsNumber, inds) &
      & BIND(C,name='FortranPlanCaller')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      character(kind=c_char) ::     name(*)    ! name of kernel
      type(c_ptr), value ::         set        ! iteration set
      integer(kind=c_int), value :: partitionSize
      integer(kind=c_int), value :: argsNumber ! number of op_dat arguments to op_par_loop
      type(op_arg), dimension(*) :: args       ! array with op_args
      integer(kind=c_int), value :: indsNumber ! number of arguments accessed indirectly via a map

      ! indexes for indirectly accessed arguments (same indrectly accessed argument = same index)
      integer(kind=c_int), dimension(*) :: inds

    end function FortranPlanCaller

    integer(kind=c_int) function getSetSizeFromOpArg (arg) BIND(C,name='getSetSizeFromOpArg')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      type(op_arg) :: arg

    end function

    subroutine op_partition_c (lib_name, lib_routine, prime_set, prime_map, coords) BIND(C,name='op_partition')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations 

      character(kind=c_char) :: lib_name(*)
      character(kind=c_char) :: lib_routine(*)

      type(op_set_core) :: prime_set
      type(op_map_core) :: prime_map
      type(op_dat_core) :: coords

    end subroutine

    integer(kind=c_int) function op_mpi_halo_exchanges (set, argsNumber, args) BIND(C,name='op_mpi_halo_exchanges')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      type(c_ptr), value ::         set        ! iteration set
      integer(kind=c_int), value :: argsNumber ! number of op_dat arguments to op_par_loop
      type(op_arg), dimension(*) :: args       ! array with op_args

    end function

    subroutine op_mpi_wait_all (argsNumber, args) BIND(C,name='op_mpi_wait_all')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      integer(kind=c_int), value :: argsNumber ! number of op_dat arguments to op_par_loop
      type(op_arg), dimension(*) :: args       ! array with op_args

    end subroutine

    subroutine op_mpi_set_dirtybit (argsNumber, args) BIND(C,name='op_mpi_set_dirtybit')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      integer(kind=c_int), value :: argsNumber ! number of op_dat arguments to op_par_loop
      type(op_arg), dimension(*) :: args       ! array with op_args

    end subroutine

    subroutine op_mpi_reduce_int (arg, data) BIND(C,name='op_mpi_reduce_int')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      type(op_arg) :: arg
      type(c_ptr) :: data

    end subroutine

    subroutine op_mpi_reduce_double (arg, data) BIND(C,name='op_mpi_reduce_double')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      type(op_arg) :: arg
      type(c_ptr) :: data

    end subroutine

    subroutine op_mpi_reduce_float (arg, data) BIND(C,name='op_mpi_reduce_float')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      type(op_arg) :: arg
      type(c_ptr) :: data

    end subroutine

    ! commented while waiting for C-side support
    ! subroutine op_mpi_reduce_bool (arg, data) BIND(C,name='op_mpi_reduce_bool')

    !   use, intrinsic :: ISO_C_BINDING
    !   use OP2_Fortran_Declarations

    !   type(op_arg) :: arg
    !   type(c_ptr) :: data

    ! end subroutine

    ! debugging routines
    subroutine op_dump_arg (arg) BIND(C,name='op_dump_arg')

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations

      type(op_arg) :: arg

    end subroutine

  end interface

  contains

    subroutine op_partition (lib_name, lib_routine, prime_set, prime_map, coords)

      use, intrinsic :: ISO_C_BINDING
      use OP2_Fortran_Declarations 

      implicit none

      character(kind=c_char) :: lib_name(*)
      character(kind=c_char) :: lib_routine(*)

      type(op_set) :: prime_set
      type(op_map) :: prime_map
      type(op_dat) :: coords

      call op_partition_c (lib_name, lib_routine, prime_set%setPtr, prime_map%mapPtr, coords%dataPtr)

    end subroutine

end module OP2_Fortran_RT_Support