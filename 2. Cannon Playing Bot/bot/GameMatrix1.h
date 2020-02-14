#ifndef GAMEMATRIX_H
#define GAMEMATRIX_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <pthread.h>
using namespace std;
struct Soldier
{
	bool opponent = false;
	bool townhall = false;
	bool free = true;
	int x = 0;
	int y = 0;				//coordinates of the soldier
	bool cannonEnd = false; //this tells if soldier is end point of a cannon
	vector<int> direction;  //it takes values - 0,45,90,135,180,225,270,315. This is only seen when cannonEnd is tree. Also tells no of no of cannons.
	vector<string> CannonKills;
	vector<string> KillMoves;
	vector<string> Moves;
	vector<string> CannonMoves;
	vector<string> Bomb;
};

struct PlayerState
{
	int NoOfTownhall;
	int NoOfSoldier;
};

vector<vector<int>> GetSimMatrix(vector<vector<Soldier>> s)
{
	vector<vector<int>> mat(s.size(),vector<int>(s[0].size(),0));
	for (int i = 0; i < s.size(); i++)
	{
		for (int j = 0; j < s[0].size(); j++)
		{
			if (s[i][j].townhall == true)
			{
				if(s[i][j].opponent)
				{
					mat[i][j] = -7;
				}
				else
				{
					mat[i][j] = 7;
				}
			}
			else if (s[i][j].free == false)
			{
				if(s[i][j].opponent)
				{
					mat[i][j] = -3;
				}
				else
				{
					mat[i][j] = 3;
				}
			}
		}
	}
	return mat;
}
void printMoves(vector<string> s)
{
	for (int i = 0; i < s.size(); i++)
	{
		cout << s[i] << endl;
	}
}
void printMatrix(vector<vector<int>> s)
{
	for (int i = 0; i < s.size(); i++)
	{
		for (int j = 0; j < s[0].size(); j++)
		{
			if(s[i][j]<0)
			{
				cout << s[i][j] << " ";	
			}
			else
			{
				cout << " " <<s[i][j] << " ";	
			}
		}
		cout << endl;
	}
}
void printGameMatrix(vector<vector<Soldier>> s)
{
	for (int i = 0; i < s.size(); i++)
	{
		for (int j = 0; j < s[0].size(); j++)
		{
			if (s[i][j].townhall == true)
			{
				cout << "T"
					 << " ";
			}
			else if (s[i][j].free == false)
			{
				cout << "S"
					 << " ";
			}
			else
			{
				cout << "  ";
			}
		}
		cout << endl;
	}
}

class GameMatrix
{
public:
	/* data */
	int n, m; //n-length and m-width of the board
	int NoOfSoldiers;
	int OpNoOfSoldiers = 0;
	int MeNoOfSoldiers = 0;
    int OpNoOfTownhalls = 0;
    int MeNoOfTownhalls = 0;
	vector<vector<Soldier>> SoldierMatrix; //this contains
	vector<string> Moves;//list moves required to come up to state state
    vector<string> FutureMoves;
    vector<GameMatrix*> childStates;
	vector<vector<int>> SimMatrix;
	unordered_map<int, Soldier> OpSoldiers;
	unordered_map<int, Soldier> MeSoldiers;
	int EvaluatedValue = 0;
	GameMatrix();
	GameMatrix(int n, int m, int NoOfSoldiers);
	int ListMoves(int x, int y);
	int ExecuteMoves(string x);
	vector<string> GenerateMoves(unordered_map<int,Soldier> Soldiers);
	void GenerateStates(bool opponent);
	~GameMatrix();
};
GameMatrix::GameMatrix()
{
}
///////////////////////////////////////////////
struct coordinate
{
	int x;
	int y;
	GameMatrix *object;
};
#define NUM_THREADS 4
pthread_mutex_t locks;
void *MainThread(void *p_param)
{
	pthread_mutex_lock(&locks);
	coordinate *my_param;
	my_param = (coordinate *)p_param;
	GameMatrix *object = (my_param->object);
	// printGameMatrix(object->SoldierMatrix);
	object->ListMoves(my_param->x, my_param->y);
	pthread_mutex_unlock(&locks);
	pthread_exit(NULL);
}

