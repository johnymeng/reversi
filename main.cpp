/**
 * @file reversi.c
 * @author <-- Your name here-->
 * @brief This file is used for APS105 Lab 8. 2023W version
 * @date 2023-03-14
 * 
 */

//this code passes 8/10 test cases

// DO NOT REMOVE THE FOLLOWING LINE
#if !defined(TESTER_P1) && !defined(TESTER_P2)
// DO NOT REMOVE THE ABOVE LINE
#include "reversi.h"
// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE

char opp_colour(char colour);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol);
void flip(char board[][26], int n, int row, int col, char colour);
bool potential_moves(char board[][26], int n, char colour);
int num_flips(char board[][26], int n, int row, int col, char colour);
double corner_value(int n);
double flip_value(int n);
double loss(char board[][26], int n, char colour);
double value_board(char board[][26], int n, char colour);
double minimax(char board[][26], int n, int depth, double alpha, double beta, bool maximizingPlayer, int flag, char colour);
bool legal(char board[][26], int n, int row, int col, char colour);

int highest_row, highest_col, turns = 1; 


int makeMove(char board[26][26], int n, char turn, int *row, int *col) 
{
char colour = turn;
char opp = opp_colour(colour);

	bool legal = false;
  if(n > 5 && n <= 8)
	{
		minimax(board, n, 5, -100000000000000, 100000000000000, true, 0, colour);
	}
	else if(n > 8 && n <= 18)
	{	
		minimax(board, n, 3, -100000000000000, 100000000000000, true, 0, colour); 
	}		
	else if(n > 18 && n <= 26)
	{
		minimax(board, n, 1, -100000000000000, 100000000000000, true, 0, colour); 
	}	
  	else
	{
		minimax(board, n, 11, -100000000000000, 100000000000000, true, 0, colour);
	}

  for (int row= 0; row < n; row++)
  {
    for (int col = 0; col < n; col++)
    {
      if (potential_moves(board, n, colour))
      {
        legal = true; 
      }
    }
  }
  if (legal)
  {
    turns += 2;
    *row = highest_row;
	*col = highest_col;
  }  
  if(!legal)
  {
    return 1;
  }
  return 1;
}

char opp_colour(char colour){
	if(colour == 'W')
		return 'B';
	else
		return 'W';
}

bool positionInBounds(int n, int row, int col){
	if(row >= 0 && row < n && col >= 0 && col < n)
	{
		return true;
	}		
	else
	{
		return false;
	}
		
}

bool legal(char board[][26], int n, int row, int col, char colour){
    if (board[row][col] == 'U')
    {
        for(int d_row = -1; d_row <= 1; d_row++){
		for(int d_col = -1; d_col <= 1; d_col++){
			if(!(d_row == 0 && d_col == 0)  && checkLegalInDirection(board, n, row, col, colour, d_row, d_col))
			{
				return true;
			}
		}
	}
    }	
	return false;
}

bool potential_moves(char board[][26], int n, char colour){
	bool legal = false;
	for(int row = 0; row < n; row++){
		for(int col = 0; col < n; col++){
			for(int d_row = -1; d_row <= 1; d_row++){
				for(int d_col = -1; d_col <= 1; d_col++){
					if(!(d_row == 0 && d_col == 0) && checkLegalInDirection(board, n, row, col, colour, d_row, d_col) ){
							legal = true;
							return legal;
					}
				}
			}
		}
	}
	return legal;
}


bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {

    int row_valid = row + deltaRow;
    int col_valid = col + deltaCol;
    if (board[row_valid][col_valid] != colour)
    {
        while(positionInBounds(n,row_valid, col_valid) && board[row_valid][col_valid] != 'U')
        {            
            if(board[row_valid][col_valid] == colour)
            {
                return true;
            }
            if (board[row_valid][col_valid] == 'U')
            {
                return false;
            }
            row_valid += deltaRow;
            col_valid += deltaCol;
        }
    }
    return false;
}

