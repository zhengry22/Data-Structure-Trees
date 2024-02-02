import random

n = random.randint(1, 2**31 - 1)
m = random.randint(1, 200000)

print(f"{n} {m}")

for _ in range(m):
    op_type = random.choice(["H", "Q"])
    i = random.randint(1, n)
    j = random.randint(i, n)

    print(f"{op_type} {i} {j}")
