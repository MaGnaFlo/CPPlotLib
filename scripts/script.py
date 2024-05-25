import matplotlib.pyplot as plt
import numpy as np

def plot(x, y, dpi, figwidth, figheight, linecolor, linewidth, linestyle, pointcolor, pointsize, grid, title, xlabel, ylabel):
    fig, ax = plt.subplots(1,1,figsize=(int(figwidth), int(figheight)))
    ax.plot(x, y, color=linecolor, lw=int(linewidth), ls=linestyle)
    ax.scatter(x, y, c=pointcolor, s=int(pointsize))
    ax.set_title(title)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.grid(grid)
    fig.set_dpi(int(dpi))
    fig.canvas.draw()

    width, height = fig.get_figwidth() * int(dpi), fig.get_figheight() * int(dpi)
    img = np.fromstring(fig.canvas.tostring_rgb(), dtype='uint8').reshape(int(height), int(width), 3)
    return img