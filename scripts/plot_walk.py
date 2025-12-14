import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Slider
import argparse
import pathlib

def main():
    parser = argparse.ArgumentParser(description="Animate a random walk from CSV data")
    parser.add_argument("--file", type=str, required=True, help="CSV filename inside the data folder")
    args = parser.parse_args()

    # Resolve path relative to script
    script_dir = pathlib.Path(__file__).resolve().parent
    data_file = script_dir.parent / "data" / args.file

    df = pd.read_csv(data_file)

    # --- Infer dimensionality ---
    def infer_dim(df):
        data = df[["x", "y", "z"]].to_numpy()
        active = (np.std(data, axis=0) > 1e-12)
        dim = int(np.sum(active))
        return max(dim, 1), active

    dim, active = infer_dim(df)

    # --- Plot setup ---
    if dim == 3:
        from mpl_toolkits.mplot3d import Axes3D
        fig = plt.figure()
        ax = fig.add_subplot(111, projection="3d")
        line, = ax.plot([], [], [], lw=2)

        ax.set_xlim(df["x"].min(), df["x"].max())
        ax.set_ylim(df["y"].min(), df["y"].max())
        ax.set_zlim(df["z"].min(), df["z"].max())
        ax.set_title("3D Random Walk")

        def init():
            line.set_data([], [])
            line.set_3d_properties([])
            return line,

        def update(frame):
            line.set_data(df["x"][:frame], df["y"][:frame])
            line.set_3d_properties(df["z"][:frame])
            return line,

    else:
        fig, ax = plt.subplots()
        line, = ax.plot([], [], lw=2)

        ax.set_xlim(df["x"].min(), df["x"].max())
        ax.set_ylim(df["y"].min(), df["y"].max())
        ax.set_title(f"{dim}D Random Walk")

        def init():
            line.set_data([], [])
            return line,

        def update(frame):
            line.set_data(df["x"][:frame], df["y"][:frame])
            return line,

    # --- Animation ---
    ani = FuncAnimation(fig, update, frames=len(df), init_func=init,
                        blit=True, interval=50, repeat=False)

    # --- Add slider for speed control ---
    axspeed = plt.axes([0.25, 0.02, 0.50, 0.03])
    speed_slider = Slider(axspeed, "Speed (ms/frame)", 1, 200, valinit=50, valstep=1)

    def update_speed(_):
        ani.event_source.interval = speed_slider.val
    speed_slider.on_changed(update_speed)

    plt.show()

if __name__ == "__main__":
    main()
