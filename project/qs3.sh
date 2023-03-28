#!/bin/bash
#

# This is an example bash script that is used to submit a job
# to the cluster.
#
# Typcially, the # represents a comment.However, #SBATCH is
# interpreted by SLURM to give an option from above. As you 
# will see in the following lines, it is very useful to provide
# that information here, rather than the command line.

# Name of the job - You MUST use a unique name for the job
#SBATCH -J qs2_13

# Standard out and Standard Error output files
# Each job should have a unique file name; otherwise, all of the
# output goes to one file and becomes hard to read/analyze.
# The default file name looks like <job name>-<job ID>.out.
#SBATCH -o %x-%j.out
#SBATCH -e %x-%j.err

# Multiple options can be used on the same line as shown below.
# Here, we set the partition and number of cores to use,
# and specify the amount of memory we would like per core.
#SBATCH -p kgcoe-mps -n 13
#SBATCH --mem-per-cpu 1G

#
# Your job script goes below this line.
#

# If the job that you are submitting is not sequential,
# then you MUST provide this line...it tells the node(s)
# that you want to use this implementation of MPI. If you
# omit this line, your results will indicate failure.
spack load --first openmpi

# Place your srun command here
# Notice that you have to provide the number of processes that
# are needed. This number needs to match the number of cores
# indicated by the -n option. If these do not, your results will
# not be valid or you may have wasted resources that others could
# have used. Using $SLURM_NPROCS guarantees a match.

# Question Set 1

# 12 slave runs
srun -n $SLURM_NPROCS mpi_hh -d 15 -c 10
srun -n $SLURM_NPROCS mpi_hh -d 150 -c 10
srun -n $SLURM_NPROCS mpi_hh -d 1500 -c 10
