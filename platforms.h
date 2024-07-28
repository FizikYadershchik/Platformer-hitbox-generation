#ifndef PLATFORMS_H
#define PLATFORMS_H

#ifdef _WIN64
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

// Объявление функций
DLL_EXPORT void InitializeMaze(int height, int width, int numPaths);
DLL_EXPORT void SetPlatformSpacing(int x_spacing, int y_spacing); // Объявление функции с параметрами отступов
DLL_EXPORT void CreatePlatformN_Blocks(int n, int path);
DLL_EXPORT void CreateLShapePlatform(int path);
DLL_EXPORT void Create2x2Platform(int path);
DLL_EXPORT void UpdateHitboxes();
DLL_EXPORT void GeneratePlatform(int path);
DLL_EXPORT void DrawMaze();
DLL_EXPORT void FreeMaze();
DLL_EXPORT int** GetMaze(); // Функция для получения массива
DLL_EXPORT void SetRandomSeed(unsigned int seed); // Функция для установки seed

// Глобальные переменные для размера игрового поля
extern int HEIGHT;
extern int WIDTH;
extern int** mas; // Массив для лабиринта
extern int** occupied; // Массив для отслеживания занятых областей

#endif // PLATFORMS_H
