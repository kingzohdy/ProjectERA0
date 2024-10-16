#pragma once
#include <vector>

class CFileChipboard
{
public:
	CFileChipboard();
	~CFileChipboard(void);


	void CopyToChipboard(std::vector<CString>& files);
	void GetFromChipboard(std::vector<CString>& files);
};