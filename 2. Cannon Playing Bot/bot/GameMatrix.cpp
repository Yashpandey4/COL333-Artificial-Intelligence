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
	int ListMoves(bool oppo);
	int ExecuteMoves(string x);
	void GenerateStates(bool opponent);
	~GameMatrix();
};
GameMatrix::GameMatrix()
{
}

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
int GameMatrix::ListMoves(bool oppo)
{
	/* 
    y: y-coordinate
    x: x-coordinate
    origin is at the left upper corner
    */
    unordered_map<int, Soldier> Soldiers;
	if(oppo)
	{
		Soldiers = this->OpSoldiers;
	}
	else
	{
		Soldiers = this->MeSoldiers;
	}
	for (auto p:Soldiers)
	{
		int x = p.second.x;
		int y = p.second.y;
		this->SoldierMatrix[y][x].CannonKills.clear();
		this->SoldierMatrix[y][x].direction.clear();
		this->SoldierMatrix[y][x].cannonEnd = false;
	}
   	for (auto p:Soldiers)
	{
		int x = p.second.x;
		int y = p.second.y;

		if (x < 0 || x >= this->m || y < 0 || y >= this->n)
		{
			// cout << "Wrong Coordinate " << x << " " << y << endl;
			continue;
		}
		if (this->SoldierMatrix[y][x].free == true)
		{
			// cout << "No Soldier There " << x << " " << y << endl;
			continue;
		}
		if(this->SoldierMatrix[y][x].townhall==true)
		{
			continue;
		}

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
				if (!(this->SoldierMatrix[ycord1][i].opponent == opponent && this->SoldierMatrix[ycord1][i].free == false))
				{
					this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(i) + to_string(ycord1));
				}
				if (this->SoldierMatrix[ycord1][i].opponent == !opponent && this->SoldierMatrix[ycord1][i].free == false) //if there is a opponent
				{
					adjacent = true;
				}
			}
		}
		// cout << "SideMoves" << endl;
		//Sideways
		if (0 <= x - 1)
		{
			if(this->SoldierMatrix[y][x - 1].opponent == !opponent && this->SoldierMatrix[y][x - 1].free == false)
			{
				this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x - 1) + to_string(y));
				adjacent = true;
			}
		}
		if (x + 1 < this->m)
		{
			if(this->SoldierMatrix[y][x + 1].opponent == !opponent && this->SoldierMatrix[y][x + 1].free == false)
			{
				this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x + 1) + to_string(y));
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
						this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(i) + to_string(ycord2));
					}
				}
			}
		}

		//cannon detection//
		////////////////////
		if(this->SoldierMatrix[y][x].cannonEnd == false)
		{
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
							// up = true;
							this->SoldierMatrix[y][x].cannonEnd = true;
							this->SoldierMatrix[y-2][x].cannonEnd = true;
							this->SoldierMatrix[y][x].direction.push_back(270);
							this->SoldierMatrix[y-2][x].direction.push_back(90);
							//moves and kills
							//2 moves
							if (y - 3 >= 0)
							{
								if (this->SoldierMatrix[y - 3][x].free == true)
								{
									this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x) + to_string(y - 3));

									for (int i = 2; i <= 3; i++)
									{
										if (y-2-i >= 0)
										{
											if (this->SoldierMatrix[y-2-i][x].opponent == (!opponent) && this->SoldierMatrix[y-2-i][x].free == false)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y-2) + "B" + to_string(x) + to_string(y-2-i));
												SoldierMatrix[y-2][x].CannonKills.push_back("S" + to_string(x) + to_string(y-2) + "B" + to_string(x) + to_string(y-2-i));
											}
											else if (this->SoldierMatrix[y-2-i][x].free == true)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y-2) + "B" + to_string(x) + to_string(y-2-i));
											}
										}
									}
								}
							}
							
							//2 kills for y
							if (y + 1 < this->n)
							{
								if (this->SoldierMatrix[y + 1][x].free == true)
								{
									this->FutureMoves.push_back("S" + to_string(x) + to_string(y-2) + "M" + to_string(x) + to_string(y+1));
								
									for (int i = 2; i <= 3; i++)
									{
										if (y + i < this->n)
										{
											if (this->SoldierMatrix[y + i][x].opponent == (!opponent) && this->SoldierMatrix[y + i][x].free == false)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y + i));
												SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y + i));
											}
											else if (this->SoldierMatrix[y + i][x].free == true)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x) + to_string(y + i));
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
							// upleft = true;
							this->SoldierMatrix[y][x].cannonEnd = true;
							this->SoldierMatrix[y-2][x-2].cannonEnd = true;
							this->SoldierMatrix[y][x].direction.push_back(315);
							this->SoldierMatrix[y-2][x-2].direction.push_back(135);
							//moves and kills
							//move
							if (y - 3 >= 0 && x - 3 >= 0)
							{
								if (this->SoldierMatrix[y - 3][x - 3].free == true)
								{
									this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x - 3) + to_string(y - 3));
									for (int i = 2; i <= 3; i++)
									{
										if (y-2-i >=0 && x-2-i >=0)
										{
											if (this->SoldierMatrix[y-2-i][x-2-i].opponent == (!opponent) && this->SoldierMatrix[y-2-i][x-2-i].free == false)
											{
												this->FutureMoves.push_back("S" + to_string(x-2) + to_string(y-2) + "B" + to_string(x-2-i) + to_string(y-2-i));
												SoldierMatrix[y-2][x-2].CannonKills.push_back("S" + to_string(x-2) + to_string(y-2) + "B" + to_string(x-2-i) + to_string(y-2-i));
											}
											else if (this->SoldierMatrix[y-2-i][x-2-i].free == true)
											{
												this->FutureMoves.push_back("S" + to_string(x-2) + to_string(y-2) + "B" + to_string(x-2-i) + to_string(y-2-i));
											}
										}
									}
								}
							}
							//2 kills
							if (y + 1 < this->n && x + 1 < this->m)
							{
								if (this->SoldierMatrix[y + 1][x + 1].free == true)
								{
									this->FutureMoves.push_back("S" + to_string(x-2) + to_string(y-2) + "M" + to_string(x+1) + to_string(y+1));
									for (int i = 2; i <= 3; i++)
									{
										if (y + i < this->n && x + i < this->m)
										{
											if (this->SoldierMatrix[y + i][x + i].opponent == (!opponent) && this->SoldierMatrix[y + i][x + i].free == false)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y + i));
												SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y + i));
											}
											else if (this->SoldierMatrix[y + i][x + i].free == true)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y + i));
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
							// upright = true;
							this->SoldierMatrix[y][x].cannonEnd = true;
							this->SoldierMatrix[y-2][x+2].cannonEnd = true;
							this->SoldierMatrix[y][x].direction.push_back(225);
							this->SoldierMatrix[y-2][x+2].direction.push_back(45);
							//moves and kills
							//move
							if (y - 3 >= 0 && x + 3 < this->m)
							{
								if (this->SoldierMatrix[y - 3][x + 3].free == true)
								{
									this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x + 3) + to_string(y - 3));

									for (int i = 2; i <= 3; i++)
									{
										if (y-2-i >=0 && x+2+i <this->m)
										{
											if (this->SoldierMatrix[y-2-i][x+2+i].opponent == (!opponent) && this->SoldierMatrix[y-2-i][x+2+i].free == false)
											{
												this->FutureMoves.push_back("S" + to_string(x+2) + to_string(y-2) + "B" + to_string(x+2+i) + to_string(y-2-i));
												SoldierMatrix[y-2][x+2].CannonKills.push_back("S" + to_string(x+2) + to_string(y-2) + "B" + to_string(x+2+i) + to_string(y-2-i));
											}
											else if (this->SoldierMatrix[y-2-i][x+2+i].free == true)
											{
												this->FutureMoves.push_back("S" + to_string(x+2) + to_string(y-2) + "B" + to_string(x+2+i) + to_string(y-2-i));
											}
										}
									}
								}
							}
							//2 kills
							if (y + 1 < this->n && x - 1 >= 0)
							{
								if (this->SoldierMatrix[y + 1][x - 1].free == true)
								{
									this->FutureMoves.push_back("S" + to_string(x+2) + to_string(y-2) + "M" + to_string(x-1) + to_string(y+1));
									
									for (int i = 2; i <= 3; i++)
									{
										if (y + i < this->n && x - i >= 0)
										{
											if (this->SoldierMatrix[y + i][x - i].opponent == (!opponent) && this->SoldierMatrix[y + i][x - i].free == false)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y + i));
												SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y + i));
											}
											else if (this->SoldierMatrix[y + i][x - i].free == true)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x - i) + to_string(y + i));
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
							// left = true;
							this->SoldierMatrix[y][x].cannonEnd = true;
							this->SoldierMatrix[y][x-2].cannonEnd = true;
							this->SoldierMatrix[y][x].direction.push_back(0);
							this->SoldierMatrix[y][x-2].direction.push_back(0);

							//moves and kills
							//move
							if (x - 3 >= 0)
							{
								if (this->SoldierMatrix[y][x - 3].free == true)
								{
									this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "M" + to_string(x - 3) + to_string(y));

									for (int i = 2; i <= 3; i++)
									{
										if (x-2-i>=0)
										{
											if (this->SoldierMatrix[y][x-2-i].opponent == (!opponent) && this->SoldierMatrix[y][x-2-i].free == false)
											{
												this->FutureMoves.push_back("S" + to_string(x-2) + to_string(y) + "B" + to_string(x-2-i) + to_string(y));
												SoldierMatrix[y][x-2].CannonKills.push_back("S" + to_string(x-2) + to_string(y) + "B" + to_string(x-2-i) + to_string(y));
											}
											else if (this->SoldierMatrix[y][x-2-i].free == true)
											{
												this->FutureMoves.push_back("S" + to_string(x-2) + to_string(y) + "B" + to_string(x-2-i) + to_string(y));
											}
										}
									}
								}
								
							}
							//2 kills
							if (x + 1 < this->m)
							{
								if (this->SoldierMatrix[y][x + 1].free == true)
								{
									this->FutureMoves.push_back("S" + to_string(x-2) + to_string(y) + "M" + to_string(x+1) + to_string(y));

									for (int i = 2; i <= 3; i++)
									{
										if (x + i < this->m)
										{
											if (this->SoldierMatrix[y][x + i].opponent == (!opponent) && this->SoldierMatrix[y][x + i].free == false)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y));
												SoldierMatrix[y][x].CannonKills.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x+i) + to_string(y));
											}
											else if (this->SoldierMatrix[y][x + i].free == true)
											{
												this->FutureMoves.push_back("S" + to_string(x) + to_string(y) + "B" + to_string(x + i) + to_string(y));
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
		if (opponent)
		{
			OpSoldiers[y * 10 + x] = this->SoldierMatrix[y][x];
		}
		else
		{
			MeSoldiers[y * 10 + x] = this->SoldierMatrix[y][x];
		}
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
			this->SoldierMatrix[y1][x1].CannonKills.clear();

			this->SoldierMatrix[y2][x2].free = false;
			this->SoldierMatrix[y2][x2].opponent = opponent;
			this->SoldierMatrix[y2][x2].townhall = false;
			this->SoldierMatrix[y2][x2].CannonKills.clear();
		}
		else if (this->SoldierMatrix[y2][x2].opponent != opponent)
		{
			x2y2free = false;
			this->SoldierMatrix[y1][x1].free = true;
			this->SoldierMatrix[y1][x1].opponent = false;
			this->SoldierMatrix[y1][x1].townhall = false;
			this->SoldierMatrix[y1][x1].CannonKills.clear();
			if(this->SoldierMatrix[y2][x2].townhall)
			{
				x2y2town = true;
			}
			this->SoldierMatrix[y2][x2].free = false;
			this->SoldierMatrix[y2][x2].opponent = opponent;
			this->SoldierMatrix[y2][x2].townhall = false;
			this->SoldierMatrix[y2][x2].CannonKills.clear();
		}
		else
		{
			cout << "Wrong Move: " + x << endl;
			printGameMatrix(this->SoldierMatrix);
			return -1;
		}
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
			this->SoldierMatrix[y2][x2].CannonKills.clear();

			
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