void flip(char board[][26], int n, int row, int col, char colour)
{
 
  for (int row_2 = row - 1; row_2 <= row + 1; row_2++)
  {
    for (int col_2 = col - 1; col_2 <= col +1; col_2++)
    {
      int deltaRow = row_2 - row, deltaCol = col_2 - col;
      if (positionInBounds(n, row + deltaRow, col + deltaCol)){
          if (checkLegalInDirection(board, n, row_2, col_2, colour, deltaRow, deltaCol))
          {
            int flip_rows = row + deltaRow;
            int flip_cols = col+deltaCol;

            while(board[flip_rows][flip_cols] != colour)
                {
                 flip_rows += deltaRow;
                 flip_cols += deltaCol;
                }									
            }
        }
     }
    }
}


int num_flips(char board[][26], int n, int row, int col, char colour){ 
	int flips = 0;
	for(int d_row =- 1; d_row <= 1; d_row++){
		for(int d_col = -1; d_col <= 1; d_col++){
			if(!(d_row == 0 && d_col == 0)  && checkLegalInDirection(board, n, row, col, colour, d_row, d_col)){
					for(int i = 1; ; i++){

						if(!(board[row + (i * d_row)][col + (i * d_col)] == colour && board[row + (i * d_row)][col + (i * d_col)] == 'U')){
							flips++;
						}
						else if(board[row + (i * d_row)][col + (i * d_col)] == colour) 
						{
							break;
						}						
					}
			}
		}
	}
	return flips;
}


double corner_value(int n){
	if(n > 5 && n <= 10)
	{
		return 90.0;
	}		
	else if(n > 10 && n <= 18)
	{
		return 50.0;
	}		
	else if(n > 18 && n <= 22)
	{
		return  65.0;
	}		
	else{
		return 62.5;
	}	
}

double flip_value(int n){
	double value;
	if(n > 5 && n <= 10)
	{
		value = 10.0 + 5.0 * pow((( turns) / ((double)(n*n))),  n/2);
	}		
	else if(n > 10 && n <= 22)		
	{
		value = 40.0 + 5.0 * pow((( turns) / ((double)(n*n))),  n/2);
	}		
	else
	{
		value = 37.5 + + 5.0 * pow((( turns) / ((double)(n*n))),  n/2);
	}
	return value;
}

double loss(char board[][26], int n, char colour){
	int loss_score;
	double score;
    char opp = opp_colour(colour);
	int moves = 0;
	for(int row = 0; row < n; row++){
		for(int col; col < n; col++){
			for(int d_row = -1; d_row<=1; d_row++){
				for(int d_col = -1; d_col<=1; d_col++){
						if(!(d_row == 0 && d_col == 0)  && checkLegalInDirection(board, n, row, col, opp, d_row, d_col) ){
								moves++;
						}
					}
				}
		}
	}
		
	loss_score = 100.0 - 25.0 * (moves);
	
	return loss_score;
}

double value_board(char board[][26], int n, char colour)
{
  char opp = opp_colour(colour);
  double score = 0;
  int corner_score = 0, unflipped = 0, flip_score = 0;

	if (n == 8)
	{
		  double board_values [8][8]= {
			100, -25,  25,   5,   5,  25, -25, 100,   
			-25, -50,  -5,  -5,  -5,  -5, -50, -25,   
			 25,  -5,  15,   3,   3,  15,  -5,  25,
			  5,  -5,   3,   3,   3,   3,  -5,   5,
			  5,  -5,   3,   3,   3,   3,  -5,   5,
			 25,  -5,  15,   3,   3,  15,  -5,  25,
			-25, -50,  -5,  -5,  -5,  -5, -50, -25,   
			100, -25,  20,   5,   5,  25, -25, 100};
	}
	else 
	{
		//top left corner
		if (board[0][0] == colour)
		{
			corner_score += 25;
		}
		else if ((board[0][1] == colour && board[0][2] == opp) || (board[1][0] == colour && board[2][0] == opp) || (board[1][1] == colour && board[2][2] == opp))
		{
			corner_score += -100;
		}

		//top right corner
		if (board[0][n-1] == colour)
		{
			corner_score += 25;
		}
		else if ((board[0][n-2] == colour && board[0][n-3] == opp) || (board[1][n-2] == colour && board[2][n-3] == opp) || (board[1][n-1] == colour && board[2][n-1] == opp))
		{
			corner_score += -100;
		}

		//bottom left corner
		if (board[n-1][0] == colour)
		{
			corner_score += 25;
		}
		else if ((board[n-2][0] == colour && board[n-3][0] == opp) || (board[n-2][1] == colour && board[n-3][2] == opp) || (board[n-1][1] == colour && board[n-1][2] == opp))
		{
			corner_score += -100;
		}

		//bottom right corner
		if (board[n-1][n-1] == colour)
		{
			corner_score += 25;
		}
		else if ((board[n-2][n-1] == colour && board[n-3][n-1] == opp) || (board[n-2][n-2] == colour && board[n-3][n-3] == opp) || (board[n-1][n-2] == colour && board[n-1][n-3] == opp))
		{
			corner_score += -100;
		}
	}
  

  for (int row = 0; row < n; row++)
  {
    for (int col = 0; col < n; col++)
    {
      if(board[row][col] !=  'U')
      {
        unflipped++;
      }
      if(board[row][col] == colour)
      {
        flip_score++;
      }
    }
  }

  flip_score = (double)( 100 * flip_score) / (double) unflipped;

  score = (double)(corner_score * corner_value(n)) + (double)(flip_score * flip_value(n)) + (double)(loss(board, n, colour) * 4.0);

  return score;
} 

