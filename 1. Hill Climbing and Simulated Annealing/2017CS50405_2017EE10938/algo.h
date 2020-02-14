/******************************Libraries Used**********************************/

#include<iostream>
#include<vector>  // for associative arrays
#include<map>     // to convert char to int to get mc fast
using namespace std;

/******************************************************************************/

/****************************Header Guard**************************************/

#ifndef ALGO_H
#define ALGO_H

/******************************************************************************/

/****************************State Space***************************************/

struct State{
  std::vector<int> coordinate;
  double best_cost;
  std::vector<std::string> solution;
  State();
  State(std::vector<int>);
  State(std::vector<int> v, int K);
  State(std::vector<int> v, double best_cos, std::vector<std::string> soln);
  void Print_State();
  double Step_Cost(State z, std::vector<std::vector<double> > MC, double cc, std::map<char,int>,std::vector<std::string>);
  bool is_Goal(int K, std::vector<std::string> X);
  bool is_Valid(int K, std::vector<std::string> X);

};

/******************************************************************************/


/******************************Function Prototypes*****************************/

void algo_that_doesnt_work(std::vector<std::string> &X, std::vector<std::string> &S);
void dp(time_t& start, double& timer, int& v_size, std::vector<char>& v, int& K, std::vector<std::string>& X, std::vector<std::string>& S, double& cc, std::vector<std::vector<double> >& MC, std::map<char, int> mp);
void dp_solution(State z, time_t& start, double& timer, int& v_size, std::vector<char>& v, int& K, std::vector<std::string>& X, std::vector<std::string>& S, double& cc, std::vector<std::vector<double> >& MC,std::map<char, int> mp);
void tester1(std::vector<std::vector<double> > MC, double cc, std::map<char,int> mp, std::vector<std::string> X);

/******************************************************************************/
#endif
