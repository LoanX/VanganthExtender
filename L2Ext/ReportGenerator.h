
class CReportGenerator
{
	wstring wReport;
	wstring wBegin;
	wstring wEnd;
	wstring wLine;
	wstring wVariable;
	vector<wstring> vData;
public:
	inline CReportGenerator(wstring wTitle)
	{
		wBegin = L"<html><head><body><center><font color=\"LEVEL\">";
		wBegin += wTitle;
		wBegin += L"</font><br><br><table>";
		wEnd = L"</table></center></body></html>";
	}
	inline void StartLine()
	{
		wLine = L"<tr>";
	};
	inline void EndLine()
	{
		wLine += L"</tr>";
	};
	inline void SaveLine()
	{
		vData.push_back(wLine);
	}
	inline void AddVariable(wstring wVar)
	{
		wVariable = L"<td align=\"center\" width=80>";
		wVariable += wVar;
		wVariable += L"</td>";
		wLine += wVariable;
	}
	inline void AddVariable(int nVar)
	{
		WCHAR wVar[10];
		_itow(nVar, wVar, 10);
		wVariable = L"<td align=\"center\" width=80>";
		wVariable += wVar;
		wVariable += L"</td>";
		wLine += wVariable;
	}
	inline void AddVariable(double dVar)
	{
		WCHAR wVar[10];
		_itow((int)dVar, wVar, 10);
		wVariable = L"<td align=\"center\" width=80>";
		wVariable += wVar;
		wVariable += L"</td>";
		wLine += wVariable;
	}
	inline wstring Generate()
	{
		wReport += wBegin;
		for(int n=0;n<vData.size();n++)
		{
			wReport += vData[n];
			wcout << vData[n].c_str() << endl;
		}
		wReport += wEnd;
		return wReport;
	}
	inline wstring GetReport()
	{
		return wReport;
	}
};