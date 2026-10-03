import subprocess
import os
import re
import sys

EXE_PATH = r"C:\Users\Варвара\source\repos\parallel_prog_lab2\x64\Debug\parallel_prog_lab2.exe"

sizes = [200, 400, 800, 1200, 1600]
threads_list = [1, 2, 4, 8]

print()
print("=" * 75)
print("ЭКСПЕРИМЕНТЫ: OpenMP умножение матриц")
print("=" * 75)
print()
print(f"{'Размер':<8} | {'Потоки':<7} | {'Время (сек)':<12} | {'Ускорение':<10} | {'Эффективность':<12}")
print("-" * 75)

results = []

for size in sizes:
    subprocess.run([sys.executable, "lab2/generate_matrix.py", str(size)], capture_output=True)

    baseline = None

    for t in threads_list:
        env = os.environ.copy()
        env["OMP_NUM_THREADS"] = str(t)

        # Запускаем 3 раза и берём среднее для точности
        times = []
        for _ in range(3):
            result = subprocess.run(
                [EXE_PATH],
                cwd=r"lab2\src",
                capture_output=True,
                text=True,
                env=env
            )
            match = re.search(r"time: ([\d.]+) sec", result.stdout)
            if match:
                times.append(float(match.group(1)))

        elapsed = sum(times) / len(times) if times else 0

        if t == 1:
            baseline = elapsed

        speedup = baseline / elapsed if elapsed > 0 else 0
        efficiency = (speedup / t * 100) if t > 0 else 0

        print(f"{size:<8} | {t:<7} | {elapsed:<12.4f} | {speedup:<10.2f} | {efficiency:<12.1f}%")
        results.append([size, t, elapsed, speedup, efficiency])

    print("-" * 75)

os.makedirs("lab2/src/output", exist_ok=True)
with open("lab2/src/output/experiments.txt", "w", encoding="utf-8") as f:
    f.write("Размер | Потоки | Время (сек) | Ускорение | Эффективность\n")
    f.write("-" * 60 + "\n")
    for r in results:
        f.write(f"{r[0]} | {r[1]} | {r[2]:.4f} | {r[3]:.2f} | {r[4]:.1f}%\n")

print()
print("Результаты сохранены в lab2/src/output/experiments.txt")