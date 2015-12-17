#pragma once
#include <memory.h>
#include <list>
#include <time.h>

struct stAstarNode
{
	stAstarNode()
	{
		nHeuristic = 0;
		nGoal = 0;
		nFscore = 0;
		parent = std::pair<int, int>(0, 0);
	}

	int nHeuristic;
	int nGoal;
	int nFscore;
	std::pair<int, int> parent;
};

class ObjectAI
{
public:
	ObjectAI() 
	{
		m_bReached = false;
		m_bPathFound = false;
		m_nPosX = 0;
		m_nPosZ = 0;
		m_nCurrentPosX = 0;
		m_nCurrentPosZ = 0;
		m_nPointer = 1;
		m_nStep = 1;

		int tempdel[5][2] = 
		{
			{ 0,0 },
			{ 1,0 },
			{ 0,1 },
			{ -1,0 },
			{ 0,-1 },
		};
		memcpy(m_delta, tempdel, sizeof(m_delta));

		int	temp[10][10];
		memset(temp, 0, sizeof(temp));
		srand(time(NULL));
		for(int i = 0; i<50; i++)
		{
			int x= 0;
			int y= 0;
			x = rand() % 10;
			y = rand() % 10;
			temp[x][y] = 1;
		}
			
		temp[0][0] = 0;
		temp[9][9] = 0;
		/*{
			{ 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 },
			{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
			{ 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 },
			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
			{ 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 },
			{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 0 },
			{ 0, 1, 0, 1, 0, 0, 0, 1, 0, 1 },
			{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
			{ 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 }
		};*/
		memcpy(m_map, temp, sizeof(m_map));

		memcpy(m_CloseMap, temp, sizeof(m_CloseMap));
		
		memset(m_Path, 0, sizeof(m_Path));
	};
	
	~ObjectAI() {};

	bool PathFinding(int x, int z);
	
	//DFS
	int		m_map[10][10];
	int		m_Path[101];
	int		m_delta[5][2];
	
	int		m_nPointer;
	int		m_nPosX;
	int		m_nPosZ;
	int		m_nStep;
	int		m_nCurrentPosX;
	int		m_nCurrentPosZ;
	
	bool	m_bReached;
	bool	m_bPathFound;

	//A* Path Finding
	stAstarNode m_AstarMap[10][10];
	std::list<std::pair<int, int>> m_openlist;
	int m_CloseMap[10][10];
	std::list<std::pair<int, int>> m_AstarPath;
	std::list<std::pair<int, int>>::iterator m_itrStep;
	void AstarInitialize();
	bool AstarPathFinding(std::pair<int, int>&, std::pair<int, int>&);
	int	 CalculateGscore(std::pair<int, int>&, std::pair<int, int>&);
	void CalculateNeighbor(std::pair<int, int>&currentpoint, std::pair<int, int>&target);
	bool UpdateNeighbor(int nHeuristic, std::pair<int, int>&parent, std::pair<int, int>&neighbor, std::pair<int, int>&target);
};