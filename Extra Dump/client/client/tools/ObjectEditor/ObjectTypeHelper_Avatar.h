
#pragma once
#include "ObjectTypeHelper.h"

class CAvatarCvtHelper : public CCvtHelper
{
public:
	unsigned int RaceNameToInt(LPTSTR raceName);
	char* IntToRaceName(unsigned int raceid);

	unsigned int GenderToInt(LPTSTR gender);
	char* IntToGender(unsigned int gender);

	unsigned int StringToItemQuality(LPTSTR quality);
	char* ItemQualityToString(unsigned int quality);
	COLORREF ItemQualityToColor(unsigned int quality);
	void EnumQuality(std::vector<char*>& arrQuality);

	int StringToItemType(LPTSTR itemtype);
	char* ItemTypeToString(int itemtype);
	void EnumItemType(std::vector<char*>& arrItemType);

	int StringToSlot(LPTSTR slot);
	int StringToSlot_en(LPTSTR slot);
	char* SlotToString(int slot);
	void EnumSlot(std::vector<char*>& arrSlot);

	bool TypeFitSlot(int type, int slot);
	virtual ~CAvatarCvtHelper(){};

	int ItemDspNameToId(LPTSTR dspname);
	char* IdToItemDspNameAsPtr(int id);
	CString IdToItemDspName(int id);

};