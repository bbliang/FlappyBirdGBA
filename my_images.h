/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=3 my_images background.png bird.png gameoverScreen.png pipeBody.png pipeNeckBottom.png pipeNeckTop.png startScreen.png 
 * Time-stamp: Monday 11/13/2017, 18:32:28
 * 
 * Image Information
 * -----------------
 * background.png 240@160
 * bird.png 17@12
 * gameoverScreen.png 240@160
 * pipeBody.png 24@1
 * pipeNeckBottom.png 26@12
 * pipeNeckTop.png 26@12
 * startScreen.png 240@160
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef MY_IMAGES_H
#define MY_IMAGES_H

extern const unsigned short background[38400];
#define BACKGROUND_SIZE 76800
#define BACKGROUND_LENGTH 38400
#define BACKGROUND_WIDTH 240
#define BACKGROUND_HEIGHT 160

extern const unsigned short bird[204];
#define BIRD_SIZE 408
#define BIRD_LENGTH 204
#define BIRD_WIDTH 17
#define BIRD_HEIGHT 12

extern const unsigned short gameoverScreen[38400];
#define GAMEOVERSCREEN_SIZE 76800
#define GAMEOVERSCREEN_LENGTH 38400
#define GAMEOVERSCREEN_WIDTH 240
#define GAMEOVERSCREEN_HEIGHT 160

extern const unsigned short pipeBody[24];
#define PIPEBODY_SIZE 48
#define PIPEBODY_LENGTH 24
#define PIPEBODY_WIDTH 24
#define PIPEBODY_HEIGHT 1

extern const unsigned short pipeNeckBottom[312];
#define PIPENECKBOTTOM_SIZE 624
#define PIPENECKBOTTOM_LENGTH 312
#define PIPENECKBOTTOM_WIDTH 26
#define PIPENECKBOTTOM_HEIGHT 12

extern const unsigned short pipeNeckTop[312];
#define PIPENECKTOP_SIZE 624
#define PIPENECKTOP_LENGTH 312
#define PIPENECKTOP_WIDTH 26
#define PIPENECKTOP_HEIGHT 12

extern const unsigned short startScreen[38400];
#define STARTSCREEN_SIZE 76800
#define STARTSCREEN_LENGTH 38400
#define STARTSCREEN_WIDTH 240
#define STARTSCREEN_HEIGHT 160

#endif

