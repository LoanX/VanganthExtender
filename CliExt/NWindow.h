#pragma once

class CNWindow
{
	BOOL m_Initialized;
public:
	CNWindow();
	~CNWindow();
	void Init();
};

extern CNWindow g_NWindow;