#include <iostream>
#include <raylib.h>
#include <vector>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color dark_green = {102, 153, 51, 255};

int cell_size = 30;
int cell_count = 25;
double last_update_time = 0.0;

bool element_in_deque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element)) {
            return true;
        }
    }
    return false;
}

bool event_triggered(double interval)
{
    double current_time = GetTime();

    if(current_time - last_update_time >= interval) {
        last_update_time = current_time;
        return true;
    }
    return false;
}

class Snake {
public :
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    vector<Color> colors;

    Snake() {
        // Initialize colors for each segment (you can customize this)
        for (size_t i = 0; i < body.size(); i++) {
            colors.push_back(dark_green); // Default color for all segments
        }
    }
    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++) {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{x * cell_size, y * cell_size, (float)cell_size, (float)cell_size};
            DrawRectangleRounded(segment, 0.5, 6, colors[i]); // Use colors[i] for the segment color
        }
    }
    void Update()
    {
        body.pop_back();
        body.push_front(Vector2Add(body[0], direction));
        colors.front() = DARKGREEN;
    }
};

class Food {

public:
    Vector2 food_position = {5, 6};
    Texture2D texture;

    Food(deque<Vector2> snake_body)
    {
        Image image = LoadImage("apple.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        food_position = generate_random_pos(snake_body);
    }
    ~Food()
    {
        UnloadTexture(texture);
    }
    void Draw() {
        DrawTexture(texture, food_position.x * cell_size, food_position.y * cell_size, WHITE);
    }

    Vector2 generate_random_cell(void)
    {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);
        return Vector2{x, y};
    }
    Vector2 generate_random_pos(deque<Vector2> snake_body)
    {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);
        Vector2 position = {x, y};
        while (element_in_deque(position, snake_body))
        {
            position = generate_random_cell();
        }
        return Vector2{x, y};
    }

};

class Game {
    public :
    Food food = Food(snake.body);
    Snake snake = Snake();

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        snake.Update();
    }

    void check_food_collision()
    {
        if (Vector2Equals(snake.body[0], food.food_position))
        {
            food.food_position = food.generate_random_pos(snake.body);
        }
    }
};

int main(void)
{
    cout << "Hello World" << endl;
    InitWindow(cell_size * cell_count, cell_size * cell_count, "Snake Test");

    SetTargetFPS(60);

    Game game = Game();
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        if (event_triggered(0.1))
            game.snake.Update();
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
            game.snake.direction = {0, -1};
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
            game.snake.direction = {0, 1};
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
            game.snake.direction = {-1, 0};
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
            game.snake.direction = {1, 0};
        ClearBackground(green);
        game.Draw();
        game.check_food_collision();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}