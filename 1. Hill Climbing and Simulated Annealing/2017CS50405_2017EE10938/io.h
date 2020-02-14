/******************************Header Guard*******************/
#ifndef IO_H
#define IO_H
/*************************************************************/

/******************************Libraries Used*******************/

#include<iostream>
#include<vector>  // for associative arrays
#include<map>     // to convert char to int to get mc fast
using namespace std;

/***************************************************************/

/******************************Function Prototypes*****************************/

void input(std::string infile ,double &timer, int &v_size, std::vector<char> &v, int &K, std::vector<std::string> &X, double &cc, std::vector<std::vector<double> > &MC, std::map<char, int> &mp);
void output_in_check(std::string outfile, double &timer, int &v_size, std::vector<char> &v, int &K, std::vector<std::string> &X, double &cc, std::vector<std::vector<double> > &MC,std::map<char, int> &mp); //temporary input checker function
void output(std::string outfile, std::vector<std::string> &S);//gives output

/******************************************************************************/
#endif
