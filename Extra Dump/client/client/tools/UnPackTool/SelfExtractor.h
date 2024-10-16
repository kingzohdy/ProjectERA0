/*************************************************************************************
*
*	File:		SelfExtracter.h
*	Version:	1.0
*
*	Author:		James Spibey
*	E-mail:		spib@bigfoot.com
*
*	Specification of the CSelfExtracter class
*
*	This code was based on suggestions from :-
*		Levente Farkas, Roger Allen, Günter (surname unknown)
*
*	You are free to use, distribute or modify this code
*	as long as this header is not removed or modified.
*
*	Self Extractor (SFX) File Format
*	---------------------------------
*	
*	Starting from the end of the archive and working backwards :-
*	
*	Header Info
*	10 bytes	Signature - Identifier for SFX archive
*	4 bytes		Number of files in archive
*	
*	Table of Contents
* 
*	Contains one record in the following format for each file
*	4 bytes		Length of filename
*	variable	Filename
*	4 bytes		Length of File	
*	4 bytes		Offset in archive to data
*
*	Data Segment
*	Each file is written (uncompressed) here in the order of the TOC
*
*	After this is the extractor executable.
*
*************************************************************************************/
#if !defined(AFX_SELFEXTRACTOR_H__849C04B2_4988_11D3_A8BC_0050043A01C0__INCLUDED_)
#define AFX_SELFEXTRACTOR_H__849C04B2_4988_11D3_A8BC_0050043A01C0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SEFileInfo.h"
#include "ShellFileOp.h"

#define NO_SOURCE			1000
#define INVALID_SIG			1001
#define SUCCESS				1002
#define COPY_FAILED			1003
#define NOTHING_TO_DO		1004
#define OUTPUT_FILE_ERROR	1005
#define INPUT_FILE_ERROR	1006
#define RESOURCE_ERROR		1007

#define MAX_FILES			256
#define SIGNATURE			"!LYME_SFX!"

typedef void (* funcPtr)(void *, void*);

class CSelfExtractor  
{
public:
	CSelfExtractor();
	virtual ~CSelfExtractor();

	//Creation
	int Create(CString ExtractorPath, CString FilePath, funcPtr function = NULL, void * userData = NULL);
	int Create(UINT Resource, CString FilePath, funcPtr function = NULL, void * userData = NULL);
	BOOL AddFile(CString Filename);

	//Extraction
	int Extract(int index, CString Dir);
	int ExtractAll(CString dir, CString ExeDir, funcPtr function = NULL, void * userData = NULL);
	int ReadTOC(CString Filename);

	//Data retrieval
	inline int GetFileSize(int index){return m_InfoArray[index].GetFileSize();}
	inline int GetFileCount(){return m_nFiles;}
	CSEFileInfo GetItem(int item){return m_InfoArray[item];}

	//Helpers
	void Reset();

protected:
	//Helpers
	CString GetThisFileName();							
	CString EnsureTrailingBackSlash(CString &string);
	
	int CreateArchive(CFile* fp, funcPtr function, void* userData);

	int		ExtractOne(CFile* fp, int index, CString Dir);
	
	//Data
	CString				m_strWorkingDir;
	CSEFileInfo			m_InfoArray[MAX_FILES];		//Array of file information
	int					m_nFiles;					//No of files in archive
	int					m_nTOCSize;					//Size of Table of contents
};

#endif // !defined(AFX_SELFEXTRACTOR_H__849C04B2_4988_11D3_A8BC_0050043A01C0__INCLUDED_)
