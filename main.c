#include "mylib.h"
#include <stdlib.h>
#include <stdio.h>
#include "my_images.h"

//structs
typedef struct
{
    int visible;
    int x;
    int topHeight;
    int botHeight;
} PIPE;

typedef struct
{
    int y;
    int x;
} BIRD;

//game state enum
enum GameState
{
    START,
    START_WAIT,
    PLAY,
    END,
    END_WAIT
};

//const
const int pipeMargin = 1;
const int delayTime = 1;
const int flyHeight = 5;
const int gravity = 1;
const int pipeSpeed = 1;
const int pipeDistance = 100;
const int numPipes = 5;

//prototypes
void reset();
void initiatePipes();
void showPipe(PIPE *pipe);
void genPipeHeight(PIPE *pipe);
void drawBird();
void clearItems(const u16 *image);
void drawPipe(PIPE *pipe);
void clearPipe(PIPE *pipe, const u16 *image);
void drawPipes();
void fall();
void movePipes();
void fly();
void flyLower();
void evaluate();
int checkCollision(BIRD *bird1, PIPE *pipe);
int status();
void drawBackground(const u16 *image);

//variables & pointers
int score = 0;
BIRD birb;
PIPE *pipes;
PIPE *currPipe;
PIPE *nextPipe;

//main funct
int main() {

    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    enum GameState state = START;

    //keystroke from last frame
    int startKeypress = 0;
    int upKeypress = 0;

    // main loop
    while (1) {
        waitForVBlank();
        if (state == START)
        {
            reset();
            state = START_WAIT;
            drawBackground(startScreen);
            drawBird();
            drawPipes();
            drawString(50, (SCREEN_WIDTH - 96) / 2, "Press UP to begin", WHITE);
        }
        else if (state == START_WAIT)
        {
          if (KEY_DOWN_NOW(BUTTON_UP) && !upKeypress) {
              drawBackground(background);
              state = PLAY;
          }
        }
        else if (state == PLAY)
        {
            clearItems(background);
            movePipes();
            evaluate();
            fall();
            if (KEY_DOWN_NOW(BUTTON_UP))
            {
                if (upKeypress)
                {
                    birb.y -= flyHeight - 2;
                }
                else
                {
                    flyLower();
                }
            }
            else if (KEY_DOWN_NOW(BUTTON_RIGHT))
            {
                birb.x = birb.x + 2;
            }
            else if (KEY_DOWN_NOW(BUTTON_LEFT))
            {
                birb.x = birb.x - 2;
            }
            else if (KEY_DOWN_NOW(BUTTON_SELECT)) {
                state = START;
            }

            if (!status() && !KEY_DOWN_NOW(BUTTON_A))
            {
                state = END;
            }

            drawBird();
            drawPipes();
        }
        else if (state == END)
        {
            drawBackground(gameoverScreen);
            char result[5];
            sprintf(result, "%d", score);
            drawString(68, 150, result, WHITE);
            drawString(150, (SCREEN_WIDTH - 126) / 2, "Press ENTER to restart", WHITE);
            state = END_WAIT;
        }
        else if (state == END_WAIT)
        {
            if (KEY_DOWN_NOW(BUTTON_START) && !startKeypress)
            {
                state = START;
            }
        }

        if (KEY_DOWN_NOW(BUTTON_START))
        {
            startKeypress = 1;
        }
        else
        {
            startKeypress = 0;
        }

        if (KEY_DOWN_NOW(BUTTON_UP))
        {
            upKeypress = 1;
        }
        else
        {
            upKeypress = 0;
        }

      }
}

//reset game status
void reset()
{
    pipes = malloc(sizeof(PIPE) * numPipes);
    currPipe = pipes;
    initiatePipes();
    birb.x = SCREEN_WIDTH / 6;
    birb.y = SCREEN_HEIGHT / 2 - BIRD_HEIGHT / 2;
    score = 0;
}

//generate pipe data
void initiatePipes()
{
    showPipe(pipes);
    for (int i = 1; i < numPipes; ++i)
    {
        pipes[i].visible = 0;
        genPipeHeight(pipes + i);
        pipes[i].x = pipes[0].x + i * pipeDistance;
    }
}

//show pipe
void showPipe(PIPE *pipe)
{
    (*pipe).visible = 1;
    (*pipe).x = SCREEN_WIDTH - PIPENECKBOTTOM_WIDTH - 1;
    genPipeHeight(pipe);
}

