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
#include "math.h"

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
#define BALL_SIZE 3
#define TRAIL_SIZE 5
typedef struct tag_Ball { 
    float x;
    float y;
    float xV;
    float yV;
} Ball;
int** trail;
char* goV;
void reset(Ball*);
void pause();
void endGame(int,int);
unsigned char* IntToString(int);
void DrawPlayer(float,float);
void DrawBall(Ball*);
void go(void);
int AddIn_main(int isAppli, unsigned short OptionNum)
{
    unsigned int key1 = 0, key2 = 0;
    short unused = 0;
    
    float player1Pos = 40;
    float player2Pos = 40;
    float sensitivity = 0.5;
    int frametime = 25;
    int trailLength = (int)pow(BALL_SIZE,2)*TRAIL_SIZE; // I know, I know, there's overlap, but it won't matter much.
    trail = (int**)malloc(trailLength*sizeof(int*));
    for (int i = 0; i < trailLength; i++) {
        trail[i] = calloc(2,sizeof(int)); // X,Y
    }
    
    //TODO: input speed
    int speed = 1;
    int player1Score = 0;
    int player2Score = 0;

    int i = -1;
    int j;
    
    const unsigned char *text = (unsigned char*) "START";
    
    Ball *ball = (Ball*)malloc(sizeof(Ball));
    reset(ball);

    Bdisp_AllClr_DDVRAM(); // clear
    PrintXY(5,57,text,0); // print to VRAM
    Bdisp_PutDisp_DD(); // copy VRAM to Display Driver
    
    Sleep(500);
    
    Bkey_Set_RepeatTime(1,1);
    goV = (char*)malloc(sizeof(char));
    while(1)
    {
        *goV = 0;
        SetTimer(1,frametime,&go)
        ball->x += ball->xV * sensitivity;
        ball->y += ball->yV * sensitivity;
        i++;
        
        //Sleep(5);
        
        // CASIO is retarded and made IsKeyDown deprecated, so let's just use !IsKeyUp
        // Okay that doesn't work either. Imma just try a bunch of shit.
        // Found this in some random forum.
        key1 = 0; key2 = 0;
        Bkey_GetKeyWait(&key1,&key2,KEYWAIT_HALTOFF_TIMEROFF,0,0,&unused);
        switch (key1) // switch-case is an optimization: it executes much faster than if else trees as the compiler will generate jump tables
        {
            /* switch-case is an optimization: it executes much faster 
             * than if else trees as the compiler will generate jump tables.
             * 
             * I had to manually binary search the values of key1/key2 for each keypress, 
             * as the documentation was nowhere to be seen.
             * In fact, The Bkey_GetKeyWait function isn't in the documentation either.
             */
            case 2: // KEY_CTRL_UP
                player2Pos -= sensitivity;
                break;
            case 3: // KEY_CTRL_DOWN
                player2Pos += sensitivity;
                break;
            case 4: // KEY_CTRL_DEL
                pause();
                break;
            case 7:
                switch (key2) 
                {
                    case 4: // KEY_CHAR_4
                        player1Pos -= sensitivity;
                        break;
                    case 3: // KEY_CHAR_1
                        player1Pos += sensitivity;
                        break;
                    case 9: // KEY_CTRL_SHIFT
                        sensitivity += 0.1;
                        break;
                    case 8: // KEY_CTRL_ALPHA
                        sensitivity -= 0.1;
                        break;
                }
        }
        switch ((int)ball->x)
        {
           case 2:
           case 3:
           case 4:   
           case 5:
                if (ball->xV < 0 && ball->y < player1Pos + BAT_SIZE && ball->y > player1Pos - BAT_SIZE)
                    ball->xV *= -1;
                break;
            case 120:
            case 121:
            case 122:
            case 123:
            case 124: 
                if (ball->xV > 0 && ball->y < player2Pos + BAT_SIZE && ball->y > player2Pos - BAT_SIZE)
                    ball->xV *= -1;
                break;
            case 0:
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
            case 126:
            case 127:
            case 128:
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
        switch ((int)ball->y)
        {
            case -3:
            case -2:
            case -1:
            case 0:
            case 1:
            case 63:
            case 64:
            case 65:
            case 66:
            case 67:
                ball->yV *= -1;
                break; 
            default:
                break;
        }
        Bdisp_AllClr_DDVRAM();
        Bdisp_AllClr_DDVRAM();
        PrintXY(50,5,IntToString(player1Score),0);
        PrintXY(80,5,IntToString(player2Score),0);
        DrawPlayer(5,player1Pos);
        DrawPlayer(122,player2Pos);
        DrawBall(ball);
        StepTrail();
        Bdisp_PutDisp_DD();
        while (!*goV) {} // wait until the timer has finished
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
    ball->yV = 0.5;
    Sleep(500);
}
void DrawPlayer(float x, float y)
{
    Bdisp_DrawLineVRAM(x,(int)y - BAT_SIZE,x,(int)y + BAT_SIZE);
}
void DrawBall(Ball* ball) 
{
    int k,l;
    for (k = 0; k < BALL_SIZE; k++) {
        for (l = 0; l < BALL_SIZE; l++) {
            Bdisp_SetPoint_VRAM((int)ball->x+k,(int)ball->y+l,1);
            int tr = 0;
            while (*trail[tr++] == 0) {}
            *trail[tr] = ball->x+k;
            *(trail[tr]+1) = ball->y+l;
        }
    }
}
void StepTrail() 
{
    int i;
    B_SQR = (int)pow(BALL_SIZE,2);
    for (i = 0; i < B_SQR; i++)
    {
        Bdisp_SetPoint_VRAM(*trail[i],*(trail[i]+1),0);
        trail[i] = trail[i+B_SQR];
        trail[i+B_SQR] = (int*)calloc(2,sizeof(int));
    }
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
    PrintXY(5,15,p1,0);
    PrintXY(62,15, p1s,0);
    PrintXY(5,25, p2,0);
    PrintXY(62,25, p2s,0);
    Bdisp_PutDisp_DD();
    Sleep(10000);
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
void go() {
	*goV= 1;
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

