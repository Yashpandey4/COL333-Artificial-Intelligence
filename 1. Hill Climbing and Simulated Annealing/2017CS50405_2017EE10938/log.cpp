/******************************Libraries Used*******************/

#include<iostream>
#include<fstream> // for file streaming
#include "error.h"
using namespace std;

/***************************************************************/

/*****************************************************
  Log keeper
*****************************************************/

void log(std::string filename, std::string s)
{
  ofstream logfile(filename.c_str());
  logfile << s;
  logfile.close();
}
