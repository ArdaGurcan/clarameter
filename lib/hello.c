#include "canvas.h"
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <emscripten/fetch.h>
void downloadSucceeded(emscripten_fetch_t *fetch)
{
    printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);

    char buff[255];
    double percentage = 0.33;
    char text1[66] = "";
    char text2[66] = "";
    strcpy(buff, fetch->data);
    char *ptr = strtok(buff, "\n");
    percentage = atof(ptr);
    ptr = strtok(NULL, "\n");
    strcpy(text1, ptr);
    ptr = strtok(NULL, "\n");
    strcpy(text2, ptr);

    HTMLCanvasElement *canvas = createCanvas("myCanvas");
    char output1[66] = "";
    char output2[66] = "";
    char ch = ' ';
    int width = 700;
    int height = 100;
    int radius = height / 2 - 10;
    int x = 50;
    int y = 154;
    canvas->setHeight(canvas, 600);
    CanvasRenderingContext2D *ctx = canvas->getContext(canvas, "2d");
    canvas->setWidth(canvas, 800);
    ctx->setFillStyle(ctx, "#4598E6");

    ctx->beginPath(ctx);
    ctx->moveTo(ctx, x + radius, y);
    ctx->lineTo(ctx, x + width - radius, y);
    ctx->quadraticCurveTo(ctx, x + width, y, x + width, y + radius);
    ctx->lineTo(ctx, x + width, y + height - radius);
    ctx->quadraticCurveTo(ctx, x + width, y + height, x + width - radius, y + height);
    ctx->lineTo(ctx, x + radius, y + height);
    ctx->quadraticCurveTo(ctx, x, y + height, x, y + height - radius);
    ctx->lineTo(ctx, x, y + radius);
    ctx->quadraticCurveTo(ctx, x, y, x + radius, y);
    ctx->closePath(ctx);
    ctx->fill(ctx);

    ctx->setFillStyle(ctx, "#E64565");

    width = round(percentage * width);
    ctx->beginPath(ctx);
    ctx->moveTo(ctx, x + radius, y);
    ctx->lineTo(ctx, x + width, y);

    ctx->lineTo(ctx, x + width, y + height);

    ctx->lineTo(ctx, x + radius, y + height);
    ctx->quadraticCurveTo(ctx, x, y + height, x, y + height - radius);
    ctx->lineTo(ctx, x, y + radius);
    ctx->quadraticCurveTo(ctx, x, y, x + radius, y);
    ctx->closePath(ctx);
    ctx->fill(ctx);

    ctx->setFont(ctx, "115px Courier New");
    ctx->fillText(ctx, "clarameter", 55, 120, -1);

    ctx->setFillStyle(ctx, "#fff");
    ctx->setFont(ctx, "20px Courier New");
    for (size_t i = 0; i < (66 / 2 - strlen(text1) / 2); i++)
    {
        strncat(output1, &ch, 1);
    }

    for (size_t i = 0; i < (66 / 2 - strlen(text2) / 2); i++)
    {
        strncat(output2, &ch, 1);
    }

    strcat(output1, text1);
    strcat(output2, text2);
    ctx->fillText(ctx, output1, 0, 300, -1);
    ctx->fillText(ctx, output2, 0, 325, -1);
    freeCanvas(canvas);
    emscripten_fetch_close(fetch);
}

void downloadFailed(emscripten_fetch_t *fetch)
{
    printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
    emscripten_fetch_close(fetch);
}

int main(void)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;
    emscripten_fetch(&attr, "https://theclara.tech/server/meter.txt");
    return 0;
}
