#ifndef APLHABETA_H
#define APLHABETA_H
#include <iostream>
#include "GameMatrix.h"
#include <bits/stdc++.h> 
#include <chrono> 
#include <ctime> 

using namespace std;

class TimeException : public exception{  
    public:  
        const char * what() const throw()  
        {  
            return "Threshold Time exceeded!\n";  
        }  
}; 


int MIN_VALUE(GameMatrix *state, int alpha, int beta, int d, std::chrono::time_point<std::chrono::system_clock> start, double time_limit, int parentvalue);
int MAX_VALUE(GameMatrix *state, int alpha, int beta, int d, std::chrono::time_point<std::chrono::system_clock> start, double time_limit, int parentvalue);
int quisecentmax(GameMatrix *state, int depth, int parentvalue);
int quisecentmin(GameMatrix *state, int depth, int parentvalue);

unsigned long long int ZobristTable[8][8][4]; 
mt19937 mt(01234567); 
  
// Generates a Randome number from 0 to 2^64-1 
unsigned long long int randomInt() 
{ 
    uniform_int_distribution<unsigned long long int> dist(0, UINT64_MAX); 
    return dist(mt); 
} 
void initTable() 
{ 
    for (int i = 0; i<8; i++) 
      for (int j = 0; j<8; j++) 
        for (int k = 0; k<4; k++) 
          ZobristTable[i][j][k] = randomInt(); 
} 
int indexOf(int piece) 
{ 
    if (piece==3) 
        return 0; 
    if (piece==7) 
        return 1; 
    if (piece==-3) 
        return 2; 
    if (piece==-7) 
        return 3; 
    else
        return -1; 
} 
// Computes the hash value of a given board 
unsigned long long int computeHash(vector<vector<int>> SimMatrix) 
{ 
    unsigned long long int h = 0; 
    for (int i = 0; i<8; i++) 
    { 
        for (int j = 0; j<8; j++) 
        { 
            if (SimMatrix[i][j]!=0) 
            { 
                int piece = indexOf(SimMatrix[i][j]); 
                h ^= ZobristTable[i][j][piece]; 
            } 
        } 
    } 
    return h; 
} 
//Taken from AIMA
/* function ALPHA-BETA-SEARCH(state) returns an action
    v ←MAX-VALUE(state,−∞,+∞)
    return the action in ACTIONS(state) with value v

function MAX-VALUE(state,α, β) returns a utility value
    if TERMINAL-TEST(state) then return UTILITY(state)
    v ←−∞
    for each a in ACTIONS(state) do
        v ←MAX(v, MIN-VALUE(RESULT(s,a),α, β))
        if v ≥ β then return v
        α←MAX(α, v)
    return v

function MIN-VALUE(state,α, β) returns a utility value
    if TERMINAL-TEST(state) then return UTILITY(state)
    v ←+∞
    for each a in ACTIONS(state) do
        v ←MIN(v, MAX-VALUE(RESULT(s,a) ,α, β))
        if v ≤ α then return vstate->MeNoOfTownhalls
        β←MIN(β, v)
    return v */


unordered_map<unsigned long long int, int> transposition_table;
bool compareMaxStates(GameMatrix* i1, GameMatrix* i2) 
{ 
    return (i1->EvaluatedValue > i2->EvaluatedValue); 
} 
bool compareMinStates(GameMatrix* i1, GameMatrix* i2) 
{ 
    return (i1->EvaluatedValue < i2->EvaluatedValue); 
} 

