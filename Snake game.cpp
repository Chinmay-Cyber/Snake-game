#include "raylib.h"
#include <vector>
#include <string>

// Constants
const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 20;

// Directions for the snake
enum Direction { UP, DOWN, LEFT, RIGHT };

// Class to represent a single snake segment
class SnakeSegment {
public:
    int x, y;

    SnakeSegment(int posX, int posY) : x(posX), y(posY) {}
};

// Class to handle the snake
class Snake {
public:
    std::vector<SnakeSegment> body;
    Direction dir;

    Snake() {
        body.push_back(SnakeSegment(10, 10)); // Initialize with one segment
        dir = RIGHT;
    }

    void ChangeDirection(Direction newDir) {
        // Prevent reversing direction
        if ((dir == UP && newDir != DOWN) ||
            (dir == DOWN && newDir != UP) ||
            (dir == LEFT && newDir != RIGHT) ||
            (dir == RIGHT && newDir != LEFT)) {
            dir = newDir;
        }
    }

    SnakeSegment GetNextHead() {
        SnakeSegment newHead = body[0];
        if (dir == UP) newHead.y--;
        if (dir == DOWN) newHead.y++;
        if (dir == LEFT) newHead.x--;
        if (dir == RIGHT) newHead.x++;
        return newHead;
    }

    void Move(bool grow = false) {
        body.insert(body.begin(), GetNextHead());
        if (!grow) {
            body.pop_back();
        }
    }

    bool CheckCollision(SnakeSegment segment) {
        for (auto& part : body) {
            if (part.x == segment.x && part.y == segment.y) {
                return true;
            }
        }
        return false;
    }

    void Draw() {
        for (auto& segment : body) {
            DrawRectangle(segment.x * cellSize, segment.y * cellSize, cellSize, cellSize, GREEN);
        }
    }
};

// Class to handle the food
class Food {
public:
    SnakeSegment position;

    Food() {
        Respawn();
    }

    void Respawn() {
        position = SnakeSegment(GetRandomValue(0, screenWidth / cellSize - 1),
                                GetRandomValue(0, screenHeight / cellSize - 1));
    }

    void Draw() {
        DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, RED);
    }
};

// Game reset function
void ResetGame(Snake& snake, Food& food, bool& gameOver, int& score) {
    snake = Snake();
    food.Respawn();
    gameOver = false;
    score = 0;
}

// Main function
int main() {
    // Initialize the window
    InitWindow(screenWidth, screenHeight, "Snake Game");
    SetTargetFPS(10);

    Snake snake;
    Food food;
    bool gameOver = false;
    int score = 0;

    while (!WindowShouldClose()) {
        // Handle input
        if (IsKeyPressed(KEY_UP)) snake.ChangeDirection(UP);
        if (IsKeyPressed(KEY_DOWN)) snake.ChangeDirection(DOWN);
        if (IsKeyPressed(KEY_LEFT)) snake.ChangeDirection(LEFT);
        if (IsKeyPressed(KEY_RIGHT)) snake.ChangeDirection(RIGHT);

        if (!gameOver) {
            // Get the next head position
            SnakeSegment newHead = snake.GetNextHead();

            // Check collisions
            if (newHead.x < 0 || newHead.x >= screenWidth / cellSize ||
                newHead.y < 0 || newHead.y >= screenHeight / cellSize ||
                snake.CheckCollision(newHead)) {
                gameOver = true;
            }

            // Check if food is eaten
            if (!gameOver) {
                bool grow = (newHead.x == food.position.x && newHead.y == food.position.y);
                if (grow) {
                    food.Respawn();
                    score++;
                }
                snake.Move(grow);
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
                    ResetGame(snake, food, gameOver, score);
                }
            }
        } else {
            snake.Draw();
            food.Draw();
            DrawText(("Score: " + std::to_string(score)).c_str(), 10, 10, 20, WHITE);
        }

        EndDrawing();
    }

    // Close the window
    CloseWindow();

    return 0;
}
