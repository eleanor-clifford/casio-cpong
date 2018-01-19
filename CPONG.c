/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : [ProjectName].c                                 */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/
#include "fxlib.h"


//****************************************************************************
//  AddIn_main (Sample program main function)
//
//  param   :   isAppli   : 1 = This application is launched by MAIN MENU.
//                        : 0 = This application is launched by a strip in eACT application.
//
//              OptionNum : Strip number (0~3)
//                         (This parameter is only used when isAppli parameter is 0.)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
void reset();
void pause();
void endGame(int,int);
int AddIn_main(int isAppli, unsigned short OptionNum)
{
    unsigned int key;

    Bdisp_AllClr_DDVRAM();
    const int BAT_SIZE
    //TODO: Set ViewWindow
    
    float player1Pos = 40f;
    float player2Pos = 40f;
    int sensitivity = 4;
    //TODO: input speed
    int speed = 1;
    int player1Score = 0;
    int player2Score = 0;

    int i = -1;
    Bdisp_AllClr_DDVRAM(); // clear
    PrintXY(5,57,"START",0); // print to VRAM
    Bdisp_PutDisp_DD(); // copy VRAM to Display Driver
    // LBL 0 (reset)
    typedef struct { 
        int x;
        int y;
        int xV;
        int yV;
    } Ball;
    Ball *ball = malloc(sizeof(Ball)); 
    
    Sleep(500);
    
    while(1)
    {
        i++;
        GetKey(&key);
        switch (key) 
        {
            case 72:
                player1Pos += sensitivity;
                break;
            case 73:
                player1Pos -= sensitivity;
                break;
            case 37:
                player2Pos += sensitivity;
                break;
            case 28:
                player2Pos -= sensitivity;
                break;
            case 44:
                pause();
                break;
        }
        ball->x += ball->xV;
        ball->y += ball->yV;
        switch (ball->x)
        {
            case 5:
                if (ballY < player1Pos + BAT_SIZE && ballY > player1Pos - BAT_SIZE)
                    vBallY *= -1;
                break;
            case 120: 
                if (ballY < player2Pos + BAT_SIZE && ballY > player2Pos - BAT_SIZE
                    vBallY *= -1;
                break;
            case 1: 
                if (++player2Score == 7) {
                    // GOTO 1
                    endGame(player1Score,player2Score);
                } else {
                    // GOTO 0
                    reset(ball);
                    continue; // restart game loop
                }
            case 125:
                if (++player1Score == 7) {
                    // GOTO 1
                    endGame(player1Score,player2Score);
                } else {
                    // GOTO 0
                    reset(ball);
                    continue; // restart game loop
                }
        }
        switch (ball->y) // optimisation - compiler will generate jump tables
        {
            case 1:
            case 63:
                vBallY *= -1;
                break; 
        }
        // TODO: SPEED
        /*
         * Cls
         * F-Line 5,playerXPos - BAT_SIZE,5,playerXPos + BAT_SIZE
         * F-Line 122,playerYPos - BAT_SIZE,122,playerYPos + BAT_SIZE
         * F-Line ball->x,ball->y,ball->x+1,ball->y
         * F-Line ball->x,ball->y+1,ball->x+1,ball->y+1
         * Text 5,50,player1Score
         * Text 5,80,player2Score
         */
        Bdisp_AllClr_DDVRAM();
        Bdisp_DrawLineVRAM(5,playerXPos - BAT_SIZE,5,playerXPos + BAT_SIZE);
        BDisp_DrawLineVRAM(122,playerYPos - BAT_SIZE,122,playerYPos + BAT_SIZE);
        BDisp_SetPoint_VRAM(ball->x,ball->y,1);
        BDisp_SetPoint_VRAM(ball->x,ball->y+1,1);
        BDisp_SetPoint_VRAM(ball->x+1,ball->y,1);
        BDisp_SetPoint_VRAM(ball->x+1,ball->y+1,1);
        PrintXY(5,50,player1Score,0);
        PrintXY(5,80,player2Score);
        Bdisp_PutDisp_DD();
    }
    return 1;
}
void pause()
{
    BDisp_AllClr_DDVRAM();
    PrintXY(1,1,"PAUSED",0);
    Bdisp_PutDisp_DD();
    unsigned int key;
    do {
        GetKey(&key);
    } while (key != 31)
}
void reset(Ball *ball) 
{
    ball->x = 60;
    ball->y = 40;
    ball->xV = 5;
    ball->yV = 3;
    Sleep(500);
}
void endGame()
{
    // Lbl 1
    /* 
     * Cls
     * Text 5,5,"GAME OVER"
     * Text 15,5,"PLAYER 1: "
     * Text 15,42, player1Score
     * Text 25,5, "PLAYER 2: "
     * Text 25,42, player2Score
     * Stop
     */
    BDisp_AllClr_DDVRAM();
    PrintXY(5,5,"GAME OVER",0);
    PrintXY(15,5,"PLAYER 1: ",0);
    PrintXY(15,42, player1Score,0);
    PrintXY(25,5, "PLAYER 2: ",0);
    PrintXY(25,42, player2Score,0);
    Bdisp_PutDisp_DD();
    exit 0;
}




//****************************************************************************
//**************                                              ****************
//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************
//**************                                              ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section

