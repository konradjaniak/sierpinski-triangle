/*
    Kompilacja: g++ trojkaty_c11threads.cpp -o trojkaty_c11threads -O3 -lpthread
    Uruchomienie: ./trojkaty_c11threads
    Pokazanie obrazka: eog grafika.ppm
    Razem: g++ trojkaty_c11threads.cpp -o trojkaty_c11threads -O3 -pthread && ./trojkaty_c11threads && eog grafika.ppm
    
    Alternatywnie:
    Kompilacja: make c
    Uruchomienie i pokazanie obrazka: make r
*/

#include <iostream>
#include <math.h>
#include <thread>

const int SIZE = 4096;
const int MAX_LEVEL = 8;
char image[SIZE][SIZE][3];

void saveImageToFile();
void drawSierpinskiTriangle(int startX, int startY, int level);
void drawSingleTriangle();

int main()
{
    drawSierpinskiTriangle(0, 0, 0);
    saveImageToFile();
    return 0;
}

void saveImageToFile()
{
    FILE * fp = fopen("grafika.ppm", "wb");
    fprintf(fp, "P6\n%i %i\n%i ", SIZE, SIZE, 255);
    fwrite(image, 1, SIZE*SIZE*3, fp);
    fclose(fp);
}

void drawSingleTriangle(int size, int startX, int startY, int level)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (j == i || j == 0 || (i == size - 1))
            {
                image[i + startY][j + startX][0] = 255;
                image[i + startY][j + startX][1] = 255;
                image[i + startY][j + startX][2] = 255;
            }
        }
    }
}

void drawSierpinskiTriangle(int startX, int startY, int level)
{
    int sizeForThread = SIZE/(int)pow(2.0, (double)level);

    if (level > 0)
        drawSingleTriangle(sizeForThread, startX, startY, level);

    if (level < MAX_LEVEL)
    {
        std::thread threads[3];

        threads[0] = std::thread(drawSierpinskiTriangle, startX, startY, level + 1);
        threads[1] = std::thread(drawSierpinskiTriangle, startX, startY + sizeForThread/2, level + 1);
        threads[2] = std::thread(drawSierpinskiTriangle, startX + sizeForThread/2, startY + sizeForThread/2, level + 1);

        for (int i = 0; i < 3; i++)
        {
            (threads[i]).join();
        }
    }
}