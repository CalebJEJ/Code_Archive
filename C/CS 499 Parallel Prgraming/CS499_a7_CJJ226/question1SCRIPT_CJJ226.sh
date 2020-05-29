#!/bin/bash
#SBATCH --job-name=exercise                        # the name of your job
#SBATCH --output=/scratch/cjj226/output.txt	# this is the file your output and errors go to
#SBATCH --time=6:00				# 20 min, shorter time, quicker start, max run time 
#SBATCH --chdir=/scratch/cjj226		# your work directory
#SBATCH --mem=1000                              # 2GB of memory
#SBATCH --qos=gpu_class
#SBATCH --gres=gpu:1

srun sleep 60
nvidia-smi
