package tetris;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.Random;

public class TetrisGame extends JFrame {
    private static final int BOARD_WIDTH = 10;
    private static final int BOARD_HEIGHT = 20;
    private static final int CELL_SIZE = 30;

    private static Timer timer;
    private Board board;

    public TetrisGame() {
        setTitle("Tetris");
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(BOARD_WIDTH * CELL_SIZE + 16, BOARD_HEIGHT * CELL_SIZE + 39);
        setResizable(false);

        board = new Board();
        add(board);
        addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                board.processKey(e.getKeyCode());
            }
        });

        timer = new Timer(500, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                board.update();
                board.repaint();
            }
        });
        timer.start();

        setLocationRelativeTo(null);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            TetrisGame game = new TetrisGame();
            game.setVisible(true);
        });
    }

    private static class Board extends JPanel {
        private final int[][] board;
        private Tetromino currentTetromino;
        private int currentX, currentY;

        public Board() {
            board = new int[BOARD_HEIGHT][BOARD_WIDTH];
            spawnTetromino();
        }

        private void spawnTetromino() {
            currentTetromino = Tetromino.getRandomTetromino();
            currentX = BOARD_WIDTH / 2 - 1;
            currentY = 0;

            if (!canMove(currentTetromino.shape, currentX, currentY)) {
                // Game Over
                timer.stop();
                JOptionPane.showMessageDialog(this, "Game Over!", "Tetris", JOptionPane.INFORMATION_MESSAGE);
                System.exit(0);
            }
        }

        public void update() {
            if (canMove(currentTetromino.shape, currentX, currentY + 1)) {
                currentY++;
            } else {
                mergeTetromino();
                clearLines();
                spawnTetromino();
            }
        }

        private void mergeTetromino() {
            for (int i = 0; i < currentTetromino.shape.length; i++) {
                for (int j = 0; j < currentTetromino.shape[0].length; j++) {
                    if (currentTetromino.shape[i][j] != 0) {
                        board[currentY + i][currentX + j] = currentTetromino.shape[i][j];
                    }
                }
            }
        }

        private void clearLines() {
            for (int i = 0; i < BOARD_HEIGHT; i++) {
                boolean fullLine = true;
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    if (board[i][j] == 0) {
                        fullLine = false;
                        break;
                    }
                }
                if (fullLine) {
                    for (int k = i; k > 0; k--) {
                        System.arraycopy(board[k - 1], 0, board[k], 0, BOARD_WIDTH);
                    }
                    board[0] = new int[BOARD_WIDTH];
                }
            }
        }

        private boolean canMove(int[][] shape, int newX, int newY) {
            for (int i = 0; i < shape.length; i++) {
                for (int j = 0; j < shape[0].length; j++) {
                    if (shape[i][j] != 0) {
                        int x = newX + j;
                        int y = newY + i;

                        if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT || board[y][x] != 0) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        public void processKey(int keyCode) {
            if (keyCode == KeyEvent.VK_LEFT) {
                if (canMove(currentTetromino.shape, currentX - 1, currentY)) {
                    currentX--;
                }
            } else if (keyCode == KeyEvent.VK_RIGHT) {
                if (canMove(currentTetromino.shape, currentX + 1, currentY)) {
                    currentX++;
                }
            } else if (keyCode == KeyEvent.VK_DOWN) {
                if (canMove(currentTetromino.shape, currentX, currentY + 1)) {
                    currentY++;
                }
            } else if (keyCode == KeyEvent.VK_UP) {
                int[][] rotated = currentTetromino.rotate();
                if (canMove(rotated, currentX, currentY)) {
                    currentTetromino.shape = rotated;
                }
            }
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);

            // Draw board
            for (int i = 0; i < BOARD_HEIGHT; i++) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    if (board[i][j] != 0) {
                        g.setColor(Color.BLUE);
                        g.fillRect(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                        g.setColor(Color.BLACK);
                        g.drawRect(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                    }
                }
            }

            // Draw current tetromino
            for (int i = 0; i < currentTetromino.shape.length; i++) {
                for (int j = 0; j < currentTetromino.shape[0].length; j++) {
                    if (currentTetromino.shape[i][j] != 0) {
                        g.setColor(Color.RED);
                        g.fillRect((currentX + j) * CELL_SIZE, (currentY + i) * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                        g.setColor(Color.BLACK);
                        g.drawRect((currentX + j) * CELL_SIZE, (currentY + i) * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                    }
                }
            }
        }
    }

    private static class Tetromino {
        private int[][] shape;

        private Tetromino(int[][] shape) {
            this.shape = shape;
        }

        public int[][] rotate() {
            int rows = shape.length;
            int cols = shape[0].length;
            int[][] rotated = new int[cols][rows];

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    rotated[j][rows - 1 - i] = shape[i][j];
                }
            }
            return rotated;
        }

        public static Tetromino getRandomTetromino() {
            int[][][] shapes = {
                    {{1, 1, 1, 1}}, // Line
                    {{1, 1}, {1, 1}}, // Square
                    {{0, 1, 0}, {1, 1, 1}}, // T
                    {{0, 1, 1}, {1, 1, 0}}, // Z
                    {{1, 1, 0}, {0, 1, 1}} // S
            };
            Random rand = new Random();
            return new Tetromino(shapes[rand.nextInt(shapes.length)]);
        }
    }
}
