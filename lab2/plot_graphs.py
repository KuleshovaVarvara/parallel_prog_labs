import matplotlib.pyplot as plt
import numpy as np

with open("lab2/src/output/experiments.txt", "r", encoding="utf-8") as f:
    lines = f.readlines()

# Парсим результаты
data = {}
for line in lines[2:]:
    parts = line.strip().split(" | ")
    if len(parts) == 5:
        size = int(parts[0])
        threads = int(parts[1])
        time = float(parts[2])
        speedup = float(parts[3])

        if size not in data:
            data[size] = {"threads": [], "times": [], "speedups": []}
        data[size]["threads"].append(threads)
        data[size]["times"].append(time)
        data[size]["speedups"].append(speedup)

fig, axes = plt.subplots(1, 2, figsize=(16, 6))

# ГРАФИК 1: Время
for size in sorted(data.keys()):
    axes[0].plot(data[size]["threads"], data[size]["times"], 'o-', label=f'{size}x{size}')

axes[0].set_xlabel('Число потоков')
axes[0].set_ylabel('Время (сек)')
axes[0].set_title('Время выполнения от числа потоков')
axes[0].set_yscale('log')
axes[0].legend()
axes[0].grid(True, alpha=0.3)

# ГРАФИК 2: Ускорение
for size in sorted(data.keys()):
    axes[1].plot(data[size]["threads"], data[size]["speedups"], 'o-', label=f'{size}x{size}')

# Идеальное ускорение
threads_arr = np.array([1, 2, 4, 8, 16, 20])
axes[1].plot(threads_arr, threads_arr, 'k--', alpha=0.5, label='Идеальное')

axes[1].set_xlabel('Число потоков')
axes[1].set_ylabel('Ускорение (раз)')
axes[1].set_title('Ускорение OpenMP')
axes[1].legend()
axes[1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('lab2/src/output/graphs.png', dpi=150)
plt.show()

print("Графики сохранены в lab2/src/output/graphs.png")