//check collisions
int checkCollision(BIRD *bird1, PIPE *pipe)
{
    if ((*bird1).y > (*pipe).topHeight - 1 && (*bird1).y + BIRD_HEIGHT < (*pipe).topHeight + (*pipe).botHeight)
    {
        return 0;
    }
    else
    {
        if ((*bird1).x + BIRD_WIDTH < (*pipe).x || (*bird1).x > (*pipe).x + PIPENECKBOTTOM_WIDTH)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

//check status
int status()
{
    return !checkCollision(&birb, currPipe) && birb.y < SCREEN_HEIGHT;
}

//randomize a pipe
void genPipeHeight(PIPE *pipe)
{
    int base = PIPENECKBOTTOM_HEIGHT + 10;
    (*pipe).botHeight = (BIRD_HEIGHT * 3) + rand() % (BIRD_HEIGHT * 4);
    (*pipe).topHeight = rand() % (SCREEN_HEIGHT - (*pipe).botHeight - base) + (base / 2);
}

// draw the bird
void drawBird()
{
    drawImage3(birb.y, birb.x, BIRD_WIDTH, BIRD_HEIGHT, bird);
}

//clear
void clearItems(const u16 *image) {
    //clear bird
    clear(birb.y, birb.x, BIRD_WIDTH, BIRD_HEIGHT, image);

    //clear pipes
    for (int i = 0; i < numPipes; ++i) {
        clearPipe(pipes + i, background);
    }
}
//remove pipe
void clearPipe(PIPE *pipe, const u16 *image)
{
    if (!(*pipe).visible)
    {
        return;
    }
    for (int i = 0; i < (*pipe).topHeight - PIPENECKBOTTOM_HEIGHT; ++i)
    {
        clear(i, (*pipe).x + pipeMargin + PIPEBODY_WIDTH - 1, pipeSpeed + 1, PIPEBODY_HEIGHT, image);
    }

    clear((*pipe).topHeight - PIPENECKBOTTOM_HEIGHT, (*pipe).x + PIPENECKBOTTOM_WIDTH - 1, pipeSpeed, PIPENECKBOTTOM_HEIGHT, image);
    clear((*pipe).topHeight + (*pipe).botHeight, (*pipe).x + PIPENECKBOTTOM_WIDTH - 1, pipeSpeed, PIPENECKBOTTOM_HEIGHT, image);
    for (int i = 0; i < SCREEN_HEIGHT - ((*pipe).topHeight + (*pipe).botHeight + PIPENECKBOTTOM_HEIGHT); ++i)
    {
        clear(i + (*pipe).topHeight + (*pipe).botHeight + PIPENECKBOTTOM_HEIGHT, (*pipe).x + pipeMargin + PIPEBODY_WIDTH - 1,
        pipeSpeed + 1, PIPEBODY_HEIGHT, image);
    }
}

void drawPipes() {
    for (int i = 0; i < numPipes; ++i) {
        drawPipe(pipes + i);
    }
}

//draw pipe bro
void drawPipe(PIPE *pipe)
{
    if (!(*pipe).visible)
    {
        return;
    }
    if ((*pipe).x < 0)
    {
        for (int i = 0; i < (*pipe).topHeight - PIPENECKBOTTOM_HEIGHT; ++i)
        {
            drawImage3FromCol(i, -1, -(*pipe).x - pipeMargin, PIPEBODY_WIDTH + 1, PIPEBODY_HEIGHT, pipeBody);
        }
        drawImage3FromCol((*pipe).topHeight - PIPENECKBOTTOM_HEIGHT, 0, -(*pipe).x, PIPENECKBOTTOM_WIDTH, PIPENECKBOTTOM_HEIGHT, pipeNeckTop);
        drawImage3FromCol((*pipe).topHeight + (*pipe).botHeight, 0, -(*pipe).x, PIPENECKBOTTOM_WIDTH, PIPENECKBOTTOM_HEIGHT, pipeNeckBottom);

        for (int i = 0; i < SCREEN_HEIGHT - ((*pipe).topHeight + (*pipe).botHeight + PIPENECKBOTTOM_HEIGHT); ++i)
        {
            drawImage3FromCol(i + (*pipe).topHeight + (*pipe).botHeight + PIPENECKBOTTOM_HEIGHT, -1, -(*pipe).x - pipeMargin, PIPEBODY_WIDTH + 1, PIPEBODY_HEIGHT, pipeBody);
        }
    }
    else
    {
        for (int i = 0; i < (*pipe).topHeight - PIPENECKBOTTOM_HEIGHT; ++i)
        {
            drawImage3(i, (*pipe).x + pipeMargin, PIPEBODY_WIDTH, PIPEBODY_HEIGHT, pipeBody);
        }
        drawImage3((*pipe).topHeight - PIPENECKBOTTOM_HEIGHT, (*pipe).x, PIPENECKBOTTOM_WIDTH, PIPENECKBOTTOM_HEIGHT, pipeNeckTop);
        drawImage3((*pipe).topHeight + (*pipe).botHeight, (*pipe).x, PIPENECKBOTTOM_WIDTH, PIPENECKBOTTOM_HEIGHT, pipeNeckBottom);

        for (int i = 0; i < SCREEN_HEIGHT - ((*pipe).topHeight + (*pipe).botHeight + PIPENECKBOTTOM_HEIGHT); ++i)
        {
            drawImage3(i + (*pipe).topHeight + (*pipe).botHeight + PIPENECKBOTTOM_HEIGHT, (*pipe).x + pipeMargin, PIPEBODY_WIDTH, PIPEBODY_HEIGHT, pipeBody);
        }
    }

}


void fall()
{
    birb.y = birb.y + gravity;
}

void fly()
{
    birb.y = birb.y - flyHeight;
}

void flyLower()
{
    birb.y = birb.y - flyHeight - 3;
}

void evaluate()
{
    if (birb.x - ((*currPipe).x + PIPEBODY_WIDTH) == 0)
    {
        score = score + 1;
    }
}

//move pipes
void movePipes()
{
    if (nextPipe != NULL && (*currPipe).x + PIPENECKBOTTOM_WIDTH < 0)
    {
        free(pipes);
        pipes = nextPipe;
        currPipe = nextPipe;
        initiatePipes();
        nextPipe = NULL;
    }
    for (int i = 0; i < numPipes; ++i)
    {
        pipes[i].x = pipes[i].x - pipeSpeed;
        if (pipes[i].x < SCREEN_WIDTH - PIPENECKBOTTOM_WIDTH && pipes[i].x + PIPENECKBOTTOM_WIDTH > 0)
        {
            pipes[i].visible = 1;
        }
        else
        {
            pipes[i].visible = 0;
        }
        if (pipes[i].x <= birb.x + BIRD_WIDTH)
        {
            if (i == numPipes - 1)
            {
                nextPipe = malloc(sizeof(PIPE) * numPipes);
            }
            else
            {
                nextPipe = NULL;
            }
            currPipe = pipes + i;
        }
    }

}
