#include <iostream>
#include <math.h>
#include <pthread.h>

const int SIZE = 4096;
char imageBuffer[SIZE][SIZE][3];
const int LEVELS = 8;

struct DrawData
{
    int startX, startY, level;
};

void* draw(void* _drawData)
{
    DrawData* drawData = (DrawData*)_drawData;
    int sizeForThread = SIZE/(int)pow(2.0, (double)drawData->level);

    if (drawData->level > 0)
    {
        for (int i = 0; i < sizeForThread; i++)
        {
            for (int j = 0; j < sizeForThread; j++)
            {
                if (j == i || j == 0 || (i == sizeForThread - 1))
                {
                    imageBuffer[i + drawData->startY][j + drawData->startX][0] = 255;
                    imageBuffer[i + drawData->startY][j + drawData->startX][1] = 255;
                    imageBuffer[i + drawData->startY][j + drawData->startX][2] = 255;
                }
            }
        }
    }

    if (drawData->level < LEVELS)
    {
        pthread_t threads[3];

        DrawData* drawDataArray = new DrawData[3];

        drawDataArray->startX = drawData->startX;
        drawDataArray->startY = drawData->startY;
        drawDataArray->level = drawData->level + 1;
        
        (drawDataArray + 1)->startX = drawData->startX;
        (drawDataArray + 1)->startY = drawData->startY + sizeForThread/2;
        (drawDataArray + 1)->level = drawData->level + 1;

        (drawDataArray + 2)->startX = drawData->startX + sizeForThread/2;
        (drawDataArray + 2)->startY = drawData->startY + sizeForThread/2;
        (drawDataArray + 2)->level = drawData->level + 1;

        pthread_create(&threads[0], NULL, draw, (void*)drawDataArray);
        pthread_create(&threads[1], NULL, draw, (void*)(drawDataArray + 1));
        pthread_create(&threads[2], NULL, draw, (void*)(drawDataArray + 2));

        for (int i = 0; i < 3; i++)
        {
            pthread_join(threads[i], NULL);
        }
    }
    return NULL;
}

int main()
{
    DrawData drawData = {0, 0, 0};
    draw(&drawData);

    FILE * fp;
    fp = fopen("grafika.ppm", "wb");
    fprintf(fp, "P6\n%i %i\n%i ", SIZE, SIZE, 255);
    fwrite(imageBuffer, 1, SIZE*SIZE*3, fp);
    fclose(fp);

    return 0;
}