///////////////////////////////////////////////
GameMatrix::GameMatrix(int n, int m, int NoOfSoldiers)
{
	this->n = n; //rows
	this->m = m; //columns
	this->NoOfSoldiers = NoOfSoldiers;
	this->MeNoOfSoldiers = NoOfSoldiers;
	this->OpNoOfSoldiers = NoOfSoldiers;
	this->MeNoOfTownhalls = 4;
	this->OpNoOfTownhalls = 4;
	//Make the matrix
	int SoldsInRow = NoOfSoldiers / (m / 2);
	Soldier man;
	vector<vector<Soldier>> rowSold(n, vector<Soldier>(m, man));
	vector<vector<int>> row(n, vector<int>(m, 0));

	//fill my opponent
	for (int i = 0; i < SoldsInRow; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (i == 0 && j % 2 == 0)
			{
				row[i][j] = -7;
				rowSold[i][j].townhall = true;
				rowSold[i][j].free = false;
				rowSold[i][j].opponent = true;
				rowSold[i][j].x = j;
				rowSold[i][j].y = i;
			}
			if (j % 2 == 1)
			{
				rowSold[i][j].free = false;
				row[i][j] = -3;
				rowSold[i][j].opponent = true;
				rowSold[i][j].x = j;
				rowSold[i][j].y = i;
			}
		}
	}
	//fill me
	for (int i = n - SoldsInRow; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (i == n - 1 && j % 2 == 1)
			{
				row[i][j] = 7;
				rowSold[i][j].townhall = true;
				rowSold[i][j].free = false;
				rowSold[i][j].x = j;
				rowSold[i][j].y = i;
			}
			if (j % 2 == 0)
			{
				rowSold[i][j].free = false;
				row[i][j] = 3;
				rowSold[i][j].x = j;
				rowSold[i][j].y = i;
			}
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			rowSold[i][j].x = j;
			rowSold[i][j].y = i;
		}
	}

	this->SoldierMatrix = rowSold;
	this->SimMatrix = row;

	// give moves to all the soldiers.....
	// using multithreading in this...
	// multithreading pointer functions are above
	pthread_t threads[SoldsInRow];
	pthread_attr_t attr;
	void *status;
	int rc;
	int r1;
	int r2;
	// for (int j = 0; j < this->m; j++)
	// {
	//     cout << "col  " << j << endl;
	//     if (j % 2 == 0)
	//     {
	//         r1 = this->n - SoldsInRow;
	//         r2 = this->n - 1;
	//         cout << r1 << " " << r2 << endl;
	//     }
	//     else
	//     {
	//         r1 = 0;
	//         r2 = SoldsInRow - 1;
	//         cout << r1 << " " << r2 << endl;
	//     }
	//     for (int i = r1; i <= r2; i++)
	//     {
	//         cout << j << " " << i << endl;
	//         this->ListMoves(j, i);
	//     }
	// }
	if (pthread_mutex_init(&locks, NULL) != 0)
	{
		printf("\n mutex init has failed\n");
	}
	coordinate cord[8][8];
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (int j = 0; j < this->m; j++)
	{
		if (j % 2 == 0)
		{
			r1 = this->n - SoldsInRow;
			r2 = this->n - 1;
		}
		else
		{
			r1 = 0;
			r2 = SoldsInRow - 1;
		}
		// Initialize and set thread joinable
		for (int i = r1; i <= r2; i++)
		{
			cord[i][j].x = j;
			cord[i][j].y = i;
			cord[i][j].object = this;
			// cout << "main() : creating thread, " << i << endl;
			rc = pthread_create(&threads[i - r1], &attr, MainThread, (void *)&cord[i][j]);
			if (rc)
			{
				cout << "Error:unable to create thread," << rc << endl;
				exit(-1);
			}
		}
		// free attribute and wait for the other threads
		for (int i = 0; i < SoldsInRow; i++)
		{
			rc = pthread_join(threads[i], &status);
			if (rc)
			{
				cout << "Error:unable to join," << rc << endl;
				exit(-1);
			}
			// cout << "Main: completed thread id :" << i;
			// cout << "  exiting with status :" << status << endl;
		}
	}
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&locks);

	for (int i = 0; i < SoldsInRow; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (j % 2 == 1)
			{
				OpSoldiers[i * 10 + j] = this->SoldierMatrix[i][j];
			}
		}
	}
	//fill me
	for (int i = n - SoldsInRow; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (j % 2 == 0)
			{
				MeSoldiers[i * 10 + j] = this->SoldierMatrix[i][j];
			}
		}
	}
}
int GameMatrix::ListMoves(int x, int y)
{
	/* 
    y: y-coordinate
    x: x-coordinate
    origin is at the left upper corner
    */
	if (x < 0 || x >= this->m || y < 0 || y >= this->n)
	{
		// cout << "Wrong Coordinate " << x << " " << y << endl;
		return -1;
	}
	if (this->SoldierMatrix[y][x].free == true)
	{
		// cout << "No Soldier There " << x << " " << y << endl;
		return -1;
	}
	if(this->SoldierMatrix[y][x].townhall==true)
	{
		return -1;
	}
	this->SoldierMatrix[y][x].Moves.clear();
	this->SoldierMatrix[y][x].KillMoves.clear();
	this->SoldierMatrix[y][x].CannonMoves.clear();
	this->SoldierMatrix[y][x].CannonKills.clear();
	this->SoldierMatrix[y][x].Bomb.clear();
	this->SoldierMatrix[y][x].direction.clear();
	this->SoldierMatrix[y][x].cannonEnd = false;

	bool opponent = this->SoldierMatrix[y][x].opponent;
	// cout << "ForwardMoves" << endl;
	//Forward moves
	int ycord1 = (opponent) ? y + 1 : y - 1;
	bool adjacent = false; //to check if it is adjacent to an opponent
	if ((0 <= ycord1 && ycord1< this->n))
	{
		for (int i = x - 1; i <= x + 1; i++)
		{
			if (!(0 <= i && i < this->m))
			{
				continue;
			}
			if (this->SoldierMatrix[ycord1][i].townhall == true) //if there is townhall
			{
				SoldierMatrix[y][x].KillMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(i) + to_string(ycord1));
			}
			else if (this->SoldierMatrix[ycord1][i].opponent == !opponent && this->SoldierMatrix[ycord1][i].free == false) //if there is a opponent
			{
				SoldierMatrix[y][x].KillMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(i) + to_string(ycord1));
				adjacent = true;
			}
			else if (this->SoldierMatrix[ycord1][i].free == true)
			{
				SoldierMatrix[y][x].Moves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(i) + to_string(ycord1));
			}
		}
	}
	// cout << "SideMoves" << e ndl;
	//Sideways
	if (0 <= x - 1)
	{
		if(this->SoldierMatrix[y][x - 1].opponent == !opponent && this->SoldierMatrix[y][x - 1].free == false)
		{
			SoldierMatrix[y][x].KillMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x - 1) + to_string(y));
			adjacent = true;
		}
	}
	if (x + 1 < this->m)
	{
		if(this->SoldierMatrix[y][x + 1].opponent == !opponent && this->SoldierMatrix[y][x + 1].free == false)
		{
			SoldierMatrix[y][x].KillMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x + 1) + to_string(y));
			adjacent = true;
		}
	}
	// cout << "BackMoves" << endl;
	//Backward moves
	int ycord2 = (opponent) ? y - 2 : y + 2;
	if ((0 <= ycord2 && ycord2< this->n))
	{
		if (adjacent == true)
		{
			for (int i = x - 2; i <= x + 2; i = i + 2)
			{
				if (!(0 <= i && i< this->m))
				{
					continue;
				}
				if (this->SoldierMatrix[ycord2][i].free == true)
				{
					SoldierMatrix[y][x].Moves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(i) + to_string(ycord2));
				}
			}
		}
	}
	// cout << "CannonMoves" << endl;
	//cannon detection//
	////////////////////
	bool up, upright, upleft = false;		   //these bool to check if cannon in that direction possible
	bool below, belowright, belowleft = false; //these bool to check if cannon in that direction possible
	bool left, right = false;
	//check up
	// cout << "checkup" << endl;
	if (y - 2 >= 0)
	{
		if (this->SoldierMatrix[y - 1][x].free == false)
		{
			if (this->SoldierMatrix[y - 1][x].opponent == opponent && this->SoldierMatrix[y - 1][x].townhall == false)
			{
				if (this->SoldierMatrix[y - 2][x].opponent == opponent && this->SoldierMatrix[y - 2][x].townhall == false && this->SoldierMatrix[y - 2][x].free == false)
				{
					up = true;
					this->SoldierMatrix[y][x].cannonEnd = true;
					this->SoldierMatrix[y][x].direction.push_back(270);
					//moves and kills
					//move
					if (y - 3 >= 0)
					{
						if (this->SoldierMatrix[y - 3][x].free == true)
						{
							if (!opponent)
							{
								SoldierMatrix[y][x].CannonMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x) + to_string(y - 3));
							}
							else
							{
								SoldierMatrix[y][x].CannonMoves.insert(SoldierMatrix[y][x].CannonMoves.begin(), "S" + to_string(x) + to_string(y) + "M" + to_string(x) + to_string(y - 3));
							}
						}
					}
					//2 kills
					if (y + 1 < this->n)
					{
						if (this->SoldierMatrix[y + 1][x].free == true)
						{
							for (int i = 2; i <= 3; i++)
							{
								if (y + i < this->n)
								{
									if (this->SoldierMatrix[y + i][x].opponent == (!opponent) && this->SoldierMatrix[y + i][x].free == false)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y + i));
										}
										else
										{
											SoldierMatrix[y][x].CannonKills.insert(SoldierMatrix[y][x].CannonKills.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y + i));
										}
									}
									else if (this->SoldierMatrix[y + i][x].free == true)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].Bomb.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y + i));
										}
										else
										{
											SoldierMatrix[y][x].Bomb.insert(SoldierMatrix[y][x].Bomb.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y + i));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//check upleft
	// cout << "checkupleft" << endl;
	if (y - 2 >= 0 && x - 2 >= 0)
	{
		if (this->SoldierMatrix[y - 1][x - 1].free == false)
		{
			if (this->SoldierMatrix[y - 1][x - 1].opponent == opponent && this->SoldierMatrix[y - 1][x - 1].townhall == false)
			{
				if (this->SoldierMatrix[y - 2][x - 2].opponent == opponent && this->SoldierMatrix[y - 2][x - 2].townhall == false && this->SoldierMatrix[y - 2][x - 2].free == false)
				{
					upleft = true;
					this->SoldierMatrix[y][x].cannonEnd = true;
					this->SoldierMatrix[y][x].direction.push_back(315);
					//moves and kills
					//move
					if (y - 3 >= 0 && x - 3 >= 0)
					{
						if (this->SoldierMatrix[y - 3][x - 3].free == true)
						{
							if (!opponent)
							{
								SoldierMatrix[y][x].CannonMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x - 3) + to_string(y - 3));
							}
							else
							{
								SoldierMatrix[y][x].CannonMoves.insert(SoldierMatrix[y][x].CannonMoves.begin(), "S" + to_string(x) + to_string(y) + "M" + to_string(x - 3) + to_string(y - 3));
							}
						}
					}
					//2 kills
					if (y + 1 < this->n && x + 1 < this->m)
					{
						if (this->SoldierMatrix[y + 1][x + 1].free == true)
						{
							for (int i = 2; i <= 3; i++)
							{
								if (y + i < this->n && x + i < this->m)
								{
									if (this->SoldierMatrix[y + i][x + i].opponent == (!opponent) && this->SoldierMatrix[y + i][x + i].free == false)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y + i));
										}
										else
										{
											SoldierMatrix[y][x].CannonKills.insert(SoldierMatrix[y][x].CannonKills.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y + i));
										}
									}
									else if (this->SoldierMatrix[y + i][x + i].free == true)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].Bomb.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y + i));
										}
										else
										{
											SoldierMatrix[y][x].Bomb.insert(SoldierMatrix[y][x].Bomb.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y + i));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//check upright
	// cout << "checkupright" << endl;
	if (y - 2 >= 0 && x + 2 < this->m)
	{
		if (this->SoldierMatrix[y - 1][x + 1].free == false)
		{
			if (this->SoldierMatrix[y - 1][x + 1].opponent == opponent && this->SoldierMatrix[y - 1][x + 1].townhall == false)
			{
				if (this->SoldierMatrix[y - 2][x + 2].opponent == opponent && this->SoldierMatrix[y - 2][x + 2].townhall == false && this->SoldierMatrix[y - 2][x + 2].free == false)
				{
					upright = true;
					this->SoldierMatrix[y][x].cannonEnd = true;
					this->SoldierMatrix[y][x].direction.push_back(225);
					//moves and kills
					//move
					if (y - 3 >= 0 && x + 3 < this->m)
					{
						if (this->SoldierMatrix[y - 3][x + 3].free == true)
						{
							if (!opponent)
							{
								SoldierMatrix[y][x].CannonMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x + 3) + to_string(y - 3));
							}
							else
							{
								SoldierMatrix[y][x].CannonMoves.insert(SoldierMatrix[y][x].CannonMoves.begin(), "S" + to_string(x) + to_string(y) + "M" + to_string(x + 3) + to_string(y - 3));
							}
						}
					}
					//2 kills
					if (y + 1 < this->n && x - 1 >= 0)
					{
						if (this->SoldierMatrix[y + 1][x - 1].free == true)
						{
							for (int i = 2; i <= 3; i++)
							{
								if (y + i < this->n && x - i >= 0)
								{
									if (this->SoldierMatrix[y + i][x - i].opponent == (!opponent) && this->SoldierMatrix[y + i][x - i].free == false)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y + i));
										}
										else
										{
											SoldierMatrix[y][x].CannonKills.insert(SoldierMatrix[y][x].CannonKills.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y + i));
										}
									}
									else if (this->SoldierMatrix[y + i][x - i].free == true)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].Bomb.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y + i));
										}
										else
										{
											SoldierMatrix[y][x].Bomb.insert(SoldierMatrix[y][x].Bomb.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y + i));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//check below
	// cout << "checkbelow" << endl;
	if (y + 2 < this->n)
	{
		if (this->SoldierMatrix[y + 1][x].free == false)
		{
			if (this->SoldierMatrix[y + 1][x].opponent == opponent && this->SoldierMatrix[y + 1][x].townhall == false)
			{
				if (this->SoldierMatrix[y + 2][x].opponent == opponent && this->SoldierMatrix[y + 2][x].townhall == false && this->SoldierMatrix[y + 2][x].free == false)
				{
					below = true;
					this->SoldierMatrix[y][x].cannonEnd = true;
					this->SoldierMatrix[y][x].direction.push_back(90);
					//moves and kills
					//move
					if (y + 3 < this->n)
					{
						if (this->SoldierMatrix[y + 3][x].free == true)
						{
							if (!opponent)
							{
								SoldierMatrix[y][x].CannonMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x) + to_string(y + 3));
							}
							else
							{
								SoldierMatrix[y][x].CannonMoves.insert(SoldierMatrix[y][x].CannonMoves.begin(), "S" + to_string(x) + to_string(y) + "M" + to_string(x) + to_string(y + 3));
							}
						}
					}
					//2 kills
					if (y - 1 >= 0)
					{
						if (this->SoldierMatrix[y - 1][x].free == true)
						{
							for (int i = 2; i <= 3; i++)
							{
								if (y - i >= 0)
								{
									if (this->SoldierMatrix[y - i][x].opponent == (!opponent) && this->SoldierMatrix[y - i][x].free == false)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y - i));
										}
										else
										{
											SoldierMatrix[y][x].CannonKills.insert(SoldierMatrix[y][x].CannonKills.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y - i));
										}
									}
									else if (this->SoldierMatrix[y - i][x].free == true)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].Bomb.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y - i));
										}
										else
										{
											SoldierMatrix[y][x].Bomb.insert(SoldierMatrix[y][x].Bomb.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y - i));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//check belowright
	// cout << "checkbelowright" << endl;
	if (y + 2 < this->n && x + 2 < this->m)
	{
		if (this->SoldierMatrix[y + 1][x + 1].free == false)
		{
			if (this->SoldierMatrix[y + 1][x + 1].opponent == opponent && this->SoldierMatrix[y + 1][x + 1].townhall == false)
			{
				if (this->SoldierMatrix[y + 2][x + 2].opponent == opponent && this->SoldierMatrix[y + 2][x + 2].townhall == false && this->SoldierMatrix[y + 2][x + 2].free == false)
				{
					belowright = true;
					this->SoldierMatrix[y][x].cannonEnd = true;
					this->SoldierMatrix[y][x].direction.push_back(135);
					if (y + 3 < this->n && x + 3 < this->m)
					{
						if (this->SoldierMatrix[y + 3][x + 3].free == true)
						{
							if (!opponent)
							{
								SoldierMatrix[y][x].CannonMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x + 3) + to_string(y + 3));
							}
							else
							{
								SoldierMatrix[y][x].CannonMoves.insert(SoldierMatrix[y][x].CannonMoves.begin(), "S" + to_string(x) + to_string(y) + "M" + to_string(x + 3) + to_string(y + 3));
							}
						}
					}
					//2 kills
					if (x - 1 >= 0 && y - 1 >= 0)
					{
						if (this->SoldierMatrix[y - 1][x - 1].free == true)
						{
							for (int i = 2; i <= 3; i++)
							{
								if (y - i >= 0 && x - i >= 0)
								{
									if (this->SoldierMatrix[y - i][x - i].opponent == (!opponent) && this->SoldierMatrix[y - i][x - i].free == false)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y - i));
										}
										else
										{
											SoldierMatrix[y][x].CannonKills.insert(SoldierMatrix[y][x].CannonKills.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y - i));
										}
									}
									else if (this->SoldierMatrix[y - i][x - i].free == true)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].Bomb.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y - i));
										}
										else
										{
											SoldierMatrix[y][x].Bomb.insert(SoldierMatrix[y][x].Bomb.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y - i));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//check belowleft
	// cout << "checkbelowleft" << endl;
	if (y + 2 < this->n && x - 2 >= 0)
	{
		if (this->SoldierMatrix[y + 1][x - 1].free == false)
		{
			if (this->SoldierMatrix[y + 1][x - 1].opponent == opponent && this->SoldierMatrix[y + 1][x - 1].townhall == false)
			{
				if (this->SoldierMatrix[y + 2][x - 2].opponent == opponent && this->SoldierMatrix[y + 2][x - 2].townhall == false && this->SoldierMatrix[y + 2][x - 2].free == false)
				{
					belowleft = true;
					this->SoldierMatrix[y][x].cannonEnd = true;
					this->SoldierMatrix[y][x].direction.push_back(45);
					if (y + 3 < this->n && x - 3 >= 0)
					{
						if (this->SoldierMatrix[y + 3][x - 3].free == true)
						{
							if (!opponent)
							{
								SoldierMatrix[y][x].CannonMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x - 3) + to_string(y + 3));
							}
							else
							{
								SoldierMatrix[y][x].CannonMoves.insert(SoldierMatrix[y][x].CannonMoves.begin(), "S" + to_string(x) + to_string(y) + "M" + to_string(x - 3) + to_string(y + 3));
							}
						}
					}
					//2 kills
					if (y - 1 >= 0 && x + 1 < this->m)
					{
						if (this->SoldierMatrix[y - 1][x + 1].free == true)
						{
							for (int i = 2; i <= 3; i++)
							{
								if (y - i >= 0 && x + i < this->m)
								{
									if (this->SoldierMatrix[y - i][x + i].opponent == (!opponent) && this->SoldierMatrix[y - i][x + i].free == false)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y - i));
										}
										else
										{
											SoldierMatrix[y][x].CannonKills.insert(SoldierMatrix[y][x].CannonKills.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y - i));
										}
									}
									else if (this->SoldierMatrix[y - i][x + i].free == true)
									{
										if (opponent)
										{
											SoldierMatrix[y][x].Bomb.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y - i));
										}
										else
										{
											SoldierMatrix[y][x].Bomb.insert(SoldierMatrix[y][x].Bomb.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y - i));
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//left
	// cout << "checkleft" << endl;
	if (x - 2 >= 0)
	{
		if (this->SoldierMatrix[y][x - 1].free == false)
		{
			if (this->SoldierMatrix[y][x - 1].opponent == opponent && this->SoldierMatrix[y][x - 1].townhall == false)
			{
				if (this->SoldierMatrix[y][x - 2].opponent == opponent && this->SoldierMatrix[y][x - 2].townhall == false && this->SoldierMatrix[y][x - 2].free == false)
				{
					left = true;
					this->SoldierMatrix[y][x].cannonEnd = true;
					this->SoldierMatrix[y][x].direction.push_back(0);
					//moves and kills
					//move
					if (x - 3 >= 0)
					{
						if (this->SoldierMatrix[y][x - 3].free == true)
						{
							SoldierMatrix[y][x].CannonMoves.insert(SoldierMatrix[y][x].CannonMoves.begin(), "S" + to_string(x) + to_string(y) + "M" + to_string(x - 3) + to_string(y));
						}
					}
					//2 kills
					if (x + 1 < this->m)
					{
						if (this->SoldierMatrix[y][x + 1].free == true)
						{
							for (int i = 2; i <= 3; i++)
							{
								if (x + i < this->m)
								{
									if (this->SoldierMatrix[y][x + i].opponent == (!opponent) && this->SoldierMatrix[y][x + i].free == false)
									{
										SoldierMatrix[y][x].CannonKills.insert(SoldierMatrix[y][x].CannonKills.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y));
									}
									else if (this->SoldierMatrix[y][x + i].free == true)
									{
										SoldierMatrix[y][x].Bomb.insert(SoldierMatrix[y][x].Bomb.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y));
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//right
	// cout << "right" << endl;
	if (x + 2 < this->m)
	{
		if (this->SoldierMatrix[y][x + 1].free == false)
		{
			if (this->SoldierMatrix[y][x + 1].opponent == opponent && this->SoldierMatrix[y][x + 1].townhall == false)
			{
				if (this->SoldierMatrix[y][x + 2].opponent == opponent && this->SoldierMatrix[y][x + 2].townhall == false && this->SoldierMatrix[y][x + 2].free == false)
				{
					right = true;
					this->SoldierMatrix[y][x].cannonEnd = true;
					this->SoldierMatrix[y][x].direction.push_back(180);
					//moves and kills
					//move
					if (x + 3 < this->m)
					{
						if (this->SoldierMatrix[y][x + 3].free == true)
						{
							SoldierMatrix[y][x].CannonMoves.insert(SoldierMatrix[y][x].CannonMoves.begin(), "S" + to_string(x) + to_string(y) + "M" + to_string(x + 3) + to_string(y));
						}
					}
					//2 kills
					if (x - 1 >= 0)
					{
						if (this->SoldierMatrix[y][x - 1].free == true)
						{
							for (int i = 2; i <= 3; i++)
							{
								if (x - i >= 0)
								{
									if (this->SoldierMatrix[y][x - i].opponent == (!opponent) && this->SoldierMatrix[y][x - i].free == false)
									{
										SoldierMatrix[y][x].CannonKills.insert(SoldierMatrix[y][x].CannonKills.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y));
									}
									else if (this->SoldierMatrix[y][x - i].free == true)
									{
										SoldierMatrix[y][x].Bomb.insert(SoldierMatrix[y][x].Bomb.begin(), "S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y));
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (opponent)
	{
		OpSoldiers[y * 10 + x] = this->SoldierMatrix[y][x];
	}
	else
	{
		MeSoldiers[y * 10 + x] = this->SoldierMatrix[y][x];
	}
	return 1;
}

int GameMatrix::ExecuteMoves(string x)
{
	char s1 = x.at(0);
	int x1 = int(x.at(1)) - int('0');
	int y1 = int(x.at(2)) - int('0');
	char move = x.at(3);
	int x2 = int(x.at(4)) - int('0');
	int y2 = int(x.at(5)) - int('0');
	vector<vector<int>> p11(9, vector<int>(2, 0));
	vector<vector<int>> p12(9, vector<int>(2, 0));
	vector<vector<int>> p13(9, vector<int>(2, 0));
	vector<vector<int>> p21(9, vector<int>(2, 0));
	vector<vector<int>> p22(9, vector<int>(2, 0));
	vector<vector<int>> p23(9, vector<int>(2, 0));

	// cout << x1 << y1 << x2 << y2 << endl;
	p11[0][0] = x1;
	p11[0][1] = y1;
	p11[1][0] = x1 - 1;
	p11[1][1] = y1;
	p11[2][0] = x1 + 1;
	p11[2][1] = y1;
	p11[3][0] = x1;
	p11[3][1] = y1 + 1;
	p11[4][0] = x1 - 1;
	p11[4][1] = y1 + 1;
	p11[5][0] = x1 + 1;
	p11[5][1] = y1 + 1;
	p11[6][0] = x1;
	p11[6][1] = y1 - 1;
	p11[7][0] = x1 - 1;
	p11[7][1] = y1 - 1;
	p11[8][0] = x1 + 1;
	p11[8][1] = y1 - 1;

	p12[0][0] = x1;
	p12[0][1] = y1;
	p12[1][0] = x1 - 2;
	p12[1][1] = y1;
	p12[2][0] = x1 + 2;
	p12[2][1] = y1;
	p12[3][0] = x1;
	p12[3][1] = y1 + 2;
	p12[4][0] = x1 - 2;
	p12[4][1] = y1 + 2;
	p12[5][0] = x1 + 2;
	p12[5][1] = y1 + 2;
	p12[6][0] = x1;
	p12[6][1] = y1 - 2;
	p12[7][0] = x1 - 2;
	p12[7][1] = y1 - 2;
	p12[8][0] = x1 + 2;
	p12[8][1] = y1 - 2;

	p13[0][0] = x1;
	p13[0][1] = y1;
	p13[1][0] = x1 - 3;
	p13[1][1] = y1;
	p13[2][0] = x1 + 3;
	p13[2][1] = y1;
	p13[3][0] = x1;
	p13[3][1] = y1 + 3;
	p13[4][0] = x1 - 3;
	p13[4][1] = y1 + 3;
	p13[5][0] = x1 + 3;
	p13[5][1] = y1 + 3;
	p13[6][0] = x1;
	p13[6][1] = y1 - 3;
	p13[7][0] = x1 - 3;
	p13[7][1] = y1 - 3;
	p13[8][0] = x1 + 3;
	p13[8][1] = y1 - 3;

	p21[0][0] = x2;
	p21[0][1] = y2;
	p21[1][0] = x2 - 1;
	p21[1][1] = y2;
	p21[2][0] = x2 + 1;
	p21[2][1] = y2;
	p21[3][0] = x2;
	p21[3][1] = y2 + 1;
	p21[4][0] = x2 - 1;
	p21[4][1] = y2 + 1;
	p21[5][0] = x2 + 1;
	p21[5][1] = y2 + 1;
	p21[6][0] = x2;
	p21[6][1] = y2 - 1;
	p21[7][0] = x2 - 1;
	p21[7][1] = y2 - 1;
	p21[8][0] = x2 + 1;
	p21[8][1] = y2 - 1;

	p22[0][0] = x2;
	p22[0][1] = y2;
	p22[1][0] = x2 - 2;
	p22[1][1] = y2;
	p22[2][0] = x2 + 2;
	p22[2][1] = y2;
	p22[3][0] = x2;
	p22[3][1] = y2 + 2;
	p22[4][0] = x2 - 2;
	p22[4][1] = y2 + 2;
	p22[5][0] = x2 + 2;
	p22[5][1] = y2 + 2;
	p22[6][0] = x2;
	p22[6][1] = y2 - 2;
	p22[7][0] = x2 - 2;
	p22[7][1] = y2 - 2;
	p22[8][0] = x2 + 2;
	p22[8][1] = y2 - 2;

	p23[0][0] = x2;
	p23[0][1] = y2;
	p23[1][0] = x2 - 3;
	p23[1][1] = y2;
	p23[2][0] = x2 + 3;
	p23[2][1] = y2;
	p23[3][0] = x2;
	p23[3][1] = y2 + 3;
	p23[4][0] = x2 - 3;
	p23[4][1] = y2 + 3;
	p23[5][0] = x2 + 3;
	p23[5][1] = y2 + 3;
	p23[6][0] = x2;
	p23[6][1] = y2 - 3;
	p23[7][0] = x2 - 3;
	p23[7][1] = y2 - 3;
	p23[8][0] = x2 + 3;
	p23[8][1] = y2 - 3;

	if (x1 < 0 || x1 >= this->m || y1 < 0 || y1 >= this->n || x2 < 0 || x2 >= this->m || y2 < 0 || y2 >= this->n)
	{
		// cout << "Wrong Coordinates: " << x << endl;
		return -1;
	}
	bool opponent = this->SoldierMatrix[y1][x1].opponent;
	if (move == 'M')
	{
		bool x2y2free = false;
		bool x2y2town = false;
		if (this->SoldierMatrix[y2][x2].free == true)
		{
			x2y2free = true;
			this->SoldierMatrix[y1][x1].free = true;
			this->SoldierMatrix[y1][x1].opponent = false;
			this->SoldierMatrix[y1][x1].townhall = false;
			this->SoldierMatrix[y1][x1].Moves.clear();
			this->SoldierMatrix[y1][x1].KillMoves.clear();
			this->SoldierMatrix[y1][x1].CannonMoves.clear();
			this->SoldierMatrix[y1][x1].CannonKills.clear();
			this->SoldierMatrix[y1][x1].Bomb.clear();

			this->SoldierMatrix[y2][x2].free = false;
			this->SoldierMatrix[y2][x2].opponent = opponent;
			this->SoldierMatrix[y2][x2].townhall = false;
			this->SoldierMatrix[y2][x2].Moves.clear();
			this->SoldierMatrix[y2][x2].KillMoves.clear();
			this->SoldierMatrix[y2][x2].CannonMoves.clear();
			this->SoldierMatrix[y2][x2].CannonKills.clear();
			this->SoldierMatrix[y2][x2].Bomb.clear();
		}
		else if (this->SoldierMatrix[y2][x2].opponent != opponent)
		{
			x2y2free = false;
			this->SoldierMatrix[y1][x1].free = true;
			this->SoldierMatrix[y1][x1].opponent = false;
			this->SoldierMatrix[y1][x1].townhall = false;
			this->SoldierMatrix[y1][x1].Moves.clear();
			this->SoldierMatrix[y1][x1].KillMoves.clear();
			this->SoldierMatrix[y1][x1].CannonMoves.clear();
			this->SoldierMatrix[y1][x1].CannonKills.clear();
			this->SoldierMatrix[y1][x1].Bomb.clear();
			if(this->SoldierMatrix[y2][x2].townhall)
			{
				x2y2town = true;
			}
			this->SoldierMatrix[y2][x2].free = false;
			this->SoldierMatrix[y2][x2].opponent = opponent;
			this->SoldierMatrix[y2][x2].townhall = false;
			this->SoldierMatrix[y2][x2].Moves.clear();
			this->SoldierMatrix[y2][x2].KillMoves.clear();
			this->SoldierMatrix[y2][x2].CannonMoves.clear();
			this->SoldierMatrix[y2][x2].CannonKills.clear();
			this->SoldierMatrix[y2][x2].Bomb.clear();
		}
		else
		{
			cout << "Wrong Move: " + x << endl;
			printGameMatrix(this->SoldierMatrix);
			return -1;
		}
		// multithreading pointer functions are above
		pthread_t threads[3];
		pthread_attr_t attr;
		void *status;
		int rc;
		int r1;
		int r2;
		if (pthread_mutex_init(&locks, NULL) != 0)
		{
			printf("\n mutex init has failed\n");
		}
		coordinate cord[9];
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		for (int j = 0; j < 9; j = j + 3)
		{
			// Initialize and set thread joinable
			for (int i = 0; i < 3; i++)
			{
				cord[i + j].x = p11[j + i][0];
				cord[i + j].y = p11[j + i][1];
				cord[i + j].object = this;
				// cout << "main() : creating thread, " << i << endl;
				rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
				if (rc)
				{
					cout << "Error:unable to create thread," << rc << endl;
					exit(-1);
				}
			}
			// free attribute and wait for the other threads
			for (int i = 0; i < 3; i++)
			{
				rc = pthread_join(threads[i], &status);
				if (rc)
				{
					cout << "Error:unable to join," << rc << endl;
					exit(-1);
				}
				// cout << "Main: completed thread id :" << i;
				// cout << "  exiting with status :" << status << endl;
			}
		}
		for (int j = 0; j < 9; j = j + 3)
		{
			// Initialize and set thread joinable
			for (int i = 0; i < 3; i++)
			{
				cord[i + j].x = p12[j + i][0];
				cord[i + j].y = p12[j + i][1];
				cord[i + j].object = this;
				// cout << "main() : creating thread, " << i << endl;
				rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
				if (rc)
				{
					cout << "Error:unable to create thread," << rc << endl;
					exit(-1);
				}
			}
			// free attribute and wait for the other threads
			for (int i = 0; i < 3; i++)
			{
				rc = pthread_join(threads[i], &status);
				if (rc)
				{
					cout << "Error:unable to join," << rc << endl;
					exit(-1);
				}
				// cout << "Main: completed thread id :" << i;
				// cout << "  exiting with status :" << status << endl;
			}
		}
		for (int j = 0; j < 9; j = j + 3)
		{
			// Initialize and set thread joinable
			for (int i = 0; i < 3; i++)
			{
				cord[i + j].x = p13[j + i][0];
				cord[i + j].y = p13[j + i][1];
				cord[i + j].object = this;
				// cout << "main() : creating thread, " << i << endl;
				rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
				if (rc)
				{
					cout << "Error:unable to create thread," << rc << endl;
					exit(-1);
				}
			}
			// free attribute and wait for the other threads
			for (int i = 0; i < 3; i++)
			{
				rc = pthread_join(threads[i], &status);
				if (rc)
				{
					cout << "Error:unable to join," << rc << endl;
					exit(-1);
				}
				// cout << "Main: completed thread id :" << i;
				// cout << "  exiting with status :" << status << endl;
			}
		}

		for (int j = 0; j < 9; j = j + 3)
		{
			// Initialize and set thread joinable
			for (int i = 0; i < 3; i++)
			{
				cord[i + j].x = p21[j + i][0];
				cord[i + j].y = p21[j + i][1];
				cord[i + j].object = this;
				// cout << "main() : creating thread, " << i << endl;
				rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
				if (rc)
				{
					cout << "Error:unable to create thread," << rc << endl;
					exit(-1);
				}
			}
			// free attribute and wait for the other threads
			for (int i = 0; i < 3; i++)
			{
				rc = pthread_join(threads[i], &status);
				if (rc)
				{
					cout << "Error:unable to join," << rc << endl;
					exit(-1);
				}
				// cout << "Main: completed thread id :" << i;
				// cout << "  exiting with status :" << status << endl;
			}
		}
		for (int j = 0; j < 9; j = j + 3)
		{
			// Initialize and set thread joinable
			for (int i = 0; i < 3; i++)
			{
				cord[i + j].x = p22[j + i][0];
				cord[i + j].y = p22[j + i][1];
				cord[i + j].object = this;
				// cout << "main() : creating thread, " << i << endl;
				rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
				if (rc)
				{
					cout << "Error:unable to create thread," << rc << endl;
					exit(-1);
				}
			}
			// free attribute and wait for the other threads
			for (int i = 0; i < 3; i++)
			{
				rc = pthread_join(threads[i], &status);
				if (rc)
				{
					cout << "Error:unable to join," << rc << endl;
					exit(-1);
				}
				// cout << "Main: completed thread id :" << i;
				// cout << "  exiting with status :" << status << endl;
			}
		}
		for (int j = 0; j < 9; j = j + 3)
		{
			// Initialize and set thread joinable
			for (int i = 0; i < 3; i++)
			{
				cord[i + j].x = p23[j + i][0];
				cord[i + j].y = p23[j + i][1];
				cord[i + j].object = this;
				// cout << "main() : creating thread, " << i << endl;
				rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
				if (rc)
				{
					cout << "Error:unable to create thread," << rc << endl;
					exit(-1);
				}
			}
			// free attribute and wait for the other threads
			for (int i = 0; i < 3; i++)
			{
				rc = pthread_join(threads[i], &status);
				if (rc)
				{
					cout << "Error:unable to join," << rc << endl;
					exit(-1);
				}
				// cout << "Main: completed thread id :" << i;
				// cout << "  exiting with status :" << status << endl;
			}
		}
		pthread_attr_destroy(&attr);
		pthread_mutex_destroy(&locks);
		if (opponent)
		{
			OpSoldiers.erase(y1 * 10 + x1);
			OpSoldiers[y2 * 10 + x2] = this->SoldierMatrix[y2][x2];
		}
		else
		{
			MeSoldiers.erase(y1 * 10 + x1);
			MeSoldiers[y2 * 10 + x2] = this->SoldierMatrix[y2][x2];
		}
		if (!x2y2free)
		{
			if (opponent)
			{
				if(x2y2town)
				{
					this->MeNoOfTownhalls = this->MeNoOfTownhalls - 1;
				}
				else
				{
					MeSoldiers.erase(y2 * 10 + x2);
					this->MeNoOfSoldiers = this->MeNoOfSoldiers - 1;
				}
			}
			else
			{
				if(x2y2town)
				{
					this->OpNoOfTownhalls = this->OpNoOfTownhalls - 1;
				}
				else
				{
					OpSoldiers.erase(y2 * 10 + x2);
					this->OpNoOfSoldiers = this->OpNoOfSoldiers - 1;

				}
			}
		}
	}
	else if (move == 'B')
	{
		bool x2y2town = false;
		if (this->SoldierMatrix[y2][x2].opponent != opponent && this->SoldierMatrix[y2][x2].free == false)
		{
			if(this->SoldierMatrix[y2][x2].townhall)
			{
				x2y2town = true;
			}
			this->SoldierMatrix[y2][x2].free = true;
			this->SoldierMatrix[y2][x2].opponent = false;
			this->SoldierMatrix[y2][x2].townhall = false;
			this->SoldierMatrix[y2][x2].Moves.clear();
			this->SoldierMatrix[y2][x2].KillMoves.clear();
			this->SoldierMatrix[y2][x2].CannonMoves.clear();
			this->SoldierMatrix[y2][x2].CannonKills.clear();
			this->SoldierMatrix[y2][x2].Bomb.clear();

			pthread_t threads[3];
			pthread_attr_t attr;
			void *status;
			int rc;
			int r1;
			int r2;
			if (pthread_mutex_init(&locks, NULL) != 0)
			{
				printf("\n mutex init has failed\n");
			}
			coordinate cord[9];
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
			for (int j = 0; j < 9; j = j + 3)
			{
				// Initialize and set thread joinable
				for (int i = 0; i < 3; i++)
				{
					cord[i + j].x = p21[j + i][0];
					cord[i + j].y = p21[j + i][1];
					cord[i + j].object = this;
					// cout << "main() : creating thread, " << i << endl;
					rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
					if (rc)
					{
						cout << "Error:unable to create thread," << rc << endl;
						exit(-1);
					}
				}
				// free attribute and wait for the other threads
				for (int i = 0; i < 3; i++)
				{
					rc = pthread_join(threads[i], &status);
					if (rc)
					{
						cout << "Error:unable to join," << rc << endl;
						exit(-1);
					}
					// cout << "Main: completed thread id :" << i;
					// cout << "  exiting with status :" << status << endl;
				}
			}
			for (int j = 0; j < 9; j = j + 3)
			{
				// Initialize and set thread joinable
				for (int i = 0; i < 3; i++)
				{
					cord[i + j].x = p22[j + i][0];
					cord[i + j].y = p22[j + i][1];
					cord[i + j].object = this;
					// cout << "main() : creating thread, " << i << endl;
					rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
					if (rc)
					{
						cout << "Error:unable to create thread," << rc << endl;
						exit(-1);
					}
				}
				// free attribute and wait for the other threads
				for (int i = 0; i < 3; i++)
				{
					rc = pthread_join(threads[i], &status);
					if (rc)
					{
						cout << "Error:unable to join," << rc << endl;
						exit(-1);
					}
					// cout << "Main: completed thread id :" << i;
					// cout << "  exiting with status :" << status << endl;
				}
			}
			for (int j = 0; j < 9; j = j + 3)
			{
				// Initialize and set thread joinable
				for (int i = 0; i < 3; i++)
				{
					cord[i + j].x = p23[j + i][0];
					cord[i + j].y = p23[j + i][1];
					cord[i + j].object = this;
					// cout << "main() : creating thread, " << i << endl;
					rc = pthread_create(&threads[i], &attr, MainThread, (void *)&cord[i + j]);
					if (rc)
					{
						cout << "Error:unable to create thread," << rc << endl;
						exit(-1);
					}
				}
				// free attribute and wait for the other threads
				for (int i = 0; i < 3; i++)
				{
					rc = pthread_join(threads[i], &status);
					if (rc)
					{
						cout << "Error:unable to join," << rc << endl;
						exit(-1);
					}
					// cout << "Main: completed thread id :" << i;
					// cout << "  exiting with status :" << status << endl;
				}
			}
			if (opponent)
			{
				if(x2y2town)
				{
					this->MeNoOfTownhalls = this->MeNoOfTownhalls - 1;
				}
				else
				{
					MeSoldiers.erase(y2 * 10 + x2);
					this->MeNoOfSoldiers = this->MeNoOfSoldiers - 1;
				}
			}
			else
			{
				if(x2y2town)
				{
					this->OpNoOfTownhalls = this->OpNoOfTownhalls - 1;
				}
				else
				{
					OpSoldiers.erase(y2 * 10 + x2);
					this->OpNoOfSoldiers = this->OpNoOfSoldiers - 1;

				}
			}
			pthread_attr_destroy(&attr);
			pthread_mutex_destroy(&locks);
		}
		else if (this->SoldierMatrix[y2][x2].opponent == opponent && this->SoldierMatrix[y2][x2].free == false)
		{
			cout << "Wrong Bomb: " + x << endl;
			return -1;
		}
	}
	else
	{
		cout << "Wrong Foramt: " + x << endl;
		return -1;
	}
	return 1;
}



vector<string> GameMatrix::GenerateMoves(unordered_map<int,Soldier> Soldiers)
{
    vector<string> FutureMoves;
	for (auto p:Soldiers)
    {
        FutureMoves.insert(FutureMoves.end(), p.second.CannonKills.begin(), p.second.CannonKills.end());
    }
	for (auto p:Soldiers)
    {
        FutureMoves.insert(FutureMoves.end(), p.second.KillMoves.begin(), p.second.KillMoves.end());
    }
	for (auto p:Soldiers)
    {
        FutureMoves.insert(FutureMoves.end(), p.second.Moves.begin(), p.second.Moves.end());
    }
	    for (auto p:Soldiers)
    {
        FutureMoves.insert(FutureMoves.end(), p.second.CannonMoves.begin(), p.second.CannonMoves.end());
    }
    for (auto p:Soldiers)
    {
        FutureMoves.insert(FutureMoves.end(), p.second.Bomb.begin(), p.second.Bomb.end());
    }
    return FutureMoves;
}

void GameMatrix::GenerateStates(bool opponent)
{
    //get moves by traversing through the hashtable
    if(opponent)
    {
        this->FutureMoves = GenerateMoves(this->OpSoldiers);
    }
    else
    {
        this->FutureMoves = GenerateMoves(this->MeSoldiers);
    }
	for (int i = 0; i < this->FutureMoves.size(); i++)
	{
		GameMatrix *temp = new GameMatrix();
		*temp = *this;
		if(opponent)
		{
			temp->EvaluatedValue = INT32_MAX;
		}
		else
		{
			temp->EvaluatedValue = INT32_MIN;
		}
		//simMateix
		temp->childStates.clear();
		temp->FutureMoves.clear();
		temp->Moves.clear();
		temp->Moves = this->Moves;
		temp->Moves.push_back(FutureMoves[i]); 
		temp->ExecuteMoves(FutureMoves[i]);
		temp->SimMatrix = GetSimMatrix(temp->SoldierMatrix);
		this->childStates.push_back(temp);
		// if(i==0)
		// {
		// 	// cout << FutureMoves[i] <<endl;
		// 	printGameMatrix(temp->SoldierMatrix);
		// }
		// cout << i <<" "<<FutureMoves[i] <<endl;
		// printGameMatrix(temp->SoldierMatrix);
	}
	
    //////////////////////////////////////////////
}

GameMatrix::~GameMatrix()
{
}


#endif