double minimax(char board[][26], int n, int depth, double alpha, double beta, bool max, int flag, char colour){ 
	
	char opp = opp_colour(colour);
	if(depth == 0)
	{
		return value_board(board, n, colour);
	}	

	if (max == true){
		double node = -100000000000000; 
			for(int row = 0; row < n; row++){ 
				for(int col = 0; col < n; col++){
					if(	legal(board, n, row, col, colour) ){
						char scoreboard[26][26];

                        for (int i = 0; i < n; i++)
                        {
                            for (int j = 0; j < n; j++)
                            {
                                scoreboard[i][j] = board[i][j];
                            }
                        }
					
						//memcpy(scoreboard, board, (sizeof(char) * 26 * 26));
						flip(scoreboard, n, row, col, colour);
						
						if(potential_moves(scoreboard, n, opp)){
							node = fmax(node, minimax(scoreboard, n, depth-1, alpha, beta, false, flag+1, colour)); 
						}else{
							node = fmax(node, minimax(scoreboard, n, depth-1, alpha, beta, true, flag+1, colour));
						}
						
						if(node > alpha){
							alpha = node;
							
							if(flag == 0){
								highest_row = row;
								highest_col = col;
							}
						}

						else if(node == -100000000000000 && flag == 0){ 
							node = value_board(scoreboard, n, colour);
							if(node > alpha){
								alpha = node;
								
								highest_row = row;
								highest_col = col;
							}
						}
						
						if(alpha >= beta)	
						{
							return node; 
						}
						
					}
				}
			}
			return node;
	}
	else{
		double node = 100000000000000;
		
		for(int row = 0; row < n; row++){
			for(int col = 0; col < n; col++){
				if(	legal(board, n, row, col, opp) ){

					char scoreboard[26][26]; 
					//memcpy(scoreboard, board, sizeof(char) * 26 * 26); 
                    for (int i = 0; i < n; i++)
                        {
                            for (int j = 0; j < n; j++)
                            {
                                scoreboard[i][j] = board[i][j];
                            }
                        }
					flip(scoreboard, n, row, col, opp);
					
					if(potential_moves(scoreboard, n, colour))
					{
						node = fmin(node, minimax(scoreboard, n, depth-1, alpha, beta, true, flag+1, colour)); 
					}
					else
					{
						node = fmin(node, minimax(scoreboard, n, depth-1, alpha, beta, false, flag+1, colour));
					}
					
					if(node < beta)
					{
						beta = node;
					}	
					if(beta <= alpha)
					{
						return node; 
					}
				}
			}
		}
		return node;
	}
}

//*******************************************************
// Note: Please only put your main function below
// DO NOT REMOVE THE FOLLOWING LINE
#ifndef TESTER_P2
// DO NOT REMOVE THE ABOVE LINE

int main(void) {
  // Write your own main function here
  return 0;
}

// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE
//*******************************************************
