#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "platforms.h"

// Глобальные переменные
int HEIGHT = 25;
int WIDTH = 100;
int **mas = NULL;
int **occupied = NULL; // Инициализация массива занятых областей
int numPaths = 2;
int *last_x, *last_y, *new_x, *new_y;

void SetRandomSeed(unsigned int seed)
{
    srand(seed);
}

void InitializeMaze(int height, int width, int numPathsInit)
{
    HEIGHT = height;
    WIDTH = width;
    numPaths = numPathsInit;

    mas = (int **)malloc(HEIGHT * sizeof(int *));
    occupied = (int **)malloc(HEIGHT * sizeof(int *)); // Инициализация массива занятых областей
    for (int i = 0; i < HEIGHT; i++)
    {
        mas[i] = (int *)malloc(WIDTH * sizeof(int));
        occupied[i] = (int *)calloc(WIDTH, sizeof(int)); // Инициализация массива нулями
        for (int j = 0; j < WIDTH; j++)
            mas[i][j] = 0;
    }

    last_x = (int *)malloc(numPaths * sizeof(int));
    last_y = (int *)malloc(numPaths * sizeof(int));
    new_x = (int *)malloc(numPaths * sizeof(int));
    new_y = (int *)malloc(numPaths * sizeof(int));

    for (int path = 0; path < numPaths; path++)
    {
        last_x[path] = 1;
        last_y[path] = (path + 1) * (HEIGHT / (numPaths + 1)); // Смещение для каждого нового пути
        mas[last_y[path]][last_x[path]] = 2;
        occupied[last_y[path]][last_x[path]] = 1; // Отметка как занятое
    }
    UpdateHitboxes();
}

int IsValidPosition(int x, int y)
{
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && mas[y][x] == 0 && occupied[y][x] == 0;
}

void CreatePlatformN_Blocks(int n, int path)
{
    for (int i = 0; i < n && new_x[path] < WIDTH; i++)
    {
        if (IsValidPosition(new_x[path], new_y[path]))
        {
            mas[new_y[path]][new_x[path]] = 2;
            occupied[new_y[path]][new_x[path]] = 1; // Отметка как занятое
        }
        new_x[path]++;
    }
    last_x[path] = new_x[path];
    last_y[path] = new_y[path];
}

void CreateLShapePlatform(int path)
{
    if (new_x[path] + 2 < WIDTH && new_y[path] - 2 >= 0)
    {
        for (int i = 0; i < 3; i++)
            if (IsValidPosition(new_x[path] + i, new_y[path]))
            {
                mas[new_y[path]][new_x[path] + i] = 2;
                occupied[new_y[path]][new_x[path] + i] = 1;
            }
        for (int i = 1; i <= 2; i++)
            if (IsValidPosition(new_x[path] + 2, new_y[path] - i))
            {
                mas[new_y[path] - i][new_x[path] + 2] = 2;
                occupied[new_y[path] - i][new_x[path] + 2] = 1;
            }
        last_x[path] = new_x[path] + 3;
        last_y[path] = new_y[path] - 2;
    }
}

void Create2x2Platform(int path)
{
    if (new_x[path] + 1 < WIDTH && new_y[path] - 1 >= 0)
    {
        for (int dy = 0; dy <= 1; dy++)
            for (int dx = 0; dx <= 1; dx++)
                if (IsValidPosition(new_x[path] + dx, new_y[path] - dy))
                {
                    mas[new_y[path] - dy][new_x[path] + dx] = 2;
                    occupied[new_y[path] - dy][new_x[path] + dx] = 1;
                }
        last_x[path] = new_x[path] + 2;
        last_y[path] = new_y[path] - 1;
    }
}

int IsPositionAdjacentToOccupied(int x, int y)
{
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
            if (y + dy >= 0 && y + dy < HEIGHT &&
                x + dx >= 0 && x + dx < WIDTH &&
                occupied[y + dy][x + dx] == 1)
                return 1;
    return 0;
}

void UpdateHitboxes()
{
    for (int j = 0; j < HEIGHT; j++)
        for (int i = 0; i < WIDTH; i++)
            if (mas[j][i] == 2)
                for (int dj = -1; dj <= 1; dj++)
                    for (int di = -1; di <= 1; di++)
                    {
                        int new_j = j + dj;
                        int new_i = i + di;
                        if (new_j >= 0 && new_j < HEIGHT && new_i >= 0 && new_i < WIDTH)
                        {
                            if (mas[new_j][new_i] != 2)
                                mas[new_j][new_i] = 1;
                            if (mas[new_j][new_i] == 1)
                                occupied[new_j][new_i] = 1; // Обновление массива занятых областей
                        }
                    }
}

void GeneratePlatform(int path)
{
    int count = 0;
    int rand_platform = rand() % 3;

    for (int i = 0; i < HEIGHT; i++)
        if (mas[i][last_x[path]] == 1)
            count++;

    new_x[path] = last_x[path] + 2;
    int max_attempts = 10;

    for (int attempts = 0; attempts < max_attempts; attempts++)
    {
        int max_shift = count > 2 ? 2 : 1;
        int shift = count > 0 ? (rand() % (2 * max_shift + 1)) - max_shift : 0;

        new_y[path] = last_y[path] + shift;

        // Проверка, чтобы между хитбоксами разных путей было расстояние в одну пустую клетку
        int is_valid = 1;
        for (int p = 0; p < numPaths; p++)
        {
            if (p != path)
            {
                for (int dy = -1; dy <= 1; dy++)
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        int check_x = new_x[path] + dx;
                        int check_y = new_y[path] + dy;
                        int prev_x = last_x[p] + dx;
                        int prev_y = last_y[p] + dy;
                        if (check_x >= 0 && check_x < WIDTH && check_y >= 0 && check_y < HEIGHT &&
                            prev_x >= 0 && prev_x < WIDTH && prev_y >= 0 && prev_y < HEIGHT &&
                            occupied[prev_y][prev_x] == 1)
                        {
                            if (abs(check_y - prev_y) <= 1)
                            {
                                is_valid = 0;
                                break;
                            }
                        }
                    }
                if (!is_valid)
                    break;
            }
        }

        if (is_valid && IsValidPosition(new_x[path], new_y[path]) && !IsPositionAdjacentToOccupied(new_x[path], new_y[path]))
            break;
    }

    if (new_x[path] < WIDTH)
    {
        if (rand_platform == 0)
            CreateLShapePlatform(path);
        else if (rand_platform == 1)
            CreatePlatformN_Blocks(rand() % 5 + 1, path);
        else if (rand_platform == 2)
            Create2x2Platform(path);
        UpdateHitboxes();
    }
}

void DrawMaze()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            switch (mas[i][j])
            {
            case 0: printf(" "); break;
            case 1: printf("-"); break;
            case 2: printf("#"); break;
            }
        }
        printf("\n");
    }
}

int **GetMaze()
{
    return mas;
}

void FreeMaze()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        free(mas[i]);
        free(occupied[i]);
    }
    free(mas);
    free(occupied);
    free(last_x);
    free(last_y);
    free(new_x);
    free(new_y);
}
