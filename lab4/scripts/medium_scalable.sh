#!/bin/bash -l

#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgwbus2020a

module add plgrid/tools/openmpi

mpicc -std=c99 -o scalable scalable.c
echo -e "Scalable for medium n: 100000000" > "medium_scalable.txt"

for ((i = 1; i <= 12; i++)); do
    echo -e "NP = $i" >> "medium_scalable.txt"
    for ((j = 1; j <= 10; j++)); do
        mpiexec -np $i ./scalable 100000000 >> "medium_scalable.txt"
    done
done
