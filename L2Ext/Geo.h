#pragma once

class CGeo
{
	CGeo(){}
	CGeo(const CGeo &);
	CGeo& operator=(const CGeo&);
public:
	static CGeo* GetInstance();
	bool CanSee(FVector* Pos1, FVector* Pos2);
	int GetZLoc(int x, int y, int z = 16000);
};