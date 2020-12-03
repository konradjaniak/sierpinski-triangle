#include <iostream>
#include <math.h>
#include <pthread.h>

const int SIZE = 4096;
char imageBuffer[SIZE][SIZE][3];
const int LEVELS = 8;

void draw(int startX, int startY, int level)
{
    int sizeForThread = SIZE/(int)pow(2.0, (double)level);

    if (level > 0)
    {
        for (int i = 0; i < sizeForThread; i++)
        {
            for (int j = 0; j < sizeForThread; j++)
            {
                if (j == i || j == 0 || (i == sizeForThread - 1))
                {
                    imageBuffer[i + startY][j + startX][0] = 255;
                    imageBuffer[i + startY][j + startX][1] = 255;
                    imageBuffer[i + startY][j + startX][2] = 255;
                }
            }
        }
    }

    if (level < LEVELS)
    {
        draw(startX, startY, level + 1);
        draw(startX, startY + sizeForThread/2, level + 1);
        draw(startX + sizeForThread/2, startY + sizeForThread/2, level + 1);
    }
}

int main()
{
    draw(0, 0, 0);

    FILE * fp;
    fp = fopen("grafika.ppm", "wb");
    fprintf(fp, "P6\n%i %i\n%i ", SIZE, SIZE, 255);
    fwrite(imageBuffer, 1, SIZE*SIZE*3, fp);
    fclose(fp);

    return 0;
}