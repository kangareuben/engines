#pragma once
#include <memory.h>
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

		int	temp[10][10] =
		{
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
		};
		memcpy(m_map, temp, sizeof(m_map));
		
		memset(m_Path, 0, sizeof(m_Path));
	};
	
	~ObjectAI() {};

	bool PathFinding(int x, int z);
	//void AIAction();

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
};