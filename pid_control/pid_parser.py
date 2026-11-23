import re
import matplotlib.pyplot as plt

with open("teraterm1.log") as fptr:
    lines = fptr.readlines()

pwrs = []
targets = []
poss = []

last_pos = None

for index, line in enumerate(lines):
    pattern = r'^\[(\w+ \w+ \d+ \d+:\d+:\d+\.\d+ \d+)\] Pwr: (\d+) Target: (\d+) Pos: (\d+)$'
    match = re.match(pattern, line)
    if match:
        pwr = match.group(2)
        target = match.group(3)
        pos = match.group(4)
        last_pos = pos
        pwrs.append(int(pwr))
        targets.append(int(target))
        poss.append(int(pos))
        

x = range(len(pwrs))
plt.figure(figsize=(10, 6))
#plt.plot(x, pwrs, label='Pwr', marker='o')
plt.plot(x, targets, label='Target', marker='x')
plt.plot(x, poss, label='Pos', marker='s')

# Add labels and title
plt.xlabel('Sample Index')
plt.ylabel('Values')
plt.title('Pwr, Target, and Pos Values Over Samples')
plt.legend()
plt.grid(True)

# Show the plot
plt.show()