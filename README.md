### Platformer-hitbox-generation
Этот алгоритм я разрабатывал на БММ 2024 года в НГУ.
Его идея заключается в том, что с помощью хитбоксов вокруг объектов мы обеспечиваем проходимость уровня ~~правда всё ещё у меня это не получилось~~.
У моего проекта есть возможность использования как и просто в виде библиотеки на C/C++ так и в динамической подключаемой библиотеке DLL. 
К примеру подключать в Unity и использовать это там.

__**Компиляции в виде библиотеке**__
```
clang -c platforms.c -o platforms.o
clang -c main.c -o main.o
clang platforms.o main.o -o maze_generator
```

__**Компиляция в DLL**__
```
 clang -c -o platforms.o platforms.c
 clang -c -o platforms.o platforms.c
```

## Функции и переменные
| Название | Значение |
| --- | --- |
| `height` | Задаёт выстоту игрового пространства |
| `width` | Задаёт ширину игрового пространства |
| `numPaths` | Задаёт количество путей |
| `SetRandomSeed(seed);` | Задаёт ключ генерации |
| `GeneratePlatform()` | Генерирует платформу |
| `DrawMaze();` | Графически выводит платформы |
| `GetMaze();` | Выдаёт массив |
| `FreeMaze();` | Очищает память |