void GameMatrix::GenerateStates(bool opponent)
{
    //get moves by traversing through the hashtable
	this->FutureMoves.erase(this->FutureMoves.begin(), this->FutureMoves.end());
	this->childStates.erase(this->childStates.begin(), this->childStates.end());
	this->ListMoves(opponent);
	for (int i = 0; i < this->FutureMoves.size(); i++)
	{
		// cout << "gone" << endl;	
		// cout << i <<" "<<FutureMoves[i] <<endl;
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
	}
	
    //////////////////////////////////////////////
}

GameMatrix::~GameMatrix()
{
	// cout << "deleting" << endl;
	for(auto p:this->childStates)
	{
		delete p;
	}
	// this->childStates.clear();
	// this->OpSoldiers.clear();
	// this->MeSoldiers.clear();
	// this->FutureMoves.clear();
}


int main(int argc, char const *argv[])
{
	GameMatrix game(8, 8, 12);
	//checking if it is positioning the pieces correctly or not
	printMatrix(game.SimMatrix);
	cout << endl;
	printGameMatrix(game.SoldierMatrix);
	//cases if the moves are correct or not
	Soldier man;
	vector<vector<Soldier>> rowSold(8, vector<Soldier>(8, man));

	// cout << game.n << endl;
	// cout << "FOR x: " << 4 << " y: " << 4 << endl;
	// cout << "moves " << game.SoldierMatrix[4][4].Moves.size() << endl;
	// printMoves(game.SoldierMatrix[4][4].Moves);
	// cout << "killmoves " << game.SoldierMatrix[4][4].KillMoves.size() << endl;
	// printMoves(game.SoldierMatrix[4][4].KillMoves);
	// cout << "CannonKills " << game.SoldierMatrix[4][4].CannonKills.size() << endl;
	// printMoves(game.SoldierMatrix[4][4].CannonKills);
	// cout << "cannonmoves " << game.SoldierMatrix[4][4].CannonMoves.size() << endl;
	// printMoves(game.SoldierMatrix[4][4].CannonMoves);
	// cout << "bomb " << game.SoldierMatrix[4][4].Bomb.size() << endl;
	// printMoves(game.SoldierMatrix[4][4].Bomb);

	// cout << "FOR x: " << 1 << " y: " << 2 << endl;
	// cout << "moves " << game.SoldierMatrix[2][1].Moves.size() << endl;
	// printMoves(game.SoldierMatrix[2][1].Moves);
	// cout << "killmoves " << game.SoldierMatrix[2][1].KillMoves.size() << endl;
	// printMoves(game.SoldierMatrix[2][1].KillMoves);
	// cout << "CannonKills " << game.SoldierMatrix[2][1].CannonKills.size() << endl;
	// printMoves(game.SoldierMatrix[2][1].CannonKills);
	// cout << "cannonmoves " << game.SoldierMatrix[2][1].CannonMoves.size() << endl;
	// printMoves(game.SoldierMatrix[2][1].CannonMoves);
	// cout << "bomb " << game.SoldierMatrix[2][1].Bomb.size() << endl;
	// printMoves(game.SoldierMatrix[2][1].Bomb);

	// cout << "FOR x: " << 2 << " y: " << 6 << endl;
	// cout << "moves " << game.SoldierMatrix[6][2].Moves.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].Moves);
	// cout << "killmoves " << game.SoldierMatrix[6][2].KillMoves.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].KillMoves);
	// cout << "CannonKills " << game.SoldierMatrix[6][2].CannonKills.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].CannonKills);
	// cout << "cannonmoves " << game.SoldierMatrix[6][2].CannonMoves.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].CannonMoves);
	// cout << "bomb " << game.SoldierMatrix[6][2].Bomb.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].Bomb);

	// cout << game.SoldierMatrix[4][3].opponent << endl;

	// game.ExecuteMoves("S25M24");
	// printGameMatrix(game.SoldierMatrix);
	// game.ExecuteMoves("S25M24");
	// game.ExecuteMoves("S24M23");
	// printGameMatrix(game.SoldierMatrix);

	// cout << "FOR x: " << 3 << " y: " << 2 << endl;
	// cout << "moves " << game.SoldierMatrix[2][3].Moves.size() << endl;
	// printMoves(game.SoldierMatrix[2][3].Moves);
	// cout << "killmoves " << game.SoldierMatrix[2][3].KillMoves.size() << endl;
	// printMoves(game.SoldierMatrix[2][3].KillMoves);
	// cout << "CannonKills " << game.SoldierMatrix[2][3].CannonKills.size() << endl;
	// printMoves(game.SoldierMatrix[2][3].CannonKills);
	// cout << "cannonmoves " << game.SoldierMatrix[2][3].CannonMoves.size() << endl;
	// printMoves(game.SoldierMatrix[2][3].CannonMoves);
	// cout << "bomb " << game.SoldierMatrix[2][3].Bomb.size() << endl;
	// printMoves(game.SoldierMatrix[2][3].Bomb);

	// cout << "FOR x: " << 2 << " y: " << 6 << endl;
	// cout << "moves " << game.SoldierMatrix[6][2].Moves.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].Moves);
	// cout << "killmoves " << game.SoldierMatrix[6][2].KillMoves.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].KillMoves);
	// cout << "CannonKills " << game.SoldierMatrix[6][2].CannonKills.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].CannonKills);
	// cout << "cannonmoves " << game.SoldierMatrix[6][2].CannonMoves.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].CannonMoves);
	// cout << "bomb " << game.SoldierMatrix[6][2].Bomb.size() << endl;
	// printMoves(game.SoldierMatrix[6][2].Bomb);

	game.ExecuteMoves("S47M56");
	cout << "Move: S47M56" <<endl;
	printGameMatrix(game.SoldierMatrix);

	// cout << "FOR x: " << 0 << " y: " << 7 << endl;
	// cout << "moves " << game.SoldierMatrix[7][0].Moves.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].Moves);
	// cout << "killmoves " << game.SoldierMatrix[7][0].KillMoves.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].KillMoves);
	// cout << "CannonKills " << game.SoldierMatrix[7][0].CannonKills.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].CannonKills);
	// cout << "cannonmoves " << game.SoldierMatrix[7][0].CannonMoves.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].CannonMoves);
	// cout << "bomb " << game.SoldierMatrix[7][0].Bomb.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].Bomb);

	// game.ExecuteMoves("S45M34");
	// cout << "Move: S45M34" <<endl;
	// printGameMatrix(game.SoldierMatrix);
	// cout << "FOR x: " << 0 << " y: " << 7 << endl;
	// cout << "moves " << game.SoldierMatrix[7][0].Moves.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].Moves);
	// cout << "killmoves " << game.SoldierMatrix[7][0].KillMoves.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].KillMoves);
	// cout << "CannonKills " << game.SoldierMatrix[7][0].CannonKills.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].CannonKills);
	// cout << "cannonmoves " << game.SoldierMatrix[7][0].CannonMoves.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].CannonMoves);
	// cout << "bomb " << game.SoldierMatrix[7][0].Bomb.size() << endl;
	// printMoves(game.SoldierMatrix[7][0].Bomb);
	// //case 1
	// if(game.MeSoldiers.find(32)==game.MeSoldiers.end())
	// {
	// 	cout << "gone" << endl;
	// }
	// else
	// {
	// 	cout << "not gone" << endl;
	// }
	// game.ExecuteMoves("S32M23");
	// printGameMatrix(game.SoldierMatrix);

	// if(game.MeSoldiers.find(32)==game.MeSoldiers.end())
	// {
	// 	cout << "gone" << endl;
	// }
	// else
	// {
	// 	cout << "not gone" << endl;
	// }
	// if(game.OpSoldiers.find(32)==game.OpSoldiers.end())
	// {
	// 	cout << "gone" << endl;
	// }
	// else
	// {
	// 	cout << "not gone" << endl;
	// }
	// if(game.OpSoldiers.find(23)==game.OpSoldiers.end())
	// {
	// 	cout << "gone" << endl;
	// }
	// else
	// {
	// 	cout << "not gone" << endl;
	// }

	//case 2
	// game.ExecuteMoves("S52M43");
	// cout << "Move: S52M43" <<endl;	
	// printGameMatrix(game.SoldierMatrix);

	// if (game.OpSoldiers.find(34) == game.OpSoldiers.end())
	// {
	// 	cout << "gone" << endl;
	// }
	// else
	// {
	// 	cout << "not gone" << endl;
	// }

	game.ListMoves(false);
	cout << "Future Moves: "<<game.FutureMoves.size() <<endl;
	printMoves(game.FutureMoves);
	cout << "CannonKills " << game.SoldierMatrix[5][4].CannonKills.size() << endl;
	cout << "CannonKills " << game.SoldierMatrix[5][2].CannonKills.size() << endl;

	printMoves(game.SoldierMatrix[5][4].CannonKills);
	printMoves(game.SoldierMatrix[2][5].CannonKills);

	// game.ExecuteMoves("S45B43");
	// cout << "Move: 45B43" <<endl;	
	// printGameMatrix(game.SoldierMatrix);
	// if (game.OpSoldiers.find(34) == game.OpSoldiers.end())
	// {
	// 	cout << "gone" << endl;
	// }
	// else
	// {
	// 	cout << "not gone" << endl;
	// }
	// game.ExecuteMoves("S23M45");

	game.GenerateStates(false);
	cout << game.childStates.size() << endl;
	printGameMatrix(game.childStates.at(15)->SoldierMatrix);
	printMatrix(game.childStates.at(15)->SimMatrix);
	return 0;
}
