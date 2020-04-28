#!/bin/bash -l

#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgwbus2020a

module add plgrid/tools/openmpi

mpicc -std=c99 -o non_scalable non_scalable.c
echo -e "Non-scalable for medium n: 1000000000" > "medium_non_scalable.txt"

for ((i = 1; i <= 12; i++)); do
    echo -e "NP = $i" >> "medium_non_scalable.txt"
    for ((j = 1; j <= 10; j++)); do
        mpiexec -np $i ./non_scalable 1000000000 >> "medium_non_scalable.txt"
    done
done
