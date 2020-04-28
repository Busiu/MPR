#!/bin/bash -l

#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive
#SBATCH --time=02:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgwbus2020a

module add plgrid/tools/openmpi

mpicc -std=c99 -o scalable scalable.c
echo -e "Scalable for large n: 1600000000" > "large_scalable.txt"

for ((i = 1; i <= 12; i++)); do
    echo -e "NP = $i" >> "large_scalable.txt"
    for ((j = 1; j <= 4; j++)); do
        mpiexec -np $i ./scalable 1600000000 >> "large_scalable.txt"
    done
done
