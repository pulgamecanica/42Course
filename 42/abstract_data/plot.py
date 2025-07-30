import matplotlib
matplotlib.use("TkAgg")  # Ensure GUI backend for interaction

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import sys

# Setup
sns.set(style="whitegrid")

# Check for CSV existence
csv_path = "benchmark_results.csv"
if not os.path.exists(csv_path):
    print(f"❌ Error: '{csv_path}' not found. Please run the benchmark generator first.")
    sys.exit(1)

# Load data
try:
    df = pd.read_csv(csv_path)
except Exception as e:
    print(f"❌ Failed to load '{csv_path}': {e}")
    sys.exit(1)

if df.empty:
    print("❌ Error: CSV is empty.")
    sys.exit(1)

# Format size labels
df["SizeLabel"] = df["Size"].apply(lambda x: f"{int(x):,}")

# Output directory
output_dir = "plots"
os.makedirs(output_dir, exist_ok=True)

# Generate one image per function with subplots per type
plot_files = []
unique_functions = df["Function"].unique()
unique_types = df["Type"].unique()

for func in unique_functions:
    safe_func = func.replace(':', '_').replace('/', '_')
    func_dir = os.path.join(output_dir, safe_func)
    os.makedirs(func_dir, exist_ok=True)

    fig, axes = plt.subplots(1, len(unique_types), figsize=(6 * len(unique_types), 5), sharey=True)
    if len(unique_types) == 1:
        axes = [axes]

    for ax, typ in zip(axes, unique_types):
        subset = df[(df["Function"] == func) & (df["Type"] == typ)]
        sns.lineplot(
            data=subset,
            x="SizeLabel",
            y="Time",
            hue="Namespace",
            style="Namespace",
            markers=True,
            dashes=False,
            ax=ax
        )
        ax.set_title(typ)
        ax.set_xlabel("Size")
        if ax == axes[0]:
            ax.set_ylabel("Time (s)")
        else:
            ax.set_ylabel("")
        ax.legend(title="Namespace")

    fig.suptitle(f"vector::{func}()", fontsize=16)
    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    fname = os.path.join(func_dir, f"{safe_func}.png")
    plt.savefig(fname)
    plot_files.append(fname)
    plt.close()

print("✅ Function benchmark plots saved as one image per function in 'plots/<function>/<function>.png'")

# ============ Interactive Slideshow of Combined Function Images ============
import matplotlib.image as mpimg
import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(12, 7))
plt.axis("off")

index = [0]


def show_image(i):
    ax.clear()
    ax.axis("off")
    try:
        img = mpimg.imread(plot_files[i])
        ax.imshow(img)
        fig.suptitle(f"{plot_files[i]}  ({i+1}/{len(plot_files)})", fontsize=14)
        fig.canvas.draw_idle()
    except Exception as e:
        print(f"❌ Error loading image: {plot_files[i]}\n{e}")
        plt.close()


def on_key(event):
    if event.key == "right":
        index[0] = (index[0] + 1) % len(plot_files)
        show_image(index[0])
    elif event.key == "left":
        index[0] = (index[0] - 1) % len(plot_files)
        show_image(index[0])
    elif event.key == "escape":
        plt.close()


fig.canvas.mpl_connect("key_press_event", on_key)
show_image(index[0])
plt.show()