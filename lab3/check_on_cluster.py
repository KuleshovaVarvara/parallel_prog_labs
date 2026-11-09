import sys


def load_matrix(filename):
    with open(filename, 'r') as f:
        N = int(f.readline())
        matrix = []
        for _ in range(N):
            row = list(map(float, f.readline().split()))
            matrix.append(row)
    return N, matrix


print("========== ВЕРИФИКАЦИЯ НА КЛАСТЕРЕ (чистый Python) ==========")

try:
    N, A = load_matrix("input/matrix_a.txt")
    _, B = load_matrix("input/matrix_b.txt")
    _, C = load_matrix("output/result.txt")

    max_error = 0.0

    # Перемножаем и сразу ищем максимальную ошибку
    for i in range(N):
        for j in range(N):
            expected = 0.0
            for k in range(N):
                expected += A[i][k] * B[k][j]

            error = abs(expected - C[i][j])
            if error > max_error:
                max_error = error

    print(f"Размер матриц: {N}x{N}")
    print(f"Максимальная ошибка: {max_error:.15f}")

    if max_error < 1e-9:
        print("✅ ВЕРИФИКАЦИЯ ПРОЙДЕНА!")
    else:
        print("❌ ВЕРИФИКАЦИЯ НЕ ПРОЙДЕНА!")

except Exception as e:
    print(f"Ошибка: {e}")