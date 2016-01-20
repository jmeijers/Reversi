/* 
 * File:   Lab6.c
 * Author: James
 *
 * Created on October 23, 2015, 5:30 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * 
 */

bool positionInBounds(int n, int row, int col){
    if(row >= 0 && row < n && col >= 0 && col < n){
        return true;
    }
    else 
        return false;
}

//creates a new board for the start of a game
void makeBoard(char board[26][26], int n){
    int i, k;
    for(i = 0; i < 26; i++)
        for(k = 0; k < 26; k++)
            board[i][k] = 'U';
    
    board[n/2][n/2] = board[n/2 - 1][n/2 - 1] = 'W';
    board[n/2 - 1][n/2] = board[n/2][n/2 - 1] = 'B';
}

//receives board as input from the user
void getBoard(char board[26][26], int n){
    char colour;
    char row, col;
    printf("Enter board configuration:\n");
    
    scanf(" %c%c%c", &colour, &row, &col);
    while(colour != '!' && row != '!' && col != '!'){
        if(positionInBounds(n, row - 'a', col - 'a'))
            board[row - 'a'][col - 'a'] = colour;
        scanf(" %c%c%c", &colour, &row, &col);
    }
    
}


bool checkLegalInDirection(char board[26][26], int n, int row, int col, 
        char colour, int deltaRow, int deltaCol){ 
    int i;
    if(positionInBounds(n, row, col)){
        if(board[row][col] == 'U'){
            //position is on the board and is unoccupied
            if(positionInBounds(n, row + deltaRow, col + deltaCol)){
                if(board[row + deltaRow][col + deltaCol] != colour 
                        && board[row + deltaRow][col + deltaCol] != 'U'){
                    for(i = 1; (positionInBounds(n, row + i*deltaRow, col + i*deltaCol)) && (board[row + deltaRow*i][col + deltaCol*i] != 'U'); i++){
                        if(board[row + deltaRow*i][col + deltaCol*i] == colour){
                            return true;
                        }
                    }
                }     
            }
        }
    }
    return false;
}

bool checkLegalInPosition(char board[26][26], int n, int row, int col, 
        char colour){
    int deltaRow, deltaCol;
    for(deltaRow = -1; deltaRow <= 1; deltaRow++){
        for(deltaCol = -1; deltaCol <= 1; deltaCol++){
            if(deltaCol != 0 || deltaRow != 0){
                if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)){
                    return true;
                }
            }
        }
    }
    return false;
}

bool movesAvailable(char board[26][26], int n, char colour){
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(checkLegalInPosition(board, n , i, j, colour)){
                return true;
            }
        }
    }
    return false;
}

//finds number of moves for colour
int findMoves(char board[26][26], int n, char colour){
    int i, k;
    int moves = 0;
    for(i = 0; i < n; i++){
        for(k = 0; k < n; k++){      
            if(checkLegalInPosition(board, n, i, k, colour)){
                moves++;
            }
        }
    }
    return moves;
}



void makeMoveInDirection(char board[26][26], int n, int row, int col, 
        char colour, int deltaRow, int deltaCol){ 
    int i;
    
    for(i = 1; positionInBounds(n, row + i*deltaRow, col + i*deltaCol) && board[row + deltaRow*i][col + deltaCol*i] != 'U'; i++){
        if(board[row + deltaRow*i][col + deltaCol*i] != colour){
            board[row + deltaRow*i][col + deltaCol*i] = colour;
        }
        else if(board[row + deltaRow*i][col + deltaCol*i] == colour){
            return; 
        }
    } 
}

void makeMove(char board[26][26], int n, int row, int col, char colour){
    if(checkLegalInPosition(board, n, row, col, colour)){
        int deltaRow, deltaCol;
        for(deltaRow = -1; deltaRow <= 1; deltaRow++){
            for(deltaCol = -1; deltaCol <= 1; deltaCol++){
                if(deltaCol != 0 || deltaRow != 0){
                    if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)){
                        makeMoveInDirection(board, n, row, col, colour, deltaRow, deltaCol);
                    }
                }
            }    
        }
        board[row][col] = colour;
    }
    else 
        printf("\nInvalid move.\n");
}

void printBoard(char board[26][26], int n){
    int i, k;
    printf("  ");
    for(i = 0; i < n; i++)
        printf("%c", 'a' + i);
    printf("\n");
    for(i = 0; i < n; i++){
        printf("%c ", 'a' + i);
        for(k = 0; k < n; k++){
            printf("%c", board[i][k]);
        }
        printf("\n");
    }
}

int tilesInDirection(char board[26][26], int n, char colour, int row, int col, 
        int deltaRow, int deltaCol){
    int i, flips;
    flips = 0;
    if(positionInBounds(n, row, col)){
        if(board[row][col] == 'U'){
            //position is on the board and is unoccupied
            if(positionInBounds(n, row + deltaRow, col + deltaCol)){
                if(board[row + deltaRow][col + deltaCol] != colour 
                        && board[row + deltaRow][col + deltaCol] != 'U'){
                    for(i = 1; (positionInBounds(n, row + i*deltaRow, col + i*deltaCol)) && (board[row + deltaRow*i][col + deltaCol*i] != 'U'); i++){
                        if(board[row + deltaRow*i][col + deltaCol*i] == colour){
                            return flips;
                        }
                        flips++;
                    }
                }     
            }
        }
    }
    return 0;
}


