#include "proto_comm.h"
#include "resdb_meta.h"
#include "stdio.h"
#include "cs_net.h"

#define MAX_ITEM_DEF 6000
#define ITEM_DEF_DATA 1600
char sItemDef[MAX_ITEM_DEF][ITEM_DEF_DATA];

typedef struct
{
	int iItemID;
	int iPos;
}ClientItem;

int confuse_comp_func(const void * v1, const void *v2)
{
	return *((int *)v1) - *((int *)v2);
}

ITEMDEF *z_get_itemdef(int iItemID, int *piPos, char* pszBuff, int iMax, int iUnit)
{
	ITEMDEF *pstItemDef;
	ITEMDEF stItemDef;
	
	if(piPos && *piPos >=0 && *piPos <iMax)
	{
		pstItemDef = (ITEMDEF *)(pszBuff + (*piPos)*iUnit);
		if ((int)pstItemDef->ItemID == iItemID)
		{
			return pstItemDef;
		}
	}

	stItemDef.ItemID = iItemID;
	pstItemDef = (ITEMDEF *)bsearch(&stItemDef, pszBuff, iMax, iUnit, confuse_comp_func);
	if (pstItemDef && piPos)
	{
		*piPos = ((int )(pstItemDef) - (int)pszBuff)/iUnit;
	}

	return pstItemDef;
}

int main(int argc, char* argv[])
{
	int i;
	int iItemDef;
	ITEMDEF *pstItemDef;
	ClientItem stCltItem;

	
	stCltItem.iPos = -1;
	if(argc == 1)
	{
		stCltItem.iItemID = 1201601;
	}
	else if(argc == 2)
	{
		stCltItem.iItemID = atoi(argv[1]);
	}
	else 
	{
		printf("²ÎÊý´íÎó\n");
		return -1;
	}
	
	if(load_res_confuse_bin("../../cfg/res/csres/ItemDef.bin",sItemDef[0],ITEM_DEF_DATA,MAX_ITEM_DEF,&iItemDef) < 0)
	{
		return -1;
	}

	if( iItemDef > 1)
	{
		qsort(sItemDef[0], iItemDef, ITEM_DEF_DATA, confuse_comp_func);	
	}

	for(i=0;i<iItemDef;i++)
	{
		pstItemDef = (ITEMDEF *)(sItemDef[0]+i*ITEM_DEF_DATA);
		printf("%d %s\n",pstItemDef->ItemID,pstItemDef->Name);
	}

	pstItemDef = z_get_itemdef(stCltItem.iItemID, &stCltItem.iPos, sItemDef[0], iItemDef, ITEM_DEF_DATA);
	if(pstItemDef)
	{
		printf("search:%d %s\n",pstItemDef->ItemID,pstItemDef->Name);
	}
	
	return 0;
}
