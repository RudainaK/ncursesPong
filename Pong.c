//
//  main.c
//  Pong
//
//  Created by Rudaina Khalil on 5/3/20.
//  Copyright © 2020 Rudaina Khalil. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h> 
#include <stdlib.h>
#include <unistd.h>

int srows;
int scols;

int wrows;
int wcols;

struct Paddle { //declaring the Paddle datatype
    //current positions
    int xPos;
    int yPos;
    //symbol
    char paddleSymbol;
    int previousMove;  
    
}LeftPaddle,RightPaddle;

struct ball {
    int xPos;
    int yPos;
    char ballSymbol;
}Ball;

//the delay function was taken from geeksforgeeks, full credit goes to them
void delay(double seconds){
    double milliSeconds = 1000*seconds;
    clock_t start = clock();
    
    while(clock()<start+milliSeconds);
}


void printBoard(char board[][wrows], WINDOW* window){ //simply prints the board
    int i = 0;
    int j = 0;
    for(i = 0; i<wrows;i++){
        for(j = 0; j<wcols;j++){ 
            if((i == 0 || i == wrows-1) && j!=0 && j!=wcols-1){//borders
                board[j][i] = '-';
            }
            if((j == 0 || j == wcols-1) && i!=0 && i!=wrows-1){
                board[j][i] = '|';
            }
            wprintw(window,"%c",board[j][i]);
        }
    }
}


void printScore(int currentLeftScore, int currentRightScore, WINDOW* window){
    int rows,cols;
    getmaxyx(window, rows, cols);
    for(int i = 0; i<((cols-1)/2)-2 ;i++){
        wprintw(window, " ");
    }
    wprintw(window, "Score\n");
    for(int i = 0; i<((cols-1)/2)-1 ;i++){
        wprintw(window, " ");
    }
    wprintw(window, "%d-%d",currentLeftScore,currentRightScore);
    wrefresh(window);
}


bool paddleCollision(int addArgsLater){
    //insert code here
    return false;
}



