#pragma once

namespace KeyEngine
{
	class CHardwareId
	{
		static const int SIZE = 8;
		char m_Part[SIZE];
	public:
		CHardwareId();
		CHardwareId(std::string hw);
		void generate();
		std::string toString();
		const char& operator[](size_t pos) const;
	};

	int32_t getCharIndex(char c);
	char getCharFromIndex(int32_t index);

	enum State
	{
		StateNone = 0,
		StateValid = 1,
		StateInvalid = 2
	};
}

class CKeyEngine
{
        CKeyEngine();
        CKeyEngine(const CKeyEngine& copy);
		CKeyEngine& operator = (const CKeyEngine& other);
		
		bool m_Loaded;
		std::string m_Name;
		std::string m_Key;
		KeyEngine::CHardwareId m_Hardware;
		KeyEngine::State m_State;
		mutable bool m_Validated;		

		void loadINI();
		void writeINI();
		bool isValid(std::string name, std::string key, const KeyEngine::CHardwareId& hardware) const;
    public:
        static CKeyEngine& getInstance();
		void init();
		bool validated() const { return m_Validated; }
		KeyEngine::State getState() const { return m_State; }
		void showInvalidKeyMessage();
};