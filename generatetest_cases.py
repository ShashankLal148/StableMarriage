import random

def generate_test_case(n, filename):
    men = []
    women = []

    for i in range(n):
        # Each man has a preference list with random length (incomplete list)
        preference = random.sample(range(n), random.randint(n // 2, n))  # At least half the women
        random.shuffle(preference)  # Introduce ties by shuffling
        men.append(f"{i} " + " ".join(map(str, preference)))

    for i in range(n):
        # Each woman has a preference list with random length (incomplete list)
        preference = random.sample(range(n), random.randint(n // 2, n))
        random.shuffle(preference)  # Introduce ties by shuffling
        women.append(f"{i} " + " ".join(map(str, preference)))

    with open(filename, "w") as f:
        f.write(f"{n}\n")  # Number of men and women
        f.write("\n".join(men) + "\n")
        f.write("\n".join(women) + "\n")

# Generate two large test cases
generate_test_case(500, "/mnt/data/test_case_1.txt")
generate_test_case(500, "/mnt/data/test_case_2.txt")
