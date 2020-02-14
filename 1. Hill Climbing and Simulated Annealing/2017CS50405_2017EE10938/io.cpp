/******************************Libraries Used*******************/

#include<iostream>
#include<fstream> // for file streaming
#include<vector>  // for associative arrays
#include<map>     // to convert char to int to get mc fast
#include "io.h"
using namespace std;

/***************************************************************/

/*****************************************************
              Input Function
*****************************************************/
void input(std::string infile ,double &timer, int &v_size, std::vector<char> &v, int &K, std::vector<std::string> &X, double &cc, std::vector<std::vector<double> > &MC, std::map<char, int> &mp)
{
  if(infile.empty())
  {
    return;
  }
  ifstream inobj;                                       //declaring inputstreaming object
  inobj.open(infile.c_str());                                   //opening the given file
  inobj >> timer;                                       //input timer
  timer *= 60;                                          //converts minutes into seconds
  inobj >> v_size;                                      //input size of Vocabulary
  v.resize(v_size);                                     //resize the Vocabulary vector to the given size
  std::string str;                                      //temporary variable used to get Vocabulary
  for (size_t i = 0; i < v.size(); i++)                 //getting Vocabulary
  {
    inobj >> str;                                       //Each string has the required character and a ','
    v[i] = str[0];                                      //Removing the ','
  }
  for (size_t i = 0; i < v.size(); i++)
  {
    mp[v[i]] = i;
  }
  mp['-'] = v.size();
  inobj >> K;                                           //input the size of the initial strings
  X.resize(K);                                          //resizing the initial string vector to the given number of strings
  for (size_t i = 0; i < X.size(); i++)                 //getting the initial strings
  {
    inobj >> X[i];                                      //taking in the strings
  }
  inobj >> cc;                                          //input the constant cost
  for (size_t i = 0; i <= (size_t)v_size; i++)          //taking in the MC matrix
  {
    std::vector<double> temp(v_size+1);                 //temporary vector to get input
    for(size_t j = 0; j <= (size_t)v_size; j++)         //getting the cost vector for ith character
    {
      double x;
      inobj >> x;
      temp[j] = x;                                //getting the individual cost
    }
    MC.push_back(temp);                                 //Adding a new row to MC
  }
  inobj >> str;                                         //Taking the last input '#'
  inobj.close();
  return;
}

/******************************************************************
              Output Function (Temporary Checker Function)
******************************************************************/


void output_in_check(std::string outfile, double &timer, int &v_size, std::vector<char> &v, int &K, std::vector<std::string> &X, double &cc, std::vector<std::vector<double> > &MC,std::map<char, int> &mp)
{
  ofstream outobj;
  outobj.open(outfile.c_str());
  if(outfile.empty())
  {
    return;
  }
  outobj << timer << std::endl;
  outobj << v_size << std::endl;
  for (size_t i = 0; i < v.size(); i++)
  {
    outobj << v[i]  << " ";
  }
  outobj  << std::endl <<  K << std::endl;
  for (size_t i = 0; i < X.size(); i++)
  {
    outobj << X[i] << std::endl;
  }
  outobj  << cc  << std::endl;
  for (size_t i = 0; i <= (size_t)v_size; i++)
  {
    for(size_t j = 0; j <= (size_t)v_size; j++)
    {
      outobj << MC[i][j] << ' ';
    }
    outobj << std::endl;
  }
  outobj << "#";
  outobj.close();
  return;
}

/******************************************************************
              Output Function
******************************************************************/
void output(std::string outfile, std::vector<std::string> &S)
{
  ofstream outobj;
  outobj.open(outfile.c_str());
  if(outfile.empty())
  {
    return;
  }
  //std::cout << X.size() << std::endl;
  for (size_t i = 0; i < S.size(); i++)
  {
    outobj << S[i] << std::endl;
  }
}
