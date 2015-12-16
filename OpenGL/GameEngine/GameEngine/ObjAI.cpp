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
