#include <stdio.h>
#include "platforms.h"
#include <time.h>

int main()
{
    unsigned int seed = time(NULL); // Пример значения для seed
    SetRandomSeed(seed);
    
    int height = 45, width = 150, numPaths = 3;
    InitializeMaze(height, width, numPaths);
    
    // Генерация платформ для каждого пути
    for (int path = 0; path < numPaths; path++)
        for (int i = 0; i < 20; i++) // Количество платформ для генерации
            GeneratePlatform(path);
    
    // Отрисовка лабиринта
    DrawMaze();

    // Получение массива
    int **maze = GetMaze();
    // Пример использования массива
    printf("Значение в позиции (10, 10): %d\n", maze[10][10]);

    // Освобождение памяти
    FreeMaze();
    return 0;
}
