################################################################################
#include <string.h>
# Variables common to all programs.
CC = gcc
MPICC = mpicc

FLAGS = -Wextra -Wall -Iinclude

COMMON_SRC = lib_hh.c plot.c cmd_args.c

LIBS = -lm
DEFINES = PLOT_PNG
DEFINES := $(addprefix -D,$(DEFINES))

################################################################################
# Variables used by sequential code.
SEQ_BIN = seq_hh
SEQ_SRC = seq_hh.c $(COMMON_SRC)

SEQ_SRC := $(addprefix src/,$(SEQ_SRC))

################################################################################
# Variables used by MPI code.
MPI_BIN = mpi_hh
MPI_SRC = mpi_hh.c $(COMMON_SRC)

MPI_SRC := $(addprefix src/,$(MPI_SRC))

all: $(SEQ_BIN) $(MPI_BIN)

$(SEQ_BIN): $(SEQ_SRC)
	$(CC) $(SEQ_SRC) $(FLAGS) $(DEFINES) $(LIBS) -o $(SEQ_BIN)

$(MPI_BIN): $(MPI_SRC)
	$(MPICC) $(MPI_SRC) $(FLAGS) $(DEFINES) $(LIBS) -o $(MPI_BIN)

clean:
	rm -f $(SEQ_BIN) $(MPI_BIN)
