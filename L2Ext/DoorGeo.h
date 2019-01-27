#pragma once

class CDoorGeo
{
	FVector m_Normal;
	double m_D;	// ax +by +cz + d = 0
	FVector m_Range[4];
	double m_MinX;
	double m_MaxX;
	double m_MinY;
	double m_MaxY;
	double m_MinZ;
	double m_MaxZ;
	FVector m_Pos;
	UINT m_Id;

public:
	CDoorGeo();
	CDoorGeo(FVector range[4], FVector pos);
	void Init(FVector range[4], FVector pos, UINT id);
	bool InLineCollision(const FVector& p1, const FVector& p2, FVector& hit);
	FVector GetNormal(FVector p, FVector q, FVector r, double& d);
	inline const FVector& GetPos() { return m_Pos; };
};