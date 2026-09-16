import numpy as np

def load_matrix(filename):
    """Читает матрицу из файла (первая строка — размер N)."""
    with open(filename, 'r') as f:
        N = int(f.readline())
        matrix = []
        for _ in range(N):
            row = list(map(float, f.readline().split()))
            matrix.append(row)
    return np.array(matrix)

print("========== ВЕРИФИКАЦИЯ ==========")

try:
    A = load_matrix("lab1/src/input/matrix_a.txt")
    B = load_matrix("lab1/src/input/matrix_b.txt")
    my_result = load_matrix("lab1/src/output/result.txt")

    correct_result = np.matmul(A, B)

    max_error = np.max(np.abs(correct_result - my_result))
    print(f"Размер матриц: {A.shape[0]}x{A.shape[1]}")
    print(f"Максимальная ошибка: {max_error:.15f}")

    if max_error < 1e-9:
        print("✅ ВЕРИФИКАЦИЯ ПРОЙДЕНА!")
    else:
        print("❌ ВЕРИФИКАЦИЯ НЕ ПРОЙДЕНА!")
        print(f"Ошибка слишком большая: {max_error:.15f}")

except FileNotFoundError as e:
    print(f"❌ Ошибка: файл не найден!")
    print(f"  {e}")

except Exception as e:
    print(f"❌ Ошибка: {e}")