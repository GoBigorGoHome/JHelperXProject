from tqdm import tqdm

sum = 0.0
for i in tqdm(range(10000000)):
    sum += i / 101.1

print(sum)
