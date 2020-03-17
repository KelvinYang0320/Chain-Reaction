#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

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
/*------class vector------*/
template <class T>
class  Vector
{
public:

    typedef T * iterator;

    Vector(){
        my_capacity = 0;
        my_size = 0;
        buffer = 0;
    }     
    ~Vector(){
        delete[ ] buffer;
    }
    unsigned int size() const{return my_size;}
    bool empty() const;
    void push_back(const T & v){
        if (my_size >= my_capacity)
            reserve(my_capacity +5);
        buffer [my_size++] = v;
    }
    void pop_back(){my_size--;};

    T & operator[](unsigned int index){return buffer[index];}
    void clear(){
        my_capacity = 0;
        my_size = 0;
        buffer = 0;
    }
    bool empty(){return (my_size==0)?1:0;}
private:
    unsigned int my_size;
    unsigned int my_capacity;
    T * buffer;
    void reserve(unsigned int capacity){
        if(buffer == 0)
        {
            my_size = 0;
            my_capacity = 0;
        }    
        T * Newbuffer = new T [capacity];
        unsigned int l_Size = capacity < my_size ? capacity : my_size;
        for (unsigned int i = 0; i < l_Size; i++)
            Newbuffer[i] = buffer[i];
        my_capacity = capacity;
        delete[] buffer;
        buffer = Newbuffer;
    }
};
/*------class vector------*/

struct Pair{
    int first;
    int second;
    Pair(int a, int b):first(a),second(b){};
    Pair():first(0),second(0){};
};
int max(double a, double b){
    return (a>b)?a:b;
}
int min(double a, double b){
    return (a<b)?a:b;
}
double evaluation(Board &board, int color){
    double eval = 0;
    double eval_enemy = 0;
    int enemy_color = (color=='r')?'b':'r';
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            if(board.get_cell_color(i, j) == color)
                eval++;
            else if(board.get_cell_color(i, j) == enemy_color)
                eval_enemy++;
        }
    }
    if(eval==30){
        return 100;
    }
    else if(eval==0){
        return -100;
    }
    else{
        return eval-eval_enemy;
    }
}
int minimax(double pre_eval, Board board, int row, int col, int depth, int alpha, int beta, bool maximizingPlayer, char me, char enemy){
    int color = (maximizingPlayer==false)?me:enemy;
    
    //Board update
    Board board_next = board;
    Player red_player(RED);
    Player blue_player(BLUE);
    if(color=='r')
        board_next.place_orb(row, col, &red_player);
    else if(color=='b')
        board_next.place_orb(row, col, &blue_player);
    else{
        cout<<"[error]"<<endl;
        return 12345;
    }

    double evaluation_of_position = evaluation(board_next, me);
    if (depth == 0 || evaluation_of_position==100 ||evaluation_of_position==0)//eval==100 means game over
        return evaluation_of_position-pre_eval;
    
    int flag=0;
    if (maximizingPlayer){
        double maxEval = -1000000;//very small number
        //for each position
        for(int i=0;i<5;i++){
            for(int j=0;j<6;j++){
                if(board.get_cell_color(row, col) != me && board.get_cell_color(row, col) != 'w')continue;
                double eval = minimax(evaluation_of_position-pre_eval,board_next, i, j, (depth - 1), alpha, beta, false, me, enemy);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if(beta <= alpha){
                    flag=1;
                    break;
                } 
            }
            if(flag==1)break;
        }
        return maxEval;
    }
    else{
        double minEval = +100000;//very large number
        //for each position
        for(int i=0;i<5;i++){
            for(int j=0;j<6;j++){
                if(board.get_cell_color(row, col) != enemy && board.get_cell_color(row, col) != 'w')continue;
                double eval = minimax(evaluation_of_position-pre_eval,board_next, i, j, (depth - 1), alpha, beta, true, me, enemy);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if(beta <= alpha){
                    flag = 1;
                    break;
                }
                    
            }
            if(flag==1)break;
        }
        return minEval;
    }
}

void algorithm_A(Board board, Player player, int index[]){

    Vector<Pair> best_pos;
    int color = player.get_color();
    //check 4 corner
    if(board.get_cell_color(0, 0) == 'w'){best_pos.push_back(Pair(0,0));}
    if(board.get_cell_color(0, 5) == 'w'){best_pos.push_back(Pair(0,5));}
    if(board.get_cell_color(4, 0) == 'w'){best_pos.push_back(Pair(4,0));}
    if(board.get_cell_color(4, 5) == 'w'){best_pos.push_back(Pair(4,5));}
    if(best_pos.empty()!=1){
        srand(time(NULL));
        int pos=rand()%best_pos.size();
        index[0]=best_pos[pos].first;
        index[1]=best_pos[pos].second;
        return;
    }
    //alpha-beta pruning algorithm
    char me = player.get_color();
    char enemy = (me=='r')?'b':'r';
    double Max_eval=-10000;
    for(int row=0;row<5;row++){
        for(int col=0;col<6;col++){
            if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w'){
                double eval=minimax(evaluation(board, me),board, row, col, 0, -10000, 10000, false, me, enemy);
                if(eval>Max_eval){
                    Max_eval = eval;
                    best_pos.clear();
                    best_pos.push_back(Pair(row,col));
                }else if(eval==Max_eval){
                    best_pos.push_back(Pair(row,col));
                }
            }
        }
    }
    srand(time(NULL));
    int pos = rand()%best_pos.size();
    index[0] = best_pos[pos].first;
    index[1] = best_pos[pos].second;
}