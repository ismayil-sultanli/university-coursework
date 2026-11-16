import numpy as np
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE
from sklearn.preprocessing import StandardScaler

def load_data(file_path):
    epochs = []
    train_losses = []
    test_losses = []
    weights = []

    with open(file_path, 'r') as f:
        for line in f:
            if line.strip().startswith('Epoch'):
                parts = line.strip().split(',')
                epoch_info = parts[0].split()
                epoch = int(epoch_info[1][:-1])
                train_loss = float(parts[0].split('=')[1].strip())
                test_loss = float(parts[1].split('=')[1].strip())
                epochs.append(epoch)
                train_losses.append(train_loss)
                test_losses.append(test_loss)
            elif line.strip():
                weight_values = list(map(float, line.strip().split()))
                weights.append(weight_values)

    return np.array(epochs), np.array(train_losses), np.array(test_losses), np.array(weights)

def plot_tsne(weights, optimizer_name, labels):
    scaler = StandardScaler()
    weights_scaled = scaler.fit_transform(weights)
    tsne = TSNE(n_components=2, random_state=42, perplexity=30, max_iter=1000)
    reduced_weights = tsne.fit_transform(weights_scaled)
    plt.figure(figsize=(10, 8))
    scatter = plt.scatter(reduced_weights[:, 0], reduced_weights[:, 1], c=labels, cmap='tab10', alpha=0.7)
    plt.legend(*scatter.legend_elements(), title="W Values")
    plt.title(f't-SNE visualization of {optimizer_name} weights (across 5 different w values)')
    plt.xlabel('t-SNE Component 1')
    plt.ylabel('t-SNE Component 2')
    plt.show()

def plot_loss_vs_metrics(all_losses, optimizer_name):
    epochs = []
    train_losses = []
    test_losses = []

    for losses in all_losses[optimizer_name]:
        epochs = np.concatenate([epochs, losses[0]])
        train_losses = np.concatenate([train_losses, losses[1]])
        test_losses = np.concatenate([test_losses, losses[2]])

    plt.figure(figsize=(10, 6))
    plt.plot(epochs, train_losses, label='Train Loss')
    plt.plot(epochs, test_losses, label='Test Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.title(f'Loss vs Epoch for {optimizer_name}')
    plt.legend()
    plt.grid(True)
    plt.show()

    plt.figure(figsize=(10, 6))
    plt.plot(epochs, train_losses, label='Train Loss')
    plt.plot(epochs, test_losses, label='Test Loss')
    plt.xlabel('Time (Epochs)')
    plt.ylabel('Loss')
    plt.title(f'Loss vs Time for {optimizer_name}')
    plt.legend()
    plt.grid(True)
    plt.show()

INPUT_SIZE = 10
LEARNING_RATE = 0.01
EPOCHS = 100
MOMENTUM = 0.9
BETA1 = 0.9
BETA2 = 0.999
EPSILON = 1e-8

optimizers = ['gd', 'sgd', 'adam']
all_weights = {'gd': [], 'sgd': [], 'adam': []}
all_losses = {'gd': [], 'sgd': [], 'adam': []}

for optimizer in optimizers:
    for i in range(1, 6):
        weight_file = f'{optimizer}_weights_{i}.txt'
        epochs, train_losses, test_losses, weights = load_data(weight_file)
        all_weights[optimizer].append(weights)
        all_losses[optimizer].append((epochs, train_losses, test_losses))

for optimizer in optimizers:
    print(f"Plotting t-SNE for {optimizer.capitalize()}...")
    weights = np.concatenate(all_weights[optimizer], axis=0)
    labels = np.concatenate([np.full(weights.shape[0] // 5, i) for i in range(5)])
    plot_tsne(weights, optimizer.capitalize(), labels)

for optimizer in optimizers:
    plot_loss_vs_metrics(all_losses, optimizer)