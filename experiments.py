import subprocess
import os
import re
import sys

EXE_PATH = r"C:\Users\Варвара\source\repos\parallel_prog_lab1\x64\Debug\parallel_prog_lab1.exe"

sizes = [200, 400, 800, 1200, 1600, 2000]

print()
print("=" * 60)
print("ЭКСПЕРИМЕНТЫ: последовательное умножение матриц")
print("=" * 60)
print()
print(f"{'Размер':<8} | {'Время (сек)':<12} | {'Объём задачи':<15}")
print("-" * 60)

results = []

for size in sizes:
    subprocess.run([sys.executable, "generate_matrix.py", str(size)], capture_output=True)

    result = subprocess.run(
        [EXE_PATH],
        cwd=r"lab1\src",
        capture_output=True,
        text=True
    )

    match = re.search(r"Time: ([\d.eE+-]+)", result.stdout)
    elapsed = float(match.group(1)) if match else 0
    operations = size ** 3

    print(f"{size:<8} | {elapsed:<12.4f} | {operations:<15}")
    results.append([size, elapsed, operations])

print("-" * 60)

os.makedirs("lab1/src/output", exist_ok=True)
with open("lab1/src/output/experiments.txt", "w", encoding="utf-8") as f:
    f.write("Размер | Время (сек) | Объём задачи (N³)\n")
    f.write("-" * 50 + "\n")
    for r in results:
        f.write(f"{r[0]} | {r[1]:.4f} | {r[2]}\n")

print()
print("Результаты сохранены в lab1/src/output/experiments.txt")