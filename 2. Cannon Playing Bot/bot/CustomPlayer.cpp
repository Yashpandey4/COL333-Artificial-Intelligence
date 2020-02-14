#include <iostream>
#include <vector>
#include <unordered_map>
#include <pthread.h>
#include "GameMatrix.h"
#include "AlphaBeta.h"
#include <algorithm>
#include <chrono> 
#include <ctime> 
using namespace std;

string IterativeDeepningAlphaBeta(GameMatrix *Game,int player, double time_left, double total_time)
{
    int DEPTH=8;
    int depth=1;
    string bestMove = "";
    double time_limit = 2;
    if(time_left>total_time - total_time/8)
    {
        time_limit = 1.25;
    }
    else if(time_left>total_time - total_time/4)
    {
        time_limit = 1.75;
    }
    else if(time_left>total_time/4)
    {
        time_limit = 2;
    }
    else if(time_left>total_time/8)
    {
        time_limit = 1.25;
    }
    else
    {
        time_limit = 1;
    }
    // transposition_table.erase(transposition_table.begin(),transposition_table.end());
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    try
    {
        while (depth < DEPTH)
        {
            // cout <<"///////////////ForDepth: "<<depth<<" /////////////////" << endl;
            transposition_table.erase(transposition_table.begin(),transposition_table.end());
            bestMove = ALPHA_BETA_SEARCH(Game, depth, start, time_limit);
            depth++;
        }
    }
    catch(const std::exception& TimeException)
    {
        // std::cerr << TimeException.what() << '\n';
        return bestMove;
    }
    return bestMove;
}

int main(int argc, char const *argv[])
{
    initTable();
    int player;
    cin >> player;
    int n;
    cin >> n;
    int m;
    cin >> m;
    double time_left;
    cin >> time_left;
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = start - start;    
    // GameMatrix game(n,m,12);
    GameMatrix *game = new GameMatrix(n,m,12);
    // cout << player << endl;
    // return 0;
    double total_time = time_left;
    if(player==2)
    {
        // printMatrix(game->SimMatrix);
        // printGameMatrix(game->SoldierMatrix);
        string move1 ="";
        string move2 ="";
        string move3 ="";
        string move4 ="";
        string move5 ="";
        string move6 ="";
        elapsed_seconds = elapsed_seconds + std::chrono::system_clock::now()-start;    
        cin >> move1;cin >> move2;cin >> move3;cin >> move4;cin >> move5;cin >> move6;
        start = std::chrono::system_clock::now();
        string move ="";
        move = move + move1+move2+move3+move4+move5+move6;
        move[1] = char((7 - (int(move[1])-int('0')))+int('0'));
        move[2] = char((7 - (int(move[2])-int('0')))+int('0'));
        move[4] = char((7 - (int(move[4])-int('0')))+int('0'));
        move[5] = char((7 - (int(move[5])-int('0')))+int('0'));
        game->ExecuteMoves(move);        
        game->Moves.push_back(move);
        // printGameMatrix(game->SoldierMatrix);
    }
    while (1)
    {
        time_left = time_left - elapsed_seconds.count();
        string bestmove = IterativeDeepningAlphaBeta(game, player, time_left, total_time);
        // cout << bestmove << endl;
        // cout << game->childStates.size() << endl;
        // cout << "size:" <<game->childStates.size()<<endl;
        // GameMatrix *temp1 = new GameMatrix();
        // temp1 = game->childStates[0]; 
        // game->childStates.erase(game->childStates.begin()); 
        // game->~GameMatrix(); 
        // game = temp1;
        // cout << game->childStates.size();
        // printMoves(game->FutureMoves);
        game->Delete();
        game->ExecuteMoves(bestmove);
        game->Moves.push_back(bestmove);
        game->childStates.clear();
        // cout << bestmove << endl;        
        if(player == 2)
        {
            bestmove[1] = char((7 - (int(bestmove[1])-int('0')))+int('0'));
            bestmove[2] = char((7 - (int(bestmove[2])-int('0')))+int('0'));
            bestmove[4] = char((7 - (int(bestmove[4])-int('0')))+int('0'));
            bestmove[5] = char((7 - (int(bestmove[5])-int('0')))+int('0'));
        }
        cout << bestmove[0] << " " << bestmove[1] << " " << bestmove[2] << " " << bestmove[3] << " " << bestmove[4] << " " << bestmove[5] << " " << "\n";

        // printMatrix(game->SimMatrix);
        // printGameMatrix(game->SoldierMatrix);
        string move1 ="";
        string move2 ="";
        string move3 ="";
        string move4 ="";
        string move5 ="";
        string move6 ="";
        elapsed_seconds = elapsed_seconds + std::chrono::system_clock::now()-start;   
        // cout << "elapsed time: " << elapsed_seconds.count() << "s\n"; 
        // printGameMatrix(game->SoldierMatrix); 
        cin >> move1;cin >> move2;cin >> move3;cin >> move4;cin >> move5;cin >> move6;
        start = std::chrono::system_clock::now();
        string move ="";
        move= move+ move1+move2+move3+move4+move5+move6;
        
        if(player==2)
        {
            move[1] = char((7 - (int(move[1])-int('0')))+int('0'));
            move[2] = char((7 - (int(move[2])-int('0')))+int('0'));
            move[4] = char((7 - (int(move[4])-int('0')))+int('0'));
            move[5] = char((7 - (int(move[5])-int('0')))+int('0'));
        }
        // game->ExecuteMoves(move);
        game->ExecuteMoves(move);
        game->Moves.push_back(move);
        // int i = 0;
        // bool done = false;
        // GameMatrix *temp = new GameMatrix();
        // // cout << game->childStates.size() <<" "<<game->childStates[i]->Moves.size()<< endl;
        // vector<GameMatrix*>::iterator it = game->childStates.begin();
        // for (i = 0; i < game->childStates.size(); i++)
        // {
        //     // cout << game->childStates[i]->Moves[game->childStates[i]->Moves.size()-1]<<" "<< game->childStates[i]->Moves[0]<< endl;
        //     if(move==game->childStates[i]->Moves[game->childStates[i]->Moves.size()-1])
        //     {
        //         temp = game->childStates[i];
        //         done = true;
        //         break;
        //     }
        //     it++;
        // }
        // if(i==game->childStates.size())
        // {
        //     game->childStates.erase(game->childStates.begin(), game->childStates.end());
        //     game->ExecuteMoves(move);
        //     game->Moves.push_back(move);
        //     // return 0;
        // }
        // else
        // {    
        //     game->childStates.erase(it);
        //     game->~GameMatrix(); 
        //     game = temp;
        //     game->childStates.erase(game->childStates.begin(), game->childStates.end());
        // }

    }
    return 0;
}
