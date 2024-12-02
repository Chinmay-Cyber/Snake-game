#include "raylib.h"
#include <vector>
#include <string>

// Constants
const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 20;

// Directions for the snake
enum Direction { UP, DOWN, LEFT, RIGHT };

// Struct to represent the snake's body
struct SnakeSegment {
    int x, y;
};

void ResetGame(std::vector<SnakeSegment>& snake, SnakeSegment& food, Direction& dir, bool& gameOver, int& score) {
    snake = { {10, 10} };
    dir = RIGHT;
    food = { GetRandomValue(0, (screenWidth / cellSize) - 1), GetRandomValue(0, (screenHeight / cellSize) - 1) };
    gameOver = false;
    score = 0;
}

int main() {
    // Initialize the window
    InitWindow(screenWidth, screenHeight, "Snake Game");
    SetTargetFPS(10);

    // Snake initialization
    std::vector<SnakeSegment> snake = { {10, 10} };
    Direction dir = RIGHT;

    // Food initialization
    SnakeSegment food = { GetRandomValue(0, (screenWidth / cellSize) - 1),
                         GetRandomValue(0, (screenHeight / cellSize) - 1) };

    // Game state
    bool gameOver = false;
    int score = 0;

    while (!WindowShouldClose()) {
        // Handle input
        if (IsKeyPressed(KEY_UP) && dir != DOWN) dir = UP;
        if (IsKeyPressed(KEY_DOWN) && dir != UP) dir = DOWN;
        if (IsKeyPressed(KEY_LEFT) && dir != RIGHT) dir = LEFT;
        if (IsKeyPressed(KEY_RIGHT) && dir != LEFT) dir = RIGHT;

        // Update the snake
        if (!gameOver) {
            // Create a new head
            SnakeSegment newHead = snake[0];
            if (dir == UP) newHead.y--;
            if (dir == DOWN) newHead.y++;
            if (dir == LEFT) newHead.x--;
            if (dir == RIGHT) newHead.x++;

            // Check collision with walls
            if (newHead.x < 0 || newHead.x >= screenWidth / cellSize ||
                newHead.y < 0 || newHead.y >= screenHeight / cellSize) {
                gameOver = true;
            }

            // Check collision with itself
            for (auto segment : snake) {
                if (segment.x == newHead.x && segment.y == newHead.y) {
                    gameOver = true;
                    break;
                }
            }

            // Move the snake
            if (!gameOver) {
                snake.insert(snake.begin(), newHead);
                // Check if food is eaten
                if (newHead.x == food.x && newHead.y == food.y) {
                    food.x = GetRandomValue(0, (screenWidth / cellSize) - 1);
                    food.y = GetRandomValue(0, (screenHeight / cellSize) - 1);
                    score++; // Increment score
                }
                else {
                    snake.pop_back();
                }
            }
        }

        // Draw everything
        BeginDrawing();
        ClearBackground(BLACK);

        if (gameOver) {
            DrawText("GAME OVER!", screenWidth / 2 - 100, screenHeight / 2 - 40, 40, RED);
            DrawText(("Score: " + std::to_string(score)).c_str(), screenWidth / 2 - 50, screenHeight / 2, 30, WHITE);
            DrawRectangle(screenWidth / 2 - 60, screenHeight / 2 + 60, 120, 40, GRAY);
            DrawText("Play Again", screenWidth / 2 - 50, screenHeight / 2 + 70, 20, BLACK);

            // Check if "Play Again" button is clicked
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (mouse.x > screenWidth / 2 - 60 && mouse.x < screenWidth / 2 + 60 &&
                    mouse.y > screenHeight / 2 + 60 && mouse.y < screenHeight / 2 + 100) {
                    ResetGame(snake, food, dir, gameOver, score);
                }
            }
        }
        else {
            // Draw the snake
            for (auto segment : snake) {
                DrawRectangle(segment.x * cellSize, segment.y * cellSize, cellSize, cellSize, GREEN);
            }

            // Draw the food
            DrawRectangle(food.x * cellSize, food.y * cellSize, cellSize, cellSize, RED);

            // Draw the score
            DrawText(("Score: " + std::to_string(score)).c_str(), 10, 10, 20, WHITE);
        }

        EndDrawing();
    }

    // Close the window
    CloseWindow();

    return 0;
}
