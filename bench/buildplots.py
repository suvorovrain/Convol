import os
import matplotlib.pyplot as plt

RESULTS_DIR = "./results"
PLOTS_DIR = "./plots"
os.makedirs(PLOTS_DIR, exist_ok=True)

algorithms = [
    "linear",
    "parallel_row",
    "parallel_col",
    "parallel_pixel",
    "q_linear",
    "q_parallel_row",
    "q_parallel_col",
    "q_parallel_pixel",
]

data = {}
for algo in algorithms:
    file_path = os.path.join(RESULTS_DIR, algo + ".txt")
    with open(file_path, "r") as f:
        values = []
        for line in f:
            line = line.strip()
            if line.startswith("elapsed_time:"):
                values.append(float(line.split(":")[1]))
        data[algo] = values

dataset1 = [data[algo][0] for algo in algorithms]
dataset2 = [data[algo][1] for algo in algorithms]

x = range(len(algorithms))
bar_width = 0.35

fig, ax = plt.subplots(figsize=(10, 6))
ax.bar([i - bar_width/2 for i in x], dataset1, width=bar_width, label="Dataset small")
ax.bar([i + bar_width/2 for i in x], dataset2, width=bar_width, label="Dataset big")
print(algorithms)
print(dataset1)
print(dataset2)
ax.set_xticks(x)
ax.set_xticklabels(algorithms, rotation=30, ha="right")
ax.set_ylabel("Time, seconds")
ax.set_title("Convolution Benchmark")
ax.legend()
ax.grid(axis="y", linestyle="--", alpha=0.7)

plot_path = os.path.join(PLOTS_DIR, "convolution_benchmark.png")
plt.tight_layout()
plt.savefig(plot_path, dpi=300)

print(f"Plot saved to {plot_path}")
