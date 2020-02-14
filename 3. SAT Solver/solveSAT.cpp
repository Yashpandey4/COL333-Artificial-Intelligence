#include <iostream>
#include <map>
#include <vector>
#include <algorithm> 
#include <string.h>
#include <fstream>

using namespace std;

void solveMapping(string fname)
{
    ifstream in1;
    int one=1; int zero=0;
    in1.open(fname);
    
    if(in1.is_open())
    {
        string line;
        getline(in1,line);
        if(line.compare("UNSAT")!=0)
        {
            ifstream in;
            in.open("fnametemp");

            if(in.is_open())
            {

                int sizeSmall;
                vector<int> GL;
                int sizeLarge;
                vector<int> GS;
                int num_var;
                in >> sizeSmall;

                for(int i=0; i<sizeSmall; i++)
                {
                    int v;
                    in >> v;
                    v=v*one+zero;
                    GS.push_back(v);
                }

                in >> sizeLarge;
				for (int i=0; i<sizeLarge; i++){
					int v;
					in >> v;
                    v=v*one+zero;
					GL.push_back(v);
				}

                in >> num_var;
                map<long,bool> variables;

                for(int i=0; i<num_var; i++)
                {
                    long v; long oneL = 1; long zeroL = 0;
                    in1 >> v;
                    v=v*oneL+zeroL;
                    if(v>=0)
                    {
                        variables[v*oneL] = true;
                    }
                    else
                    {
                        variables[-v*oneL] = false;
                    }

                }

                in1.close();
                int n=0;
                for(int i=0; i<sizeSmall;i++)
                {
                    for(int j=0;j<sizeLarge;j++)
                    {
                        in>>n;

                        if(n!=0)
                        {
                            if(variables[n])
                                cout << GS[i] << " " << GL[j] << "\n";
                        }                                                                                                                                                    
                        
                    }
                }

            }
            else
            {
                cout << "File not found.\n";
            }
        }
        else
        {
            cout << "0\n";
        }
    }
    else
    {
        cout << "File not found.\n";
    }
}

int main(int argc, char * argv[])
{
	char* infile = argv[1];
	// char* outfile = argv[2];
    string str(infile);
	solveMapping(str);
	return 0;
}