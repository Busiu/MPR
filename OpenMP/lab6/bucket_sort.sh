#!/bin/bash -l

#SBATCH --nodes 1
#SBATCH --ntasks 20
#SBATCH --exclusive
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgwbus2020a

gcc -Wall bucket_sort.c -o bucket_sort -fopenmp -std=c99

echo -e "The Results of Bucket Sort:\n" > bucket_sort.txt
for ((i = 1; i<= 20; i++)); do
    echo -e "Number of threads - $i:" >> bucket_sort.txt
    env ./bucket_sort 1000000 $i 1000 1000 >> bucket_sort.txt
    echo -e "" >> bucket_sort.txt
done