int cannons(GameMatrix *state)
{
    vector<float> cannoncount(3,0);//(270,90), (225,135), (315,45)
    vector<float> cannonbad(3,0);//bad 315,45 cannon, bad 225,135 cannon
    vector<float> sidecan(2,0);//left and right
    vector<float> badsidecan(2,0);//bad left and bad right
    vector<float> backcan(3,0);
    vector<float> backcanbad(3,0);
    for(auto p:(state->MeSoldiers)){
        if(p.second.cannonEnd)
        {
            for (int i = 0; i < p.second.direction.size(); i++)
            {
                if(p.second.direction[i]==90) 
                {
                    if(p.second.y<2) cannonbad[0] = cannonbad[0] + 1;
                    else cannoncount[0] = cannoncount[0] + 1;
                }
                else if(p.second.direction[i]==135)
                {
                    if(p.second.x<2||p.second.y<2) cannonbad[1] = cannonbad[1] + 1;
                    else cannoncount[1] = cannoncount[1] + 1;
                }
                else if(p.second.direction[i]==45)
                {
                    if(p.second.x>5||p.second.y<2) cannonbad[2] = cannonbad[2] + 1;
                    else cannoncount[2] = cannoncount[2] + 1;
                }
                else if(p.second.direction[i]==0)
                {
                    if(p.second.x>5) badsidecan[0] = badsidecan[0] + 1;
                    else sidecan[0] = sidecan[0] + 1;
                }
                else if(p.second.direction[i]==180)
                {
                    if(p.second.x<2) badsidecan[1] = badsidecan[1] + 1;
                    else sidecan[1] = sidecan[1] + 1;
                }
                else if(p.second.direction[i]==270) 
                {
                    if(p.second.y>5) backcanbad[0] = backcanbad[0] + 1;
                    else backcan[0] = backcan[0] + 1;
                }
                else if(p.second.direction[i]==225)
                {
                    if(p.second.x<2||p.second.y>5) backcanbad[1] = backcanbad[1] + 1;
                    else backcan[1] = backcan[1] + 1;
                }
                else if(p.second.direction[i]==315)
                {
                    if(p.second.x>5||p.second.y>5) backcanbad[2] = backcanbad[2] + 1;
                    else backcan[2] = backcan[2] + 1;
                }
            }
        }
    }
    for (auto p:(state->OpSoldiers))
    {
        if(p.second.cannonEnd)
        {
            for (int i = 0; i < p.second.direction.size(); i++)
            {
                if(p.second.direction[i]==90) 
                {
                    if(p.second.y<2) backcanbad[0] = backcanbad[0] - 1;
                    else backcan[0] = backcan[0] - 1;
                }
                else if(p.second.direction[i]==135)
                {
                    if(p.second.x<2||p.second.y<2) backcanbad[1] = backcanbad[1] - 1;
                    else backcan[1] = backcan[1] - 1;
                }
                else if(p.second.direction[i]==45)
                {
                    if(p.second.x>5||p.second.y<2) backcanbad[2] = backcanbad[2] - 1;
                    else backcan[2] = backcan[2] - 1;
                }
                else if(p.second.direction[i]==0)
                {
                    if(p.second.x>5) badsidecan[0] = badsidecan[0] - 1;
                    else sidecan[0] = sidecan[0] - 1;
                }
                else if(p.second.direction[i]==180)
                {
                    if(p.second.x<2) badsidecan[1] = badsidecan[1] - 1;
                    else sidecan[1] = sidecan[1] - 1;
                }
                else if(p.second.direction[i]==270) 
                {
                    if(p.second.y>5) cannonbad[0] = cannonbad[0] - 1;
                    else cannoncount[0] = cannoncount[0] - 1;
                }
                else if(p.second.direction[i]==225)
                {
                    if(p.second.x<2||p.second.y>5) cannonbad[1] = cannonbad[1] - 1;
                    else cannoncount[1] = cannoncount[1] - 1;
                }
                else if(p.second.direction[i]==315)
                {
                    if(p.second.x>5||p.second.y>5) cannonbad[2] = cannonbad[2] - 1;
                    else cannoncount[2] = cannoncount[2] - 1;
                }
            }
        }
    }

    int cannonkills1 = 0; int cannonkills2 = 0;

    for (auto p:(state->MeSoldiers))
    {
        cannonkills1 = cannonkills1+p.second.CannonKills.size();
    }
    for (auto p:(state->OpSoldiers))
    {
        cannonkills2 = cannonkills2+p.second.CannonKills.size();
    }
    return 60*(cannonkills1-cannonkills2)+ 10*(cannoncount[0]+cannoncount[1]+cannoncount[2]) + 0*(backcan[0]+backcan[1]+backcan[2]) + (sidecan[0]+sidecan[1])
                                         -10*(cannonbad[0]+cannonbad[1]+cannonbad[2]) + 0*(backcanbad[0]+backcan[1]+backcan[2]) -(badsidecan[0]+badsidecan[1]);
}

