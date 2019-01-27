#include "stdafx.h"
#include "DoorGeo.h"

CDoorGeo::CDoorGeo()
{

}

CDoorGeo::CDoorGeo(FVector range[4], FVector position)
{
	m_Pos = position;
	m_Range[0] = range[0];
	
	map<double, FVector> dist;

	double distance[3] = { 0 };
	
	m_MinX = range[0].x;
	m_MaxX = range[0].x;
	m_MinY = range[0].y;
	m_MaxY = range[0].y;
	m_MinZ = range[0].z;
	m_MaxZ = range[0].z;

	for(UINT n=0;n<3;n++)
	{
		FVector& pos = range[n+1];
		dist[range[0].Distance(pos)] = pos;
		if(m_MinX > pos.x)
		{
			m_MinX = pos.x;
		}else if(m_MaxX < pos.x)
		{
			m_MaxX = pos.x;
		}
		if(m_MinY > pos.y)
		{
			m_MinY = pos.y;
		}else if(m_MaxY < pos.y)
		{
			m_MaxY = pos.y;
		}
		if(m_MinZ > pos.z)
		{
			m_MinZ = pos.z;
		}else if(m_MaxZ < pos.z)
		{
			m_MaxZ = pos.z;
		}
	}

	if(m_MinX > position.x)
	{
		m_MinX = position.x;
	}else if(m_MaxX < position.x)
	{
		m_MaxX = position.x;
	}
	if(m_MinY > position.y)
	{
		m_MinY = position.y;
	}else if(m_MaxY < position.y)
	{
		m_MaxY = position.y;
	}
	if(m_MinZ > position.z)
	{
		m_MinZ = position.z;
	}else if(m_MaxZ < position.z)
	{
		m_MaxZ = position.z;
	}
	UINT index = 1;
	for(map<double, FVector>::iterator it = dist.begin(); it != dist.end(); it++)
	{
		m_Range[index] = it->second;
		index++;
	}

	if(index == 4)
	{
		//succeed
	}else
	{
		//error
	}

	//generate normal for plane
	FVector point = m_Range[3];
	point.z = m_Pos.z;
	m_Normal = GetNormal(m_Range[0], m_Range[3], point, m_D);
}

void CDoorGeo::Init(FVector range[4], FVector position, UINT id)
{
	m_Pos = position;
	m_Range[0] = range[0];
	m_Id = id;
	
	map<double, FVector> dist;

	double distance[3] = { 0 };
	
	m_MinX = range[0].x;
	m_MaxX = range[0].x;
	m_MinY = range[0].y;
	m_MaxY = range[0].y;
	m_MinZ = range[0].z;
	m_MaxZ = range[0].z;

	for(UINT n=0;n<3;n++)
	{
		FVector& pos = range[n+1];
		dist[range[0].Distance(pos)] = pos;
		if(m_MinX > pos.x)
		{
			m_MinX = pos.x;
		}else if(m_MaxX < pos.x)
		{
			m_MaxX = pos.x;
		}
		if(m_MinY > pos.y)
		{
			m_MinY = pos.y;
		}else if(m_MaxY < pos.y)
		{
			m_MaxY = pos.y;
		}
		if(m_MinZ > pos.z)
		{
			m_MinZ = pos.z;
		}else if(m_MaxZ < pos.z)
		{
			m_MaxZ = pos.z;
		}
	}
	if(dist.size() < 3)
	{
		//make sure all ranges are unique
		range[1].z -= 50;
		range[2].z += 30;
		range[3].z -= 15;
		dist.clear();
		for(UINT n=0;n<3;n++)
		{
			FVector& pos = range[n+1];
			dist[range[0].Distance(pos)] = pos;
			if(m_MinX > pos.x)
			{
				m_MinX = pos.x;
			}else if(m_MaxX < pos.x)
			{
				m_MaxX = pos.x;
			}
			if(m_MinY > pos.y)
			{
				m_MinY = pos.y;
			}else if(m_MaxY < pos.y)
			{
				m_MaxY = pos.y;
			}
			if(m_MinZ > pos.z)
			{
				m_MinZ = pos.z;
			}else if(m_MaxZ < pos.z)
			{
				m_MaxZ = pos.z;
			}
		}
	}

	if(m_MinX > position.x)
	{
		m_MinX = position.x;
	}else if(m_MaxX < position.x)
	{
		m_MaxX = position.x;
	}
	if(m_MinY > position.y)
	{
		m_MinY = position.y;
	}else if(m_MaxY < position.y)
	{
		m_MaxY = position.y;
	}
	if(m_MinZ > position.z)
	{
		m_MinZ = position.z;
	}else if(m_MaxZ < position.z)
	{
		m_MaxZ = position.z;
	}
	
	UINT index = 1;
	for(map<double, FVector>::iterator it = dist.begin(); it != dist.end(); it++)
	{
		m_Range[index] = it->second;
		index++;
	}

	if(index == 4)
	{
		//succeed
	}else
	{
		//error
		g_Log.Add(CLog::Error, "[%s] Invalid door coordinates [%d]!", __FUNCTION__, id);
	}

	//generate normal for plane
	FVector point = m_Range[3];
	point.z = m_Pos.z;
	m_Normal = GetNormal(m_Range[0], m_Range[3], point, m_D);
}

bool CDoorGeo::InLineCollision(const FVector &p1, const FVector &p2, FVector& hit)
{
	//quick test to boost performance
	if (p2.x < m_MinX && p1.x < m_MinX) return false;
	if (p2.x > m_MaxX && p1.x > m_MaxX) return false;
	if (p2.y < m_MinY && p1.y < m_MinY) return false;
	if (p2.y > m_MaxY && p1.y > m_MaxY) return false;
	if (p2.z < m_MinZ && p1.z < m_MinZ) return false;
	if (p2.z > m_MaxZ && p1.z > m_MaxZ) return false;

	double u = (m_Normal.x*p1.x + m_Normal.y*p1.y + m_Normal.z*p1.z + m_D) / (m_Normal.x*(p1.x - p2.x) + m_Normal.y*(p1.y - p2.y) + m_Normal.z*(p1.z - p2.z));
	if(u >= 0.0 && u <= 1.0)
	{
		FVector r = p1;
		r = (p1 + (p2 - p1)* u);
		if(r.x >= m_MinX && r.x <= m_MaxX && r.y >= m_MinY && r.y <= m_MaxY && r.z >= m_MinZ && r.z <= m_MaxZ)
		{
			hit = r;
			return true;
		}
	}
	return false;
}

FVector CDoorGeo::GetNormal(FVector p, FVector q, FVector r, double& d)
{
	FVector pq = p.Join(q);
	FVector pr = p.Join(r);
	FVector n = pq.Cross(pr);
	d = n.x * (-p.x) + n.y * (-p.y) + n.z * (-p.z);
	return n;
}
