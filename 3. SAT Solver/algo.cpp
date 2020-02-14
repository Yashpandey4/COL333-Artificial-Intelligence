#include <iostream>
#include <fstream>
#include <algorithm> 
#include <string.h>
#include <vector>
#include <map>

using namespace std;

 int** var_ass;

typedef struct node
{
  int indegree = 0;
  int outdegree = 0;
  vector <int> adjList;
} node;

node* GL;
node* GS;

ifstream infile;
int v1,v2;
bool** G1;
bool** G2;

int getDim()
{
  string line;
  int v=0;
  while (getline(infile,line))
  {
    if (line=="0 0")
      break;

    int a, b;
    string c="", d= "";
    bool flag = false;

    for (int i=0; i<line.length(); i++)
    {
      if (line[i]==' ')
        flag = true;
      else
      {
        if (flag==false)
          c+=line[i];
        else
          d+=line[i];
      }
    }

    a= stoi(c);
    b= stoi(d);

    int e= max(a,b);
    if (e>v)
      v=e;
  }
  return v;
}

void getEdges(bool** G, node* dList)
{
  string line;
  while (getline(infile,line))
  {
    if (line=="0 0")
      break;

    int a, b;
    string c="", d= "";
    bool flag = false;

    for (int i=0; i<line.length(); i++)
    {
      if (line[i]==' ')
        flag = true;
      else
      {
        if (flag==false)
          c+=line[i];
        else
          d+=line[i];
      }
    }

    a= stoi(c);
    b= stoi(d);

    dList[a].outdegree++;
    dList[b].indegree++;
    dList[a].adjList.push_back(b);
    G[a][b]= true;
  }
}

bool** createArray(int v)
{
  bool** G= new bool*[v+1];
  for(int i=0; i<v+1; i++)
    G[i]= new bool[v+1];
  for(int i=0; i< v+1; i++)
  {
    for (int j=0; j< v+1; j++)
    {
      G[i][j]=false;
    }
  }
  return G;
}

// var_ass is a global variable with same dim as valid
int** solve()
{
    int** valid;
    valid=new int[v2+1][v1+1]
    var_ass = new int[v2+1][v1+1];
    int p=0;  int q=0;
    for(int i=0; i<v2+1;i++)
    {
        // valid[p] = new vector<int>[v1+1]; 
        q=0;
        // var_ass[p] = new vector<int>[v1+1];
        for(int j=0;j<v1+1;j++)
        {
            if((GS[i].indegree && GL[j].indegree) && (GS[i].outdegree <= GS[j].outdegree))
                valid[p][q] = 1;
            else
                valid[p][q] = 0;
            q++;
        }
        p++;
    }
    return valid;
}

void assign(int i,int j,int val)  // var has same dec as valid
{
    var_ass[i][j] = val;
}

void gen_output(int** valid, string f){
    valid = new int[v2+1][v1+1];
    int n1 = v1+1; //GL.size();
    int m1 = v2+1; //GS.size();
	int one=1;
	int zero=0;
	// int num_vars = v1*v2;
	int num_clauses = m1*one + (m1*n1*one*(n1-1)/2) + (m1*n1*one*(m1-1)/2) + (m1*n1*((m1*n1)-1)/2) + 3*one + zero;
    // int num_clauses = v2*one +zero + v2*one*(v1*(v1+zero-1))/2 + v1*(v2*(v2 - 1)/2) + (v2*(v2-1)*v1*(v1-1))
	int jargon1=0;
	int jargon2=0;
    int counter=1;
    int countArray[n1] = {0};
	int countArrayRowwise[m1] = {0};
    ofstream out(f, ios::out);
	ofstream out1("temp",ios::app);
	for (int i=0; i<m1; i++)
	{
		for (int j=0; j<n1; j++)
		{
			if (valid[i][j] == 0){
				
				assign(i,j,zero);
			}
			else{
				assign(i,j,counter*one+zero);
				counter++;
				jargon1++;
				countArray[j] ++;
				jargon2++;
				countArrayRowwise[i] ++;
			}
		}
	}

    int num_vars = counter-one+zero;
    out << "p cnf " << num_vars << " " << num_clauses << "\n";
    out1 << num_vars << "\n";
    for (int i=0; i<m1; i++)
    {
        for (int j=0; j<n1; j++)
        {
            if (valid[i][j] == 1)
            {
                out << var_ass[i][j] << " ";
                out1 << var_ass[i][j] << " ";
            }
            else
            {
                out1 << "0";
            }
        }
        out << "0\n";
        out1 << "0";
    }

    int row = 0;
    int column = 0;
    int row1 = 0;
    int column1 = 0;

    for (int i=0; i<m1;i++) //it
    {
        column = 0 + zero;
        for(int j=0; j<n1; j++)  //it1
        {
            if (valid[row][column] == 0){
                column ++;
                continue;
            }
            row1 = 0+zero;
            column1 = 0+zero;
            for (int k=0; k<m1;k++) //it2
            {
                if (row == row1)
                {
                    row1++;
                    continue;
                }
                column1 = 0+zero;
                for (int l=0; l<n1; l++) //it3
                {
                    if ((valid[row1][column1] == 0) || (column == column1)){
                        column1++;
                        continue;
                    }
                    auto ita = find (GS[i].adjList.begin(), GS[i].adjList.end(), k);
                    auto itb = find (GL[j].adjList.begin(), GL[j].adjList.end(), l);
                    if (((ita != GS[i].adjList.end()) && (itb == GL[j].adjList.end())) || ((ita == GS[i].adjList.end()) && (itb != GL[j].adjList.end()))){
                        out << "-" << var_ass[row][column] << " -" << var_ass[row1][column1] << " 0\n";
                    }
                    column1++;
                }
                row1++;
            }
            column ++; 
        }
        row++;
    }

    for (int i = 0; i < m1; i++)
    {
        if (countArrayRowwise[i] <= 1)
        {
            continue;
        }
        else
        {
            for (int j=0; j<n1; j++)
            {
                if (valid[i][j] == 0)
                {
                    continue;
                }
                else
                {
                    for (int k = j+1; k < n1; k++)
                    {
                        if (valid[i][k] == 1){
                            out << "-" << var_ass[i][j] << " -" << var_ass[i][k] << " 0\n";
                        }
                    }
                }
            }
        }
    }


    for (int j=0; j<n1; j++)
    {
        if (countArray[j] <= 1)
        {
            continue;
        }
        else
        {
            for (int i = 0; i < m1; i++)
            {
                if (valid[i][j] == 0)
                {
                    continue;
                }
                else
                {
                    for (int k = i+1; k < m1; k++)
                    {
                        if (valid[k][j] == 0);
                        else
                        {
                            out << "-" << var_ass[i][j] << " -" << var_ass[k][j] << " 0\n";
                        }
                    }
                }      
            }
        }
    }
    out.close();
    out1.close();
}

int main(int argc, char * argv[])
{
    char* f = argv[1];
	char* outfile = argv[2];
    string str(f);
    infile.open(str);
    string line;

    v1=getDim();
    v2=getDim();

    G1= createArray(v1);
    G2= createArray(v2);

    GL = new node[v1+1];
    GS = new node[v2+1];

    infile.close();
    infile.clear();
    infile.open(str);

    getEdges(G1, GL);
    getEdges(G2, GS);

    int** table; 
    table = solve();

    gen_output(table,str);

    return 0;

}
    