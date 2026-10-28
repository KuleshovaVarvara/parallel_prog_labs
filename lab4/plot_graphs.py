import matplotlib.pyplot as plt

data = {}

with open("src/output/experiments.txt", "r", encoding="utf-8") as f:
    lines = f.readlines()

for line in lines[2:]:
    parts = line.strip().split(" | ")
    if len(parts) == 5:
        size = int(parts[0])
        block = parts[1]
        speedup = float(parts[4])
        if block not in data:
            data[block] = {"sizes": [], "speedups": []}
        data[block]["sizes"].append(size)
        data[block]["speedups"].append(speedup)

plt.figure(figsize=(10, 6))
for block, vals in data.items():
    plt.plot(vals["sizes"], vals["speedups"], 'o-', label=f'Блок {block}')

plt.xlabel('Размер матрицы N', fontsize=12)
plt.ylabel('Ускорение (раз)', fontsize=12)
plt.title('Ускорение CUDA в зависимости от размера матрицы и размера блока', fontsize=14)
plt.grid(True, alpha=0.3)
plt.legend()

plt.savefig('src/output/graphs.png', dpi=150)
plt.show()
print("График сохранен в lab4/src/output/graphs.png")