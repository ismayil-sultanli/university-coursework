# C Algorithms & Systems

Data structures, algorithms and numerical computing in C (plus two small Python tools).

| Folder | What it is |
|--------|-----------|
| `data-structures/avl-student-grades` | Student grades database on a self-balancing **AVL tree**: search by ID or name, in-order sort, filtering, statistics, add/remove, save to file, tree view |
| `data-structures/call-center-queue-simulation` | Call-center simulation with linked-list queue and stack; 4 input files with expected outputs in `tests/` |
| `data-structures/flight-runway-scheduler` | Flight records read from file, sorted in a linked list and split into one file per runway |
| `data-structures/bst-and-linked-lists` | Surname BST that moves names into a deque/queue by length; linked lists: insert after the last even element, move zeros to the end |
| `data-structures/linked-list-intersection` | Intersection of two linked lists using interchangeable comparison functions |
| `algorithms/matrix-path-finder` | Iterative 8-direction flood fill: finds paths from the first to the last column |
| `algorithms/runway-departure-scheduler` | Assigns departing planes to two runways with a minimum time gap and reports average delay |
| `numerical-methods` | Menu-driven solver with its own function parser: bisection, regula falsi, Newton-Raphson, matrix inverse, Gauss elimination, Gauss-Seidel, numerical derivative, Simpson, trapezoid, Gregory-Newton interpolation |
| `machine-learning/optimizers-from-scratch` | Gradient descent, SGD and Adam in plain C; Python script visualises the weight trajectories with t-SNE |
| `games/three-player-reversi` | Reversi for three players on an N×N board |
| `games/skipitty-board-game` | Board game against a player or the computer, with undo and binary save/load |
| `signal-processing` | Convolution from scratch with plots, an echo effect on recorded audio, and a Tkinter signal and Fourier-series analyser |

## Build and test

```bash
./run_tests.sh                     # builds every program and runs the checks
gcc -std=c11 -O2 file.c -o program -lm   # build a single program
```

Examples:

```bash
./call_center_sim data-structures/call-center-queue-simulation/tests/input01.txt
cd data-structures/avl-student-grades && ./avl_student_grades students.txt
```

The numerical solver accepts functions like `x^3-2*x-5`, `2x^2+sin(x)`.