int tilesInPosition(char board[26][26], int n, char colour, int row, int col){
    int deltaRow, deltaCol;
    int flips = 0;
    if(checkLegalInPosition(board, n, row, col, colour)){
        for(deltaRow = -1; deltaRow <= 1; deltaRow++){
            for(deltaCol = -1; deltaCol <= 1; deltaCol++){
                if(deltaCol != 0 || deltaRow != 0){
                    flips = flips + tilesInDirection(board, n, colour, row, col, deltaRow, deltaCol);
                }
            }
        }
    }
    return flips;
}

int edgeScorer(char board[26][26], int n, int row, int col){
    int score = 7; 
    int deltaRow, deltaCol;
    for(deltaRow = -1; deltaRow <= 1; deltaRow++){
        for(deltaCol = -1; deltaCol <= 1; deltaCol++){
            if(deltaRow != 0 || deltaCol != 0)
                if(positionInBounds(n, row + deltaRow, col + deltaCol))
                    if(board[row + deltaRow][col + deltaCol] == 'U')
                        score--;
        }
    }
    return score;
    
}

void scorePosition(char board[26][26], int scoreBoard[26][26][2], int n, char colour, int row, int col){
    //if it isn't a legal move, no score
    if(!(checkLegalInPosition(board, n , row, col, colour))){
        scoreBoard[row][col][0] = 0;
        scoreBoard[row][col][1] = 0;
        return;
    }
    if(((row == 0) || (row == (n-1))) && ((col == 0) || (col == (n-1)))){
        scoreBoard[row][col][0] = -1;
        scoreBoard[row][col][1] = -1;
        return;
    }
    int edgeScore, moveScore;
    moveScore = tilesInPosition(board, n, colour, row, col);
    edgeScore = edgeScorer(board, n, row, col);
    
    scoreBoard[row][col][0] = moveScore;
    scoreBoard[row][col][1] = edgeScore;   
}

void scoreTheBoard(char board[26][26], int scoreBoard[26][26][2], int n, char colour){
    int col, row;
    for(row = 0; row < n; row++){
        for(col = 0; col < n; col++){
            scorePosition(board, scoreBoard, n, colour, row, col);
        }
    }
}


void decideMove(char board[26][26], int n, char colour, int *numberOfMoves){
    int bestRow, bestCol, i, j;
    double highScore, currentScore;
    bool largeMove = true;
    int scoreBoard[26][26][2];
    scoreTheBoard(board, scoreBoard, n, colour);
    int moves = findMoves(board, n, colour);
    if(*numberOfMoves < (n*n)/4)
        largeMove = false;
    highScore = currentScore = 0; 
    for(i = 0; i < n && highScore >= 0; i++){
        for(j = 0; j < n && highScore >= 0; j++){
            if(scoreBoard[i][j][0] == 0)
                currentScore = 0;
            else {
                if(scoreBoard[i][j][0] == -1){
                    printf("Computer places %c at %c%c.\n", colour, i + 'a', j + 'a');
                    makeMove(board, n, i, j, colour);
                    return;
                }
                else if(largeMove)
                    currentScore = (scoreBoard[i][j][0])/2 + scoreBoard[i][j][1];
                else
                    currentScore = 1 / (double)scoreBoard[i][j][0] + (double)scoreBoard[i][j][1] / 7;
            }
            if(currentScore > highScore){
                bestRow = i;
                bestCol = j;
                highScore = currentScore;
            }
        }
    }
    

    
    printf("Computer places %c at %c%c.\n", colour, bestRow + 'a', bestCol + 'a');
    makeMove(board, n, bestRow, bestCol, colour);
}


char findWinner(char board[26][26], int n){
    int i, j;
    int B = 0;
    int W = 0;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(board[i][j] == 'B')
                B++;
            else if (board[i][j] == 'W')
                W++;
        }
    }
    if(B > W)
        return 'B';
    if (W > B)
        return 'W';
    else
        return 'T';
}



int main(int argc, char** argv) {
    char board[26][26];
    int n;
    int numberOfMoves = 0;
    int *moves = &numberOfMoves;
    char computerColour, currentColour;
    printf("Enter the board dimension: ");
    scanf("%d", &n);
    makeBoard(board, n);
    printf("Computer plays (B/W) : ");
    scanf(" %c", &computerColour);
    currentColour = 'B';
    char nextColour = 'W';
    char tempColour;
    bool gameOver = 0;
    char rowMove, colMove;
    
    printBoard(board, n);
    
    while(!gameOver){
        if(movesAvailable(board, n, currentColour)){
            if(currentColour == computerColour){
                decideMove(board, n, computerColour, moves);
                numberOfMoves++;
            }
            else{
                printf("Enter move for colour %c (RowCol): ", currentColour);
                scanf(" %c%c", &rowMove, &colMove);
                if(checkLegalInPosition(board, n, rowMove - 'a', colMove - 'a', currentColour)){
                    makeMove(board, n, rowMove - 'a', colMove - 'a', currentColour);
                    numberOfMoves++;
                }
                else{
                    printf("Invalid move.\n");
                    printf("%c player wins.\n", findWinner(board, n));
                    return 0;
                }
            }
            printBoard(board, n);
            
        }
        else{
            printf("%c player has no valid move.\n", currentColour);
        }
        
        tempColour = currentColour;
        currentColour = nextColour;
        nextColour = tempColour;
        
        if(!movesAvailable(board, n, 'B') && !movesAvailable(board, n, 'W')){
            printf("%c player wins.\n", findWinner(board, n));
            gameOver = true;
        }
        
    }
    
    
    return (EXIT_SUCCESS);
}