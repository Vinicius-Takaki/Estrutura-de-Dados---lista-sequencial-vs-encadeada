"""
Reads results_sorting.csv and produces a comparison chart with one panel
per initial data order (random / sorted / reverse), plotting comparisons
on a log scale so the O(n^2) algorithms (selection/insertion/bubble) can
be seen alongside the O(n log n) ones (shell/quick/merge) on the same axes.
"""
import os
import pandas as pd
import matplotlib.pyplot as plt

HERE = os.path.dirname(os.path.abspath(__file__))
CSV_PATH = os.path.join(HERE, "results_sorting.csv")
OUTPUT_PATH = os.path.join(HERE, "..", "assets", "sorting_comparison_chart.png")

df = pd.read_csv(CSV_PATH)

orders = ["random", "sorted", "reverse"]
algorithms = [
    ("selection", "tab:red"),
    ("insertion", "tab:orange"),
    ("bubble", "tab:brown"),
    ("shell", "tab:green"),
    ("quick", "tab:blue"),
    ("merge", "tab:purple"),
]

fig, axes = plt.subplots(1, 3, figsize=(18, 6), sharey=True)

for ax, order in zip(axes, orders):
    for algo, color in algorithms:
        sub = df[(df.algorithm == algo) & (df.order == order)].sort_values("n")
        ax.plot(sub.n, sub.comparisons, marker="o", color=color, label=algo)
    ax.set_title(f"Initial order: {order}")
    ax.set_xlabel("n (list size)")
    ax.set_yscale("log")
    ax.grid(alpha=0.3, which="both")

axes[0].set_ylabel("Comparisons (log scale)")
axes[0].legend()
plt.suptitle("Sorting Algorithms — Comparisons by Initial Order", fontsize=16)
plt.tight_layout()
os.makedirs(os.path.dirname(OUTPUT_PATH), exist_ok=True)
plt.savefig(OUTPUT_PATH, dpi=110)
print(f"Chart saved to {OUTPUT_PATH}")
