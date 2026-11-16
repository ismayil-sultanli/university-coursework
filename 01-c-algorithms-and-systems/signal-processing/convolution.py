def task_1():
    def myConv(x, y):

        n = len(x)
        m = len(y)

        result_length = n + m - 1

        result = [0] * result_length

        for i in range(n):
            for j in range(m):
                result[i + j] += x[i] * y[j]

        return result

    x_input = input("Enter sequence x (values separated by spaces): ")
    y_input = input("Enter sequence y (values separated by spaces): ")

    x = list(map(int, x_input.split()))
    y = list(map(int, y_input.split()))

    result = myConv(x, y)

    print("Result (x * y) =", result)


def task_2():
    import matplotlib.pyplot as plt

    def myConv(x, y):
        n = len(x)
        m = len(y)
        result_length = n + m - 1
        result = [0] * result_length
        for i in range(n):
            for j in range(m):
                result[i + j] += x[i] * y[j]
        return result

    x_input = input("Enter sequence x (values separated by spaces): ")
    y_input = input("Enter sequence y (values separated by spaces): ")

    x = list(map(int, x_input.split()))
    y = list(map(int, y_input.split()))

    result = myConv(x, y)


    plt.figure(figsize=(10, 4))


    plt.subplot(1, 3, 1)
    plt.stem(x, basefmt=" ", linefmt='b-', markerfmt='bo', label='x[n]')
    plt.title('x[n]')
    plt.legend()


    plt.subplot(1, 3, 2)
    plt.stem(y, basefmt=" ", linefmt='g-', markerfmt='go', label='y[m]')
    plt.title('y[m]')
    plt.legend()


    plt.subplot(1, 3, 3)
    plt.stem(result, basefmt=" ", linefmt='r-', markerfmt='ro', label='Convolution result (x * y)')
    plt.title('Convolution result (x * y)')
    plt.legend()

    plt.tight_layout()
    plt.show()

    print("Result (x * y) =", result)


def task_3():
    import sounddevice as sd
    import numpy as np
    import soundfile as sf


    def process_audio(x, A, M, delay):
        y = np.copy(x)
        for k in range(1, M + 1):

            shifted_x = np.roll(x, delay * k)
            shifted_x[:delay * k] = 0

            y += (A ** -k) * k * shifted_x
        return y


    def record_audio(duration, fs=44100):
        print(f"Recording for {duration} seconds...")
        audio = sd.rec(int(duration * fs), samplerate=fs, channels=1, dtype='float64')
        sd.wait()
        print("Recording finished.")
        return audio.flatten()


    def play_audio(data, fs=44100):
        sd.play(data, fs)
        sd.wait()

    fs = 44100
    A = 2
    delay = 3000


    x1 = record_audio(5)
    x2 = record_audio(10)


    y1 = {}
    y2 = {}

    for M in [3, 4, 5]:
        y1[M] = process_audio(x1, A, M, delay)
        y2[M] = process_audio(x2, A, M, delay)


    for M in [3, 4, 5]:
        play_audio(y1[M], fs)
        play_audio(y2[M], fs)


    sf.write('x1.wav', x1, fs)
    sf.write('x2.wav', x2, fs)
    for M in [3, 4, 5]:
        sf.write(f'y1_M{M}.wav', y1[M], fs)
        sf.write(f'y2_M{M}.wav', y2[M], fs)

    print("x1.wav, x2.wav")
    for M in [3, 4, 5]:
        print(f'y1_M{M}.wav, y2_M{M}.wav')


def show_menu():
    print("1. Convolution from scratch")
    print("2. Convolution with plots")
    print("3. Echo effect on recorded audio")
    print("4. Exit")


while True:
    show_menu()
    choice = input("Please choose an option (1-4): ")

    if choice == "1":
        task_1()
    elif choice == "2":
        task_2()
    elif choice == "3":
        task_3()
    elif choice == "4":
        print("Exiting the program...")
        break
    else:
        print("Invalid option! Please enter a number between 1 and 4.")
