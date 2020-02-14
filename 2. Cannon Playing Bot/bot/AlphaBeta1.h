#ifndef APLHABETA_H
#define APLHABETA_H
#include <iostream>
#include "GameMatrix1.h"
#include <bits/stdc++.h> 

int MAX_VALUE(GameMatrix *state, int alpha, int beta, int d);
int MIN_VALUE(GameMatrix *state, int alpha, int beta, int d);

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
        if v ≤ α then return v
        β←MIN(β, v)
    return v */

int evaluation(int t1, int t2, int s1, int s2)
{
    return 130*(t1-t2) + 10*(s1-s2);
}
unordered_map<unsigned long long int, int> transposition_table;
bool compareMaxStates(GameMatrix* i1, GameMatrix* i2) 
{ 
    return (i1->EvaluatedValue > i2->EvaluatedValue); 
} 
bool compareMinStates(GameMatrix* i1, GameMatrix* i2) 
{ 
    return (i1->EvaluatedValue < i2->EvaluatedValue); 
} 


int MAX_VALUE(GameMatrix *state, int alpha, int beta, int d)
{
    if(d==0)
    {
        //return EVAL(s)
        return evaluation(state->MeNoOfTownhalls, state->OpNoOfTownhalls, state->MeNoOfSoldiers, state->OpNoOfSoldiers);
    }
    if(state->childStates.size()==0)
    {
        state->GenerateStates(false);
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
            temp = MIN_VALUE((state->childStates.at(i)), alpha, beta, d-1);
        }
        else
        {
            temp = transposition_table.at(hash);
        }
        v = max(v, temp);
        state->childStates.at(i)->EvaluatedValue = temp;
        transposition_table[hash] = temp;
        if(v>=beta) return v;
        alpha=max(alpha, v);
    }
    //sort the childrens of this state according to evaluated value
    sort(state->childStates.begin(), state->childStates.end(), compareMaxStates);
    cout << "After Sortng States in Max Value at depth: " <<d <<" "<<state->childStates[0]->EvaluatedValue << endl;
    // cout << "done" << endl;
    ///////////////////////////////////////////////////////////////
    return v;
}

int MIN_VALUE(GameMatrix *state, int alpha, int beta, int d)
{
    if(d==0)
    {
        //return EVAL(s)
        return evaluation(state->MeNoOfTownhalls, state->OpNoOfTownhalls, state->MeNoOfSoldiers, state->OpNoOfSoldiers);
    }
    int v = INT32_MAX;
    int temp = 0;
    // //generate children
    if(state->childStates.size()==0)
    {
        state->GenerateStates(true);
    }
    ///////////////////
    unsigned long long int hash;
    for (int i = 0; i < state->childStates.size(); i++)
    {
        hash = computeHash(state->childStates.at(i)->SimMatrix);
        if(transposition_table.find(hash)==transposition_table.end())
        {
            temp = MAX_VALUE((state->childStates.at(i)), alpha, beta, d-1);
        }
        else
        {
            temp = transposition_table.at(hash);
        }
        v = min(v, temp);
        state->childStates.at(i)->EvaluatedValue = temp;
        transposition_table[hash] = temp;
        if(v<=alpha) return v;
        beta=min(beta, v);
    }
    //sort the childrens of this state according to evaluated value
    sort(state->childStates.begin(), state->childStates.end(), compareMinStates);
    ///////////////////////////////////////////////////////////////
    return v;
}

string ALPHA_BETA_SEARCH(GameMatrix *state, int d)
{
    // pair<int, vector<GameMatrix>> temppair;
    int value = 0;
    value = MAX_VALUE(state, INT32_MIN, INT32_MAX, d);
    state->EvaluatedValue = value;
    //binary search to find the value;
    cout << "Before returning alpha beta" << state->childStates[0]->EvaluatedValue << endl;
    return state->childStates[0]->Moves.at(state->childStates[0]->Moves.size()-1);
}

#endif