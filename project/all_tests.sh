#!/bin/bash
# queues all jobs for all question sets
./qs1.sh
./qs2.sh
sbatch qs3.sh
sbatch qs4.sh