int evaluation(GameMatrix *state)
{
    int t1 = state->MeNoOfTownhalls;
    int t2 = state->OpNoOfTownhalls;
    int s1 = state->MeNoOfSoldiers;
    int s2 = state->OpNoOfSoldiers;
    int p = cannons(state);
    return 6000*(t1-t2) + 400*(s1-s2) + p;
}

// int quisecentmin(GameMatrix *state, int depth, int parentvalue)
// {
//     int e = evaluation(state);
//     if((e-parentvalue>100||e-parentvalue<100) && depth!=0)
//     {
        
//         state->GenerateStates(true);
//         int v = INT32_MAX;
//         for (int i = 0; i < state->childStates.size(); i++)
//         {
//             v = min(v, quisecentmax(state->childStates[i], depth-1, e));
//         }
//         return v;
//     }
//     else
//     {
//         return e;
//     }
// }
// int quisecentmax(GameMatrix *state, int depth, int parentvalue)
// {
//     int e = evaluation(state);
//     if((e-parentvalue>100||e-parentvalue<100) && depth!=0)
//     {
//         state->GenerateStates(false);
//         int v = INT32_MIN;
//         for (int i = 0; i < state->childStates.size(); i++)
//         {
//             v = max(v, quisecentmin(state->childStates[i], depth-1, e));
//         }
//         return v;
//     }
//     else
//     {
//         return e;
//     }
// }

