import tkinter as tk
from tkinter import ttk
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib
matplotlib.use("TkAgg")

class SignalAnalyzerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Signal Analyzer")

        self.entries = []
        for i in range(3):
            frame = ttk.Frame(root)
            frame.pack(side=tk.TOP, fill=tk.X)
            ttk.Label(frame, text=f"Signal {i + 1} (A, f, θ)").pack(side=tk.LEFT)
            entry = ttk.Entry(frame)
            entry.pack(side=tk.LEFT)
            self.entries.append(entry)

        self.plot_button = ttk.Button(root, text="Plot signals", command=self.plot_signals)
        self.plot_button.pack(side=tk.TOP)

        self.figure = plt.Figure(figsize=(10, 6), dpi=100)
        self.canvas = FigureCanvasTkAgg(self.figure, master=root)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

    def plot_signals(self):
        A = []
        f = []
        theta = []
        for entry in self.entries:
            values = entry.get().split(',')
            if len(values) == 3:
                A.append(float(values[0]))
                f.append(float(values[1]))
                theta.append(float(values[2]))

        t = np.linspace(0, 1, 500)

        sine_signals = [A[i] * np.sin(2 * np.pi * f[i] * t + np.radians(theta[i])) for i in range(3)]
        cosine_signals = [A[i] * np.cos(2 * np.pi * f[i] * t + np.radians(theta[i])) for i in range(3)]

        synthesized_signal = sum(sine_signals) + sum(cosine_signals)

        self.figure.clear()
        ax1 = self.figure.add_subplot(311)
        ax2 = self.figure.add_subplot(312)
        ax3 = self.figure.add_subplot(313)

        for i in range(3):
            ax1.plot(t, sine_signals[i], label=f'Sine {i + 1}')
            ax2.plot(t, cosine_signals[i], label=f'Cosine {i + 1}')

        ax3.plot(t, synthesized_signal, label='Synthesized signal')

        ax1.legend()
        ax2.legend()
        ax3.legend()

        ax1.set_title('Sine signals')
        ax2.set_title('Cosine signals')
        ax3.set_title('Synthesized signal')

        self.canvas.draw()


class FourierSeriesAnalyzerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Fourier Series Analyzer")

        self.entries = []
        labels = ["a_0", "a_1", "a_2", "a_3", "b_1", "b_2", "b_3", "w_0", "T_0"]
        for label in labels:
            frame = ttk.Frame(root)
            frame.pack(side=tk.TOP, fill=tk.X)
            ttk.Label(frame, text=label).pack(side=tk.LEFT)
            entry = ttk.Entry(frame)
            entry.pack(side=tk.LEFT)
            self.entries.append(entry)

        self.plot_button = ttk.Button(root, text="Plot Fourier series", command=self.plot_fourier_series)
        self.plot_button.pack(side=tk.TOP)

        self.figure = plt.Figure(figsize=(10, 6), dpi=100)
        self.canvas = FigureCanvasTkAgg(self.figure, master=root)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

    def plot_fourier_series(self):
        values = [float(entry.get()) for entry in self.entries]
        a0, a1, a2, a3, b1, b2, b3, w0, T0 = values

        t = np.linspace(0, T0, 500)

        f_t = a0 + a1 * np.cos(w0 * t) + a2 * np.cos(2 * w0 * t) + a3 * np.cos(3 * w0 * t) + \
              b1 * np.sin(w0 * t) + b2 * np.sin(2 * w0 * t) + b3 * np.sin(3 * w0 * t)

        self.figure.clear()
        ax = self.figure.add_subplot(111)

        ax.plot(t, f_t, label='Fourier series')

        ax.legend()
        ax.set_title('Fourier series signal')

        self.canvas.draw()


class FourierTriangleWaveApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Fourier Series Triangle Wave Analyzer")

        self.entries = []
        labels = ["A", "T"]
        for label in labels:
            frame = ttk.Frame(root)
            frame.pack(side=tk.TOP, fill=tk.X)
            ttk.Label(frame, text=label).pack(side=tk.LEFT)
            entry = ttk.Entry(frame)
            entry.pack(side=tk.LEFT)
            self.entries.append(entry)

        self.plot_button = ttk.Button(root, text="Plot", command=self.plot_fourier_triangle)
        self.plot_button.pack(side=tk.TOP)

        self.figure = plt.Figure(figsize=(10, 6), dpi=100)
        self.canvas = FigureCanvasTkAgg(self.figure, master=root)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

    def plot_fourier_triangle(self):
        A = float(self.entries[0].get())
        T = float(self.entries[1].get())

        t = np.linspace(-T, T, 1000)
        w0 = 2 * np.pi / T

        f_t = np.zeros_like(t)
        for k in range(1, 6, 2):
            f_t += (8 * A / (k ** 2 * np.pi ** 2)) * np.cos(k * w0 * t)

        self.figure.clear()
        ax = self.figure.add_subplot(111)

        ax.plot(t, f_t, label='Fourier series approximation')
        ax.plot(t, A * np.abs((t % T) - T / 2) - A / 2, label='Original triangle wave', linestyle='--')

        ax.legend()
        ax.set_title(f'Fourier series of a triangle wave (A={A}, T={T})')

        self.canvas.draw()


def button1_action():
    window = tk.Toplevel()
    app = SignalAnalyzerApp(window)

def button2_action():
    window = tk.Toplevel()
    app = FourierSeriesAnalyzerApp(window)

def button3_action():
    window = tk.Toplevel()
    app = FourierTriangleWaveApp(window)

def build_menu():
    root = tk.Tk()
    root.title("Menu")

    button_frame = ttk.Frame(root)
    button_frame.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=10)

    button1 = ttk.Button(button_frame, text="1. Signals", command=button1_action)
    button1.pack(fill=tk.X, pady=5)

    button2 = ttk.Button(button_frame, text="2. Fourier series", command=button2_action)
    button2.pack(fill=tk.X, pady=5)

    button3 = ttk.Button(button_frame, text="3. Triangle wave", command=button3_action)
    button3.pack(fill=tk.X, pady=5)

    root.mainloop()

if __name__ == "__main__":
    build_menu()
