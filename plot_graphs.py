import matplotlib.pyplot as plt
import numpy as np

with open("lab1/src/output/experiments.txt", "r", encoding="utf-8") as f:
    lines = f.readlines()

sizes = []
times = []
for line in lines[2:]:  # пропускаем заголовок
    parts = line.strip().split(" | ")
    if len(parts) == 3:
        sizes.append(int(parts[0]))
        times.append(float(parts[1]))

fig, ax = plt.subplots(figsize=(10, 6))

ax.plot(sizes, times, 'o-', color='steelblue', linewidth=2, markersize=8, label='Эксперимент')

sizes_arr = np.array(sizes)
times_arr = np.array(times)
theoretical = times_arr[0] * (sizes_arr / sizes_arr[0]) ** 3
ax.plot(sizes, theoretical, '--', color='red', alpha=0.6, label='Теоретическая O(N³)')

ax.set_xlabel('Размер матрицы N', fontsize=12)
ax.set_ylabel('Время (сек)', fontsize=12)
ax.set_title('Зависимость времени умножения матриц от размера N', fontsize=14)
ax.grid(True, alpha=0.3)
ax.legend()

plt.tight_layout()
plt.savefig('lab1/src/output/graph.png', dpi=150)
plt.show()

print("График сохранён в lab1/src/output/graph.png")