int MAX_VALUE(GameMatrix *state, int alpha, int beta, int d,std::chrono::time_point<std::chrono::system_clock> start, double time_limit, int parentvalue)
{ 
    std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now()-start;
    // cout << elapsed_seconds.count() << "s" << endl;
    if(elapsed_seconds.count()>time_limit)
    {
        TimeException t;
        throw t;
    }
    if(state->MeNoOfTownhalls==2 || state->OpNoOfTownhalls == 2)
    {
        if(state->MeNoOfTownhalls==2 )
        {
            return (-1*(state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4 + 4)*(-12000);
        }
        else if(state->OpNoOfTownhalls==2 )
        {
            return ((state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4 + 4)*12000;
        }
    }
    if(state->MeNoOfSoldiers==0 || state->OpNoOfSoldiers == 0)
    {
        if(state->MeNoOfSoldiers==0)
        {
            return (-1*(state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4 + 6)*(-12000);
        }
        if(state->OpNoOfSoldiers==0)
        {
            return ((state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4 + 6)*(12000);
        }
    }
    if(d==0)
    {
        int e = evaluation(state);
        //return EVAL(s)
        // if(e-parentvalue>100 || e-parentvalue<100)
        // {
        //     //quiscent search
        //     return quisecentmax(state, 2, parentvalue);
        // }
        // else 
            return e;
    }
    if(state->childStates.size()==0)
    {
        // cout << "go man" << endl;
        state->GenerateStates(false);
    }
    //if remain zero
    if(state->childStates.size()==0)
    {
        //I don't have immediate moves
        return ((state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4-2)*(12000);
    }
    ///////////////////
    int v = INT32_MIN;
    int temp = 0;
    unsigned long long int hash;
    for (int i = 0; i < state->childStates.size(); i++)
    {
        hash = computeHash(state->childStates.at(i)->SimMatrix);
        if(transposition_table.find(hash)==transposition_table.end())
        {
            temp = MIN_VALUE((state->childStates.at(i)), alpha, beta, d-1, start, time_limit, parentvalue);
        }
        else
        {
            temp = transposition_table.at(hash);
        }
        v = max(v, temp);
        parentvalue = v;
        state->childStates.at(i)->EvaluatedValue = temp;
        transposition_table[hash] = temp;
        if(v>=beta) 
        {
            sort(state->childStates.begin(), state->childStates.end(), compareMaxStates);
            return v;
        }
        alpha=max(alpha, v);
    }
    //sort the childrens of this state according to evaluated value
    sort(state->childStates.begin(), state->childStates.end(), compareMaxStates);
    // cout << "After Sortng States in Max Value at depth: " <<d <<" "<<state->childStates[0]->EvaluatedValue << endl;
    // cout << "done" << endl;
    ///////////////////////////////////////////////////////////////
    return v;
}

int MIN_VALUE(GameMatrix *state, int alpha, int beta, int d, std::chrono::time_point<std::chrono::system_clock> start, double time_limit, int parentvalue)
{
    std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now()-start;
    // cout << elapsed_seconds.count() << "s" << endl;

    if(elapsed_seconds.count()>time_limit)
    {
        TimeException t;
        throw t;
    }
    if(state->MeNoOfTownhalls==2 || state->OpNoOfTownhalls == 2)
    {
        if(state->MeNoOfTownhalls==2 )
        {
            return (-1*(state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4 + 4)*(-12000);
        }
        else if(state->OpNoOfTownhalls==2)
        {
            return ((state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4 + 4)*12000;
        }
    }
    if(state->MeNoOfSoldiers==0 || state->OpNoOfSoldiers == 0)
    {
        if(state->MeNoOfSoldiers==0)
        {
            return (-1*(state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4 + 6)*(-12000);
        }
        if(state->OpNoOfSoldiers==0)
        {
            return ((state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4 + 6)*(12000);
        }
    }
    if(d==0)
    {
        //return EVAL(s)
        int e = evaluation(state);
        //return EVAL(s)
        // if(e-parentvalue>100 || e-parentvalue<100)
        // {
        //     //quiscent search
        //     return quisecentmin(state, 2, parentvalue);
        // }
        // else 
            return e;
    }
    int v = INT32_MAX;
    int temp = 0;
    // //generate children
    if(state->childStates.size()==0)
    {
        state->GenerateStates(true);
    }
    if(state->childStates.size()==0)
    {
        //I don't have immediate moves
        return ((state->MeNoOfTownhalls-state->OpNoOfTownhalls)*4+2)*(12000);
    }
    ///////////////////
    unsigned long long int hash;
    for (int i = 0; i < state->childStates.size(); i++)
    {
        hash = computeHash(state->childStates.at(i)->SimMatrix);
        if(transposition_table.find(hash)==transposition_table.end())
        {
            temp = MAX_VALUE((state->childStates.at(i)), alpha, beta, d-1, start, time_limit,parentvalue);
        }
        else
        {
            temp = transposition_table.at(hash);
        }
        v = min(v, temp);
        parentvalue = v;
        state->childStates.at(i)->EvaluatedValue = temp;
        transposition_table[hash] = temp;
        if(v<=alpha) 
        {
            sort(state->childStates.begin(), state->childStates.end(), compareMinStates);
            return v;
        }
        beta=min(beta, v);
    }
    //sort the childrens of this state according to evaluated value
    sort(state->childStates.begin(), state->childStates.end(), compareMinStates);
    ///////////////////////////////////////////////////////////////
    return v;
}

string ALPHA_BETA_SEARCH(GameMatrix *state, int d,std::chrono::time_point<std::chrono::system_clock> start, double time_limit)
{
    // pair<int, vector<GameMatrix>> temppair;
    int value = 0;
    value = MAX_VALUE(state, INT32_MIN, INT32_MAX, d, start, time_limit, INT32_MIN);
    state->EvaluatedValue = value;
    //binary search to find the value;
    // cout << "Before returning alpha beta" << state->childStates[0]->EvaluatedValue << endl;
    return state->childStates[0]->Moves.at(state->childStates[0]->Moves.size()-1);
}

#endif