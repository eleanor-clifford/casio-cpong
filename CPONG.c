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
#define BAT_SIZE 6
typedef struct tag_Ball { 
    int x;
    int y;
    int xV;
    int yV;
} Ball;
void reset(Ball*);
void pause();
void endGame(int,int);
unsigned char* IntToString(int);
void DrawPlayer(float,float);
void DrawBall(Ball*);
int AddIn_main(int isAppli, unsigned short OptionNum)
{
    unsigned int key;
    
    float player1Pos = 40;
    float player2Pos = 40;
    int sensitivity = 1;
    
    //TODO: input speed
    int speed = 1;
    int player1Score = 0;
    int player2Score = 0;

    int i = -1;
    int j;
    
    const unsigned char *text = (unsigned char*) "START";
    
    Ball *ball = (Ball*) malloc(sizeof(Ball));
    reset(ball);

    Bdisp_AllClr_DDVRAM(); // clear
    PrintXY(5,57,text,0); // print to VRAM
    Bdisp_PutDisp_DD(); // copy VRAM to Display Driver
    
    Sleep(500);
    Bkey_Set_RepeatTime(1,1);
    while(1)
    {
        ball->x += ball->xV;
        ball->y += ball->yV;
        i++;
        
        Sleep(100);
        
        // CASIO is retarded and made IsKeyDown deprecated, so let's just use !IsKeyUp

        if (!IsKeyUp(KEY_CHAR_4)) {
            player1Pos -= sensitivity;
        }
        if (!IsKeyUp(KEY_CHAR_1)) {
            player1Pos += sensitivity;
        }
        if (!IsKeyUp(KEY_CTRL_UP)) {
            player2Pos -= sensitivity;
        }
        if (!IsKeyUp(KEY_CTRL_DOWN)) {
            player2Pos += sensitivity;
        }
        if (!IsKeyUp(KEY_CTRL_DEL)) {
            pause();
        }
        switch (ball->x)
        {
            case 5:
                if (ball->y < player1Pos + BAT_SIZE && ball->y > player1Pos - BAT_SIZE)
                    ball->xV *= -1;
                break;
            case 120: 
                if (ball->y < player2Pos + BAT_SIZE && ball->y > player2Pos - BAT_SIZE)
                    ball->xV *= -1;
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
                break;
            case 125:
                if (++player1Score == 7) {
                    // GOTO 1
                    endGame(player1Score,player2Score);
                } else {
                    // GOTO 0
                    reset(ball);
                    continue; // restart game loop
                }
                break;
            default:
                break;
        }
        switch (ball->y) // optimisation - compiler will generate jump tables
        {
            case 1:
            case 63:
                ball->yV *= -1;
                break; 
            default:
                break;
        }
        // TODO: SPEED
        //const unsigned char* test = (unsigned char*)"TEST";
        /*
        Bdisp_AllClr_DDVRAM();
        for (j = 0; j < key; j++) {
            Bdisp_SetPoint_VRAM(j+5,25,1);
        }
        */
        //PrintXY(60,5,IntToString((int)key),0);
        Bdisp_AllClr_DDVRAM();
        PrintXY(50,5,IntToString(player1Score),0);
        PrintXY(80,5,IntToString(player2Score),0);
        DrawPlayer(5,player1Pos);
        DrawPlayer(122,player2Pos);
        DrawBall(ball);
        Bdisp_PutDisp_DD();
    }
    return 1;
}
void pause()
{
    
    const unsigned char* text = (unsigned char*)"PAUSED";
    unsigned int key;
    
    Bdisp_AllClr_DDVRAM();
    PrintXY(1,1,text,0);
    Bdisp_PutDisp_DD();
    
    GetKey(&key);
}

void reset(Ball* ball) 
{
    ball->x = 60;
    ball->y = 40;
    ball->xV = 1;
    ball->yV = 1;
    Sleep(500);
}
void DrawPlayer(float x, float y)
{
    Bdisp_DrawLineVRAM(x,(int)y - BAT_SIZE,x,(int)y + BAT_SIZE);
}
void DrawBall(Ball* ball) 
{
    Bdisp_SetPoint_VRAM(ball->x,ball->y,1);
    Bdisp_SetPoint_VRAM(ball->x,ball->y+1,1);
    Bdisp_SetPoint_VRAM(ball->x+1,ball->y,1);
    Bdisp_SetPoint_VRAM(ball->x+1,ball->y+1,1);
}

void endGame(int player1Score,int player2Score)
{
    // Lbl 1
    const unsigned char* ov = (unsigned char*)"GAME OVER";
    const unsigned char* p1 = (unsigned char*)"PLAYER 1: ";
    const unsigned char* p2 = (unsigned char*)"PLAYER 2: ";
    const unsigned char* p1s = IntToString(player1Score);
    const unsigned char* p2s = IntToString(player2Score);
    
    Bdisp_AllClr_DDVRAM();
    PrintXY(5,5,ov,0);
    PrintXY(15,5,p1,0);
    PrintXY(15,42, p1s,0);
    PrintXY(25,5, p2,0);
    PrintXY(25,42, p2s,0);
    Bdisp_PutDisp_DD();
    Sleep(100);
    //exit 0;
}
unsigned char* IntToString(int in)
{
    // NOTE: Does not work for int > 9
    unsigned char* out = (unsigned char*)malloc(2*sizeof(unsigned char));
    *(out+1) = '\x0';
    *out = (unsigned char) in + 48; 
    return out;
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

