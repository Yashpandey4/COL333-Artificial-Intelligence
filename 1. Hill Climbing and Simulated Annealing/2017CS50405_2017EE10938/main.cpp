/**
* @author : DHANANJAY KAJLA
* date : 19/08/2019
*/

/******************************Libraries Used*******************/

#include<iostream>
#include<iomanip> // for time function
#include<time.h>  // for time manipulation
#include<vector>  // for associative arrays
#include<sstream> // to convert double to string
#include<map>     // to convert char to int to get mc fast
#include "io.h"
#include "algo.h"
#include "log.h"
using namespace std;

/***************************************************************/

/*****************************************************
              Main Function
*****************************************************/
int main(int argc, char* argv[])
{

  /**************************Time Counter**************************************/

  time_t start,end;                                       //Counts execution time
  time(&start);                                           //Starts execution time

  /****************************************************************************/


  /**************************Error Log*****************************************/

  if(argc != 3 )                                          //Handles incorrect number of inputs
  {
    log("error.log","Incorrect number of Arguments");
    return 0;
  }

  /****************************************************************************/


  /**************************Variable Declaration******************************/

  double timer = 0.0;                                     //Allowed time
  int v_size = 0;                                         //Vocabulary size
  std::vector<char> v;                                    //Vocabulary
  int K = 0;                                              //Number of strings
  std::vector<std::string> X;                             //Set of initial strings
  std::vector<std::string> S;                             //Set of final strings
  double cc = 0;                                          //Conversion cost constant
  std::vector<std::vector<double> > MC;                   //Mactching cost matrix

  /****************************************************************************/

  /**************************Extra Variable************************************/

  std::map<char, int> mp;

  /****************************************************************************/

  /**************************Input*********************************************/

  input(argv[1] , timer, v_size, v, K, X, cc, MC, mp);         //Takes input
  output_in_check("out.txt", timer, v_size, v, K, X, cc, MC, mp);

  /****************************************************************************/


  /**************************Algorithm*****************************************/

  //algo_that_doesnt_work(X,S);                             //Temporary format checking algo
  //dp(start, timer, v_size, v, K, X, S, cc, MC, mp);
  tester1(MC,cc,mp,X);

  /****************************************************************************/


  /**************************Output********************************************/

  //output_in_check("out.txt", timer, v_size, v, K, X, cc, MC, mp);
  output(argv[2], S);                                     //Gives output


  /****************************************************************************/


  /**************************Timer Log*****************************************/

  time(&end);                                             //Marks the end time
  double d = end - start;                                 //d is the total time taken by the program
  std::ostringstream s;                                   //converts double to string
  s << d;
  log("time.log", s.str());                               //logs the time taken by the program to run

  /****************************************************************************/

  return 0;
}
