#pragma once

class CMacAddress
{
	BYTE p[6];
public:
	CMacAddress(void);
	CMacAddress(string sMac);
	~CMacAddress(void);
	string ToString();
	bool operator==(const CMacAddress &other) const;
	bool operator!=(const CMacAddress &other) const;
};
