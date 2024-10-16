#include <ctype.h>
#include <string.h>

#include "CharacterMap.h"
#include "SpecialCharacterMap.h"
#include "OIDirtyInterface.h"

int MakeMapCode(char * pszCharacter, char * pszSpellCode, char * pszPinyinCode, int iMaxCodeLenth)
{
	size_t iSrcLen = strlen(pszCharacter);
	size_t iMaxCodeCount = iMaxCodeLenth - 1;
	size_t i = 0, j = 0, iSpellCount = 0, iPinyinCount = 0;
	
	unsigned char cHiChar = 0, cLoChar = 0;
	int iIndex = -1, iTmpLen = 0;

	const char * pszTempCode = NULL;
	int fLoopFlag = 1;
	
	if ( (pszCharacter == NULL) || (pszSpellCode == NULL) || (pszPinyinCode == NULL) || (iMaxCodeLenth <= 0) )
	{//error input
		return -1;
	}

	while( (i < iSrcLen) && (fLoopFlag == 1) )
	{
		cHiChar = pszCharacter[i];
		if (cHiChar <= 128)
		{//ASCII�ַ������ֽ��ַ�
			iTmpLen = 1;
			if (iSpellCount + iTmpLen > iMaxCodeCount)
			{
				fLoopFlag = 0;
				continue;
			}
			pszSpellCode[iSpellCount++] = cHiChar;
			i++;
			continue;
		}
		
		if (i == iSrcLen - 1)
		{//���һ���ֽ�Ϊ�ǵ��ַ�����������
			i++;
			continue;
		}
		
		cLoChar = pszCharacter[i + 1];
		if( (cHiChar >= 129) && (cLoChar >= 64 ) )
		{//GBK�ַ���˫�ֽ��ַ�
			switch (cHiChar)
			{
			case 163://ȫ�ǣ��ӣãɣ��ַ�
				iTmpLen = 1;
				if (iSpellCount + iTmpLen > iMaxCodeCount)
				{
					fLoopFlag = 0;
					break;
				}

				pszSpellCode[iSpellCount++] = cLoChar - 128;
				break;
			case 162://��������
				if ( (cLoChar - 161 >=0) && (cLoChar - 161 < 94) )
				{
					iIndex = cLoChar - 161;
				}
				else
				{
					break;
				}

				pszTempCode =  CharIndex_Luoma[iIndex];
				if (pszTempCode != NULL)
				{
					iTmpLen = strlen(pszTempCode);
					if (iSpellCount + iTmpLen > iMaxCodeCount)
					{
						fLoopFlag = 0;
						break;
					}

					strcpy(&pszSpellCode[iSpellCount], pszTempCode);
					iSpellCount += iTmpLen;
				}
				
				break;
			case 166://ϣ����ĸ
				if ( (cLoChar > 0xa1) && (cLoChar < 0xb8) )
				{
					iIndex = cLoChar - 0xa1;
				}
				else if ( (cLoChar > 0xc1) && (cLoChar < 0xd8) )
				{
					iIndex = cLoChar - 0xc1;
				}
				else
				{
					break;
				}

				pszTempCode =  CharIndex_Xila[iIndex];
				if (pszTempCode != NULL)
				{
					iTmpLen = strlen(pszTempCode);
					if (iSpellCount + iTmpLen > iMaxCodeCount)
					{
						fLoopFlag = 0;
						break;
					}

					strcpy(&pszSpellCode[iSpellCount], pszTempCode);
					iSpellCount += iTmpLen;
				}
				break;
			case 167://����-�������ĸ
				if ( (cLoChar > 160) && (cLoChar < 194) )
				{
					iIndex = cLoChar - 161;
				}
				else if ( (cLoChar > 208) && (cLoChar < 242) )
				{
					iIndex = cLoChar - 209;
				}
				else
				{
					break;
				}

				pszTempCode = CharIndex_Xilier[iIndex];
				if (pszTempCode != NULL)
				{
					iTmpLen = strlen(pszTempCode);
					if (iSpellCount + iTmpLen > iMaxCodeCount)
					{
						fLoopFlag = 0;
						break;
					}

					strcpy(&pszSpellCode[iSpellCount], pszTempCode);
					iSpellCount += iTmpLen;
				}
				
				break;
			default://����
				if ( (cHiChar >= 255) || (cLoChar >= 255) )
				{
					break;
				}
				else
				{
					iIndex = g_alOIDirtyPinyinCodeIndex[cHiChar - 129][cLoChar - 64] - 1;
				}
				
				//����
				pszTempCode = g_aszOIDirtyPinyinCode[iIndex];
				if (pszTempCode != NULL)
				{
					iTmpLen = strlen(pszTempCode);
					if (iPinyinCount + iTmpLen > iMaxCodeCount)
					{
						fLoopFlag = 0;
						break;
					}

					strcpy(&pszPinyinCode[iPinyinCount], pszTempCode);
					iPinyinCount += iTmpLen;
				}

				//����
				if (iSpellCount + 2 > iMaxCodeCount)
				{
					fLoopFlag = 0;
					break;
				}
				pszSpellCode[iSpellCount++] = cHiChar;
				pszSpellCode[iSpellCount++] = cLoChar;

				break;
			}
		}
		else
		{
			if (iSpellCount + 2 > iMaxCodeCount)
			{
				fLoopFlag = 0;
				continue;
			}
			pszSpellCode[iSpellCount++] = cHiChar;
			pszSpellCode[iSpellCount++] = cLoChar;
		}

		i += 2;
	}

	pszSpellCode[iSpellCount] = 0;
	pszPinyinCode[iPinyinCount] = 0;
	
	for (j = 0; j < iSpellCount ; j++)
	{
		pszSpellCode[j] = tolower(pszSpellCode[j]);
	}
	for (j = 0; j < iPinyinCount; j++)
	{
		pszPinyinCode[j] = tolower(pszPinyinCode[j]);
	}

	if (fLoopFlag == 0)
	{//code buffer too short
		return 1;
	}

	return 0;
}

