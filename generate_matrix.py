import numpy as np
import sys
import os

size = int(sys.argv[1]) if len(sys.argv) > 1 else 200

input_dir = "lab1/src/input"
os.makedirs(input_dir, exist_ok=True)

A = np.random.rand(size, size)
B = np.random.rand(size, size)

np.savetxt(f"{input_dir}/matrix_a.txt", A, fmt="%.14f", header=str(size), comments='')
np.savetxt(f"{input_dir}/matrix_b.txt", B, fmt="%.14f", header=str(size), comments='')

print(f"Матрицы {size}x{size} созданы в папке {input_dir}/")