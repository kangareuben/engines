#include "ObjAI.h"

bool ObjectAI::PathFinding(int x, int z)
{
	int tempmap[10][10];
	memcpy(tempmap, m_map, sizeof(tempmap));
	
	while (m_nPointer != 0)
	{
		if (m_nPointer < 101)
		{
			m_Path[m_nPointer]++;
			if (m_Path[m_nPointer] < 5)
			{
				int tx = m_nPosX + m_delta[m_Path[m_nPointer]][0];
				int tz = m_nPosZ + m_delta[m_Path[m_nPointer]][1];
				if (tx == x && tz == z)
				{
					break;
				}
				else
				if (tempmap[tx][tz] == 0)
				{
					m_nPosX = tx;
					m_nPosZ = tz;
					tempmap[tx][tz] = 1;
					m_nPointer++;

				}
			}
			else
			{
				m_Path[m_nPointer] = 0;
				m_nPointer--;
				tempmap[m_nPosX][m_nPosZ] = 0;
				m_nPosX -= m_delta[m_Path[m_nPointer]][0];
				m_nPosZ -= m_delta[m_Path[m_nPointer]][1];
			}
		}
		else
		{
			m_nPointer--;
			tempmap[m_nPosX][m_nPosZ] = 0;
			m_nPosX -= m_delta[m_Path[m_nPointer]][0];
			m_nPosZ -= m_delta[m_Path[m_nPointer]][1];
		}
	}
	
	return true;
}

void ObjectAI::AstarInitialize()
{
	m_AstarPath.clear();
	int	temp[10][10];
			memset(temp, 0, sizeof(temp));
			srand(time(NULL));
			for(int i = 0; i<75; i++)
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
			memset(m_AstarMap, 0, sizeof(m_AstarMap));
			m_openlist.clear();
}

int ObjectAI::CalculateGscore(std::pair<int, int>&start, std::pair<int, int>&target)
{
	int x, y;
	x = abs(start.first - target.first);
	y = abs(start.second - target.second);
	if (x > y)
	{
		int t;
		t = x;
		x = y;
		y = t;
	}
	y = y - x;
	return (14 * x + 10 * y);
}

bool ObjectAI::UpdateNeighbor(int nHeuristic, std::pair<int, int>&parent, std::pair<int, int>&neighbor, std::pair<int, int>&target)
{
	//if this node doesn't exist, just skip it
	if(neighbor.first<0 || neighbor.second<0) return false;
	if(neighbor.first>9 || neighbor.second>9) return false;
	//if this node is unreachable, then just skip it
	if (m_CloseMap[neighbor.first][neighbor.second]!=0)	return false;
	//if this is a new node, update it directly and add it to openlist
	if (m_AstarMap[neighbor.first][neighbor.second].nFscore == 0)
	{
		m_AstarMap[neighbor.first][neighbor.second].nGoal = CalculateGscore(neighbor, target);
		m_AstarMap[neighbor.first][neighbor.second].nHeuristic = nHeuristic;
		m_AstarMap[neighbor.first][neighbor.second].nFscore = m_AstarMap[neighbor.first][neighbor.second].nGoal + nHeuristic;
		m_AstarMap[neighbor.first][neighbor.second].parent = parent;
		m_openlist.push_back(neighbor);		
	}
	//if this is a node in open list, see if it need update
	else if (nHeuristic < m_AstarMap[neighbor.first][neighbor.second].nHeuristic)
	{
		m_AstarMap[neighbor.first][neighbor.second].nHeuristic = nHeuristic;
		m_AstarMap[neighbor.first][neighbor.second].nFscore = m_AstarMap[neighbor.first][neighbor.second].nGoal + nHeuristic;
		m_AstarMap[neighbor.first][neighbor.second].parent = parent;
	}
}

void ObjectAI::CalculateNeighbor(std::pair<int, int>&currentpoint, std::pair<int, int>&target)
{
	int nCurrentHeu = m_AstarMap[currentpoint.first][currentpoint.second].nHeuristic;
	std::pair<int, int> neighborpoint;
	// update neighborpoint 8 direction
	//1,0
	neighborpoint = currentpoint;
	neighborpoint.first += 1;
	UpdateNeighbor(nCurrentHeu+10, currentpoint, neighborpoint, target);
	//1,1
	neighborpoint.second += 1;
	UpdateNeighbor(nCurrentHeu + 14, currentpoint, neighborpoint, target);
	//1,-1
	neighborpoint.second -= 2;
	UpdateNeighbor(nCurrentHeu + 14, currentpoint, neighborpoint, target);
	//0,-1
	neighborpoint.first -= 1;
	UpdateNeighbor(nCurrentHeu + 10, currentpoint, neighborpoint, target);
	//0,1
	neighborpoint.second += 2;
	UpdateNeighbor(nCurrentHeu + 10, currentpoint, neighborpoint, target);
	//-1,1
	neighborpoint.first -= 1;
	UpdateNeighbor(nCurrentHeu + 14, currentpoint, neighborpoint, target);
	//-1,0
	neighborpoint.second -= 1;
	UpdateNeighbor(nCurrentHeu + 10, currentpoint, neighborpoint, target);
	//-1,-1
	neighborpoint.second -= 1;
	UpdateNeighbor(nCurrentHeu + 14, currentpoint, neighborpoint, target);
	return;
}

bool ObjectAI::AstarPathFinding(std::pair<int, int>&start, std::pair<int, int>&target)
{
	std::pair<int, int> currentpoint;	//current node we are at
	m_openlist.push_back(start);		//add start point to open node list
	//calculate f, h, g for start point first;
	m_AstarMap[start.first][start.second].nHeuristic = 0;
	m_AstarMap[start.first][start.second].nGoal = CalculateGscore(start, target);
	m_AstarMap[start.first][start.second].nFscore = m_AstarMap[start.first][start.second].nHeuristic + m_AstarMap[start.first][start.second].nGoal;
	//A star loop
	while (true)
	{
		std::list<std::pair<int, int>>::iterator itr1, itr2;
		itr2 = m_openlist.begin();
		//find next step
		for (itr1 = m_openlist.begin(); itr1 != m_openlist.end(); itr1++)
		{
			if (m_AstarMap[itr1->first][itr1->second].nFscore < m_AstarMap[itr2->first][itr2->second].nFscore)
				itr2 = itr1;
		}
		//dead end, no path, return
		if (itr2 == m_openlist.end()) return false;
		//save itr2 as the current point and erase it in open list, mark it in colse list
		currentpoint = *itr2;
		m_openlist.erase(itr2);
		m_CloseMap[currentpoint.first][currentpoint.second] = 1;
		//if we reach the target, return
		if (currentpoint == target)
		{
			m_AstarPath.clear();
			m_AstarPath.push_front(currentpoint);
			while(currentpoint != start)
			{
				currentpoint = m_AstarMap[currentpoint.first][currentpoint.second].parent;
				m_AstarPath.push_front(currentpoint);
			}
			m_itrStep = m_AstarPath.begin();
			m_nCurrentPosX = m_itrStep->first;
			m_nCurrentPosZ = m_itrStep->second;
			return true;
		}
		//calculate the neighbor nodes' h, g, f value, 8 direction
		CalculateNeighbor(currentpoint, target);
	}
}