int main(int argc, const char * argv[]) {
    //for now it is only a two player game
    srand(time(NULL)); //random seed

    initscr(); //hopefully placing this here makes it still work
    cbreak();
    noecho();
    /*
    wprintw(stdscr, "resize window to desired size, press enter when ready");
    keypad(stdscr, true);
    
    int temp = getch();
    while(temp != KEY_ENTER){
        continue;
    }
    clear();
    keypad(stdscr, false);
    */
    
    getmaxyx(stdscr,srows,scols); //max dimensions of stdscr
    
    
    WINDOW* scoreW = newwin(2,scols,0,0);
    WINDOW* boardW = newwin(srows-2,scols,2,0); //newwin(rows,cols,starty,startx)
    curs_set(0);
    //wborder(boardW,0,0,0,0,0,0,0,0);
    
    //wrefresh(boardW);
    //wgetch(boardW);
    
    keypad(boardW,TRUE); //allows for arrow keys to be detected using ncurses 
    //curs_set(0);
    nodelay(boardW, true);

    
    int leftscore = 0;
    int rightscore = 0;
    
    getmaxyx(boardW,wrows,wcols); //max dimensions of the window with the board
    
    char board[wcols][wrows];//board[x][y]
    
    for(int i = 0; i<wcols;i++){
        for(int j = 0; j<wrows;j++){
            board[i][j] = ' ';
        }
    }//prefills with spaces first
    
    
    board[0][0] = '+'; //corners
    board[0][wrows-1] = '+';
    board[wcols-1][0] = '+';
    board[wcols-1][wrows-1] = '+';
    
    //start screen
    mvwprintw(boardW, (wrows/2), (wcols/2)-2, "Pong");
    mvwprintw(boardW, (wrows/2)+1, (wcols/2)-10, "Press Enter to Start");

    
    bool start = true;
    while (start == true){
        int ch = wgetch(boardW); //NOT WORKING
        switch(ch){
            case '\n':
                werase(boardW);
                start = false;
                break;
        }
    }
    

    
    //initial conditions of left paddle and right paddle
    LeftPaddle.paddleSymbol = ']'; //will stay constant
    LeftPaddle.xPos = 5; //will stay constant
    LeftPaddle.yPos = wrows/2; //should be 12 if checked in debugger 
    
    
    RightPaddle.paddleSymbol = '['; //will stay constant
    RightPaddle.xPos = wcols-6; //will stay constant //i suspect this may be causing the seg fault
    RightPaddle.yPos = wrows/2; //should be 12 if checked in debugger 
    
    //initial conditions of ball 
    Ball.ballSymbol = 'o';
    Ball.xPos = wcols/2;
    Ball.yPos = wrows/2;
    
    //starting positions
    board[LeftPaddle.xPos][LeftPaddle.yPos]=LeftPaddle.paddleSymbol;
    board[RightPaddle.xPos][RightPaddle.yPos]=RightPaddle.paddleSymbol;
    board[Ball.xPos][Ball.yPos] = Ball.ballSymbol;
    
    bool continueGame = true;
    
    bool keepMoving = false;
    //int pointsLeft = 0;
    //int pointsRight = 0;
    
    //for the ball's starting angle:

    int dRow = rand()%3 - 1; 
    int dCol = rand()%3 - 1;
    while (dCol == 0){
        dCol = rand()%3 - 1; //dCol can never be 0 at the start, it will otherwise just bounce vertically forever
    }
    
    printScore(leftscore, rightscore, scoreW);
    printBoard(board, boardW);//initially prints the paddles and ball
    while(continueGame == true){
        
        int ch1 = wgetch(boardW); //one switch case for both paddles
        //however, it doesnt allow for both players to press at the same time, potentially somethething for multithreading 
        switch(ch1) {
            case 'w':
                if(LeftPaddle.yPos != 1){
                    LeftPaddle.yPos--; //note: remember that zero as at the top and the positive y direction is down
                    board[LeftPaddle.xPos][LeftPaddle.yPos+1] = ' '; //should move according to screen
                    board[LeftPaddle.xPos][LeftPaddle.yPos] = LeftPaddle.paddleSymbol;
                    LeftPaddle.previousMove = 1;
                }
                break;
            case 's':
                if(LeftPaddle.yPos != wrows-2){
                    LeftPaddle.yPos++;
                    board[LeftPaddle.xPos][LeftPaddle.yPos-1] = ' ';
                    board[LeftPaddle.xPos][LeftPaddle.yPos] = LeftPaddle.paddleSymbol;
                    LeftPaddle.previousMove = -1;
                }
                break;
            case KEY_UP:
                if(RightPaddle.yPos != 1){
                    RightPaddle.yPos--;
                    board[RightPaddle.xPos][RightPaddle.yPos+1] = ' ';
                    board[RightPaddle.xPos][RightPaddle.yPos] = RightPaddle.paddleSymbol;
                    RightPaddle.previousMove = 1;
                }
                break;
            case KEY_DOWN:
                if(RightPaddle.yPos != wrows-2){
                    RightPaddle.yPos++;
                    board[RightPaddle.xPos][RightPaddle.yPos-1] = ' ';
                    board[RightPaddle.xPos][RightPaddle.yPos] = RightPaddle.paddleSymbol;
                    RightPaddle.previousMove = -1;
                }
                break;
            default: //if nothing is pressed i think, hopefully this catches if getch returns ERR 
                RightPaddle.previousMove = 0;
                LeftPaddle.previousMove = 0;
                break;  
        }
        
        
        
        
        
        
        
        
        
    
        //ball movements: check for collisions first 
        if(Ball.yPos == wrows-2 && dRow == 1){  //lower wall collision 
              dRow = -1;
        } else if (Ball.yPos == 1 && dRow == -1){ //upper wall collision
              dRow = 1;
        } else if(Ball.xPos == 1 || Ball.xPos == wcols-2) {
        //if it hits the side walls
            if(leftscore <10 && rightscore <10){
                if(Ball.xPos == 1){ //left wall
                    leftscore+=0; //i know this is kinda useless but I like it 
                    rightscore+=1;
                } else if(Ball.xPos == wcols-2){//right wall
                    leftscore+=1;
                    rightscore+=0;
                }
                //reset board
                board[LeftPaddle.xPos][LeftPaddle.yPos] = ' '; //erases old positions
                board[RightPaddle.xPos][RightPaddle.yPos] = ' ';
                board[Ball.xPos][Ball.yPos] = ' ';
                
                //reset positions to starting positions
                Ball.xPos = wcols/2;
                Ball.yPos = wrows/2;
                LeftPaddle.yPos = wrows/2;
                RightPaddle.yPos = wrows/2;
                
                board[LeftPaddle.xPos][LeftPaddle.yPos]=LeftPaddle.paddleSymbol;
                board[RightPaddle.xPos][RightPaddle.yPos]=RightPaddle.paddleSymbol;
                board[Ball.xPos][Ball.yPos] = Ball.ballSymbol;
                
                //new starting angle for ball
                dRow = rand()%3 - 1; 
                dCol = rand()%3 - 1;
                while (dCol == 0){
                    dCol = rand()%3 - 1; //dCol can never be 0 at the start, it will otherwise just bounce vertically forever
                }
            } else {
                continueGame = false;
            }
            
        } else if (Ball.xPos == 6 && (Ball.yPos == LeftPaddle.yPos || Ball.yPos == LeftPaddle.yPos+1 || Ball.yPos == LeftPaddle.yPos+1) &&  ((dCol == -1 && dRow == 1) || (dCol == -1 && dRow == 0) || (dCol == -1 && dRow == -1)) && keepMoving == false){
            if(Ball.yPos == LeftPaddle.yPos-1){ //bottom corner
                if(LeftPaddle.previousMove == 1){ //paddle up
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = 0;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        keepMoving = true;
                        continue;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagonal
                        dCol = 1;
                        dRow = -1;
                    }
                } else if (LeftPaddle.previousMove == 0){ //paddle still
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = -1;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        keepMoving = true;
                        continue;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagnial
                        dCol = 1;
                        dRow = -1;
                    }
                } else if (LeftPaddle.previousMove == -1){
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = 1;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        dCol = 1;
                        dRow = 1;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagnial
                        dCol = 1;
                        dRow = 0;
                    } 
                    
                }
            } else if (Ball.yPos == LeftPaddle.yPos) { //directly in the middle
                
                if(LeftPaddle.previousMove == 1){ //paddle up
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = 0;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        dCol = 1;
                        dRow = -1;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagonal
                        dCol = 1;
                        dRow = -1;
                    }
                } else if (LeftPaddle.previousMove == 0){ //paddle still
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = 1;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        dCol = 1;
                        dRow = 0;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagnial
                        dCol = 1;
                        dRow = -1;
                    }
                } else if (LeftPaddle.previousMove == 1){
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = 1;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        dCol = 1;
                        dRow = 1;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagnial
                        dCol = 1;
                        dRow = 0;
                    } 
                    
                }                
                
            } else if (Ball.yPos == LeftPaddle.yPos+1){ //top corner
                
                if(LeftPaddle.previousMove == 1){ //paddle up
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = 0;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        dCol = 1;
                        dRow = -1;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagonal
                        dCol = 1;
                        dRow = -1;
                    }
                } else if (LeftPaddle.previousMove == 0){ //paddle still
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = 1;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        continue;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagnial
                        dCol = 1;
                        dRow = 1;
                    }
                } else if (LeftPaddle.previousMove == -1){
                    if (dCol == -1 && dRow == 1){ //ball down diagonal
                        dCol = 1;
                        dRow = 1;
                    } else if (dCol == -1 && dRow == 0){ //ball left
                        continue;
                    } else if (dCol == -1 && dRow == -1){ //ball up diagnial
                        dCol = 1;
                        dRow = 0;
                    } 
                } 
                
                
            }
        
        } else if (Ball.yPos == LeftPaddle.yPos-1 && Ball.xPos == LeftPaddle.xPos &&  ((dCol == -1 && dRow == 1) || (dCol == -1 && dRow == 0) || (dCol == -1 && dRow == -1)) && keepMoving == false){ //top 
            
            if((LeftPaddle.previousMove == 0 || LeftPaddle.previousMove == -1) && dRow == 0){ //unmoving paddles as ball passes over
                keepMoving = true;
                continue; //should prevent freezing
            } else if (LeftPaddle.previousMove == 1 && dRow == 0) {
                dRow = -1;
                dCol = -1;
            } else {
                keepMoving = true;
                continue;
            }
        } else if(Ball.yPos == LeftPaddle.yPos+1 && Ball.xPos == LeftPaddle.xPos &&  ((dCol == -1 && dRow == 1) || (dCol == -1 && dRow == 0) || (dCol == -1 && dRow == -1)) && keepMoving == false) { //bottom
            if((LeftPaddle.previousMove == 0 || LeftPaddle.previousMove == 1) && dRow == 0){ //unmoving paddles as ball passes under
                keepMoving = true;
                continue; //should prevent freezing
            } else if (LeftPaddle.previousMove == -1 && dRow == 0) {
                dRow = 1;
                dCol = -1;
            } else {
                keepMoving = true;
                continue;
            } 
        } else if (Ball.xPos == wcols-7 && (Ball.yPos == RightPaddle.yPos || Ball.yPos == RightPaddle.yPos+1 || Ball.yPos == RightPaddle.yPos+1) &&  ((dCol == 1 && dRow == 1) || (dCol == 1 && dRow == 0) || (dCol == 1 && dRow == -1)) && keepMoving == false){
            if(Ball.yPos == RightPaddle.yPos+1){
                if (RightPaddle.previousMove == 1){ //ball below
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = 0;
                    } else if (dCol == 1 && dRow == 0){ //ball left
                        dCol = -1;
                        dRow = -1;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagonal
                        dCol = -1;
                        dRow = -1;
                    }
                } else if(RightPaddle.previousMove == 0) {
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = -1;
                    } else if (dCol == 1 && dRow == 0){ //ball left
                        keepMoving = true;
                        continue;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagnial
                        dCol = -1;
                        dRow = -1;
                    }
                } else if(RightPaddle.previousMove == -1) {
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = 1;
                    } else if (dCol == 1 && dRow == 0){ //ball left
                        dCol = -1;
                        dRow = 1;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagnial
                        dCol = -1;
                        dRow = 0;
                    }
                }
            } else if (Ball.yPos == RightPaddle.yPos){
                if(RightPaddle.previousMove == 1){ //paddle up
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = 0;
                    } else if (dCol == 1 && dRow == 0){ //ball left
                        dCol = -1;
                        dRow = -1;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagonal
                        dCol = -1;
                        dRow = -1;
                    }
                } else if (RightPaddle.previousMove == 0){ //paddle still
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = 1;
                    } else if (dCol == 1 && dRow == 0){ //ball left
                        dCol = -1;
                        dRow = 0;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagnial
                        dCol = -1;
                        dRow = -1;
                    }
                } else if (RightPaddle.previousMove == -1){
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = 1;
                    } else if (dCol == 1 && dRow == 0){ //ball left
                        dCol = -1;
                        dRow = 1;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagnial
                        dCol = -1;
                        dRow = 0;
                    } 
                    
                }
            } else if (Ball.yPos == RightPaddle.yPos-1){ //ball above
                if(RightPaddle.previousMove == 1){ //paddle up
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = 0;
                    } else if (dCol == 1 && dRow == 0){ //ball left
                        dCol = 1;
                        dRow = -1;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagonal
                        dCol = 1;
                        dRow = -1;
                    }
                } else if (RightPaddle.previousMove == 0){ //paddle still
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = 1;
                    } else if (dCol == 1 && dRow == 0){ //ball right
                        dCol = 1;
                        dRow = 0;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagnial
                        dCol = -1;
                        dRow = 1;
                    }
                } else if (RightPaddle.previousMove == -1){
                    if (dCol == 1 && dRow == 1){ //ball down diagonal
                        dCol = -1;
                        dRow = 1;
                    } else if (dCol == 1 && dRow == 0){ //ball left
                        dCol = -1;
                        dRow = 1;
                    } else if (dCol == 1 && dRow == -1){ //ball up diagnial
                        dCol = -1;
                        dRow = 0;
                    } 
                    
                }
            }
        
        } else if (Ball.yPos == RightPaddle.yPos-1 && Ball.xPos == RightPaddle.xPos &&  ((dCol == 1 && dRow == 1) || (dCol == 1 && dRow == 0) || (dCol == 1 && dRow == -1)) && keepMoving == false){
            if(RightPaddle.previousMove == 0 && dRow == 0){ //unmoving paddles as ball passes over
                keepMoving = true;
                continue; //should prevent freezing
            } else if (RightPaddle.previousMove == 1 && dRow == 0) {
                dRow = -1;
                dCol = 1;
            } else {
                keepMoving = true;
                continue;
            } 
        } else if (Ball.yPos == RightPaddle.yPos+1 && Ball.xPos == RightPaddle.xPos &&  ((dCol == 1 && dRow == 1) || (dCol == 1 && dRow == 0) || (dCol == 1 && dRow == -1)) && keepMoving == false){
            if(RightPaddle.previousMove == 0 && dRow == 0){ //unmoving paddles as ball passes under
                keepMoving = true;
                continue; //should prevent freezing
            } else if (RightPaddle.previousMove == -1 && dRow == 0) {
                dRow = 1;
                dCol = 1;
            } else {
                keepMoving = true;
                continue;
            } 
            
        }else if (Ball.yPos != 0 || Ball.yPos != wrows-1 || Ball.xPos != 0 || Ball.xPos != wcols-1){ //if no collisions
            usleep(90000); //delay so ball can actually be seen by players
            board[Ball.xPos][Ball.yPos] = ' '; //old positions
            Ball.yPos+=dRow; //updated positions
            Ball.xPos+=dCol;
            board[Ball.xPos][Ball.yPos] = Ball.ballSymbol;
            keepMoving = false;
        }
        
            
        
          
        
        
        //refresh screen
        werase(boardW);
        werase(scoreW);
        printBoard(board, boardW);
        printScore(leftscore, rightscore, scoreW);
        
        

        
        
        
        
        
    } //while game loop 
    //END SCREEN HERE
    werase(boardW);
    werase(scoreW);
    mvwprintw(boardW, (wrows/2), (wcols/2)-8, "Game Over, %c wins",leftscore>rightscore?'L':'R');
    mvwprintw(boardW, (wrows/2)+1, (wcols/2)-6, "Press Enter");
    
    bool end = false;
    while (end == false){
        int ch = wgetch(boardW); //NOT WORKING
        switch(ch){
            case '\n':
                werase(boardW);
                end = true;
                break;
        }
    }
    
    //game is done 
    
    delwin(boardW);
    delwin(scoreW);
    endwin();
    return 0;
        
        
        
    
    
}

