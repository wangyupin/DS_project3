#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "../include/player.h"
#include "../include/algorithm.h"

using namespace std;

int minimax_alpha_beta(Board board,int depth,int a,int b,Player player);
int val_func(Board board,Player player);
int get_val(bool my_turn,Board board,int i_1,int j_1,int i_2,int j_2);

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions
 * 1. ~ 4. are listed in next block)
 *
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 *
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/


char op_color,color;
int minimax_row,minimax_col;


void algorithm_A(Board board, Player player, int index[]){
    color=player.get_color();
    if('r'==color) op_color='b';
    else if('b'==color) op_color='r';
    int temp_row,temp_col,temp=-100;

    minimax_alpha_beta(board,4,INT_MIN,INT_MAX,player);
    index[0]=minimax_row;
    index[1]=minimax_col;

}


int minimax_alpha_beta(Board board,int depth,int a,int b,Player player){//init a=-infinite, b=+infinite
    if(depth==0||board.win_the_game(player))
        return val_func(board,player);
    if(player.get_color()==color){ //max
        Player op(op_color);
        //int best_val=INT_MIN;
        for(int i=0;i<5;i++){
            for(int j=0;j<6;j++){
                if(board.get_cell_color(i,j)==color||board.get_cell_color(i,j)=='w'){
                    Board temp_board=board;
                    temp_board.place_orb(i,j,&player);
                    int temp_val=minimax_alpha_beta(temp_board,depth-1,a,b,op);
                    if(temp_val>a){
                        a=temp_val;
                        if(depth==4){
                            minimax_row=i;
                            minimax_col=j;
                        }
                    }
                    if(b<=a) break;
                }
            }
            if(b<=a) break;
        }
        return a;
    }else if(player.get_color()==op_color){ //min
        Player this_player(color);
        //int best_val=INT_MAX;
        for(int i=0;i<5;i++){
            for(int j=0;j<6;j++){
                if(board.get_cell_color(i,j)==op_color||board.get_cell_color(i,j)=='w'){
                    Board temp_board=board;
                    temp_board.place_orb(i,j,&player);
                    int temp_val=minimax_alpha_beta(temp_board,depth-1,a,b,this_player);
                    if(temp_val<b)
                        b=temp_val;
                    if(b<=a) break;
                }
            }
            if(b<=a) break;
        }
        return b;
    }
}

int val_func(Board board,Player player){
    int val=0,orb=0,op_orb=0;
    int dir[8][2]={{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};

    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            if(board.get_cell_color(i,j)==color){
                orb+=board.get_orbs_num(i,j);
                bool safe=true;
                int cnt=0;
                int left=board.get_capacity(i,j)-board.get_orbs_num(i,j);
                for(int k=0;k<8;k++){
                    int near_i=i+dir[k][0],near_j=j+dir[k][1];
                    if((0<=near_i&&near_i<ROW) && (0<=near_j&&near_j<COL)){
                        if(board.get_cell_color(near_i,near_j)==op_color){
                           int op_left=board.get_capacity(near_i,near_j)-board.get_orbs_num(near_i,near_j);
                            if(op_left<=1){
                                safe=false;
                                val-=8-board.get_capacity(i,j);
                            }
                        }else if((board.get_cell_color(near_i,near_j)==color)&&(left<=1)){
                            if((board.get_capacity(near_i,near_j)-board.get_orbs_num(near_i,near_j)<=1)){
                                val+=2;
                            }
                        }
                    }
                }
                if(safe){
                    if(board.get_capacity(i,j)==3)
                        val+=5;
                    else if(board.get_capacity(i,j)==5)
                        val+=3;
                    if(left==1)
                        val+=3;
                }

            }else if(board.get_cell_color(i,j)==op_color) op_orb+=board.get_orbs_num(i,j);
        }
    }


    val+=orb;
    if(orb>0&&op_orb==0)
        val=10000;
    else if(orb==0&&op_orb>0)
        val=-10000;
    return val;

}
