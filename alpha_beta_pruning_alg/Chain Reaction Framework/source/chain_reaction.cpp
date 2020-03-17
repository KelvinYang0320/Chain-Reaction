#include <iostream>
#include "../include/board.h"
#include "../include/player.h"
#include "../include/rules.h"
#include "../include/algorithm.h"

using namespace std;

int main(){
int win_time=0;
for(int t=0;t<1;t++){

    Board board;
    Player red_player(RED);
    Player blue_player(BLUE);

    bool first_two_step = true;
    int round = 1;
    int index[2];

    while(1){

        //////////// Red Player operations ////////////
        algorithm_A(board, red_player, index);
        board.place_orb(index[0], index[1], &red_player);

        if(rules_violation(red_player)){
            cout<<"[Violation]";
            return 0;
        }

        board.print_current_board(index[0], index[1], round);
        round++;

        if(board.win_the_game(red_player) && !first_two_step){
            cout << "Red Player won the game !!!" << endl;
            win_time+=1;
            break;
            //return 0;
        }

        //////////// Blue Player operations ////////////
        algorithm_D(board, blue_player, index);
        board.place_orb(index[0], index[1], &blue_player);

        if(rules_violation(blue_player)){
            cout<<"[Violation]";
            return 0;
        }
        
        board.print_current_board(index[0], index[1], round);
        round++;

        if(board.win_the_game(blue_player) && !first_two_step){
            cout << "Blue Player won the game !!!" << endl;
            break;
            //return 0;
        }

        first_two_step = false;
    }
}
    cout<<win_time<<"/100"<<endl;
    return 0;
} 