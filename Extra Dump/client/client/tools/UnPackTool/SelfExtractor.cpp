/*************************************************************************************
*
*	File:		SelfExtracter.cpp
*	Version:	1.0
*
*	Author:		James Spibey
*	Date:		04/08/1999
*	E-mail:		spib@bigfoot.com
*
*	Implementation of the CSelfExtracter class
*
*	You are free to use, distribute or modify this code
*	as long as this header is not removed or modified.
*
*
*************************************************************************************/

#include "stdafx.h"
#include "SelfExtractor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*******************************************************************************
* 
* Function: CSelfExtractor::CSelfExtractor
* 
* Description: 
*	Default Constructor 
* 
* Parameters: 
*    None
* 
* Return: 
*    None
*******************************************************************************/
CSelfExtractor::CSelfExtractor()
{
	m_nFiles		= 0;
	m_nTOCSize		= 0;
}

/*******************************************************************************
* 
* Function: CSelfExtractor::~CSelfExtractor
* 
* Description: 
*	 Destructor 
* 
* Parameters: 
*    None
* 
* Return: 
*    None
*******************************************************************************/
CSelfExtractor::~CSelfExtractor()
{

}

/*******************************************************************************
* 
* Function: CSelfExtractor::Create
* 
* Description: 
*    Creates the Self-extracting executable 
* 
* Parameters: 
*    CString ExtractorPath: Path to the Extractor Executable 
*    CString Filename:		Filename of the Self Extracting Archive to create 
*	 funcPtr pFn:			Pointer to a user defined callback function
*	 void*   UserData:		User defined data to pass to the callback function
* 
* Return: 
*    int: Error Code
*		NOTHING_TO_DO		- No files have been selected to be archived
*		COPY_FAILED			- Failed to copy the extractor
*		OUTPUT_FILE_ERROR	- Failed to open the copied file for appending
*		INPUT_FILE_ERROR	- Failed to open an input file
*******************************************************************************/
int CSelfExtractor::Create(CString ExtractorPath, CString Filename, funcPtr pFn /* = NULL */, void* userData /*=NULL*/)
{
	//Make sure we have some files to add
	if(m_nFiles < 1)
		return NOTHING_TO_DO;

	//Copy the extractor to the new archive
	CShellFileOp shOp;
	shOp.SetFlags(FOF_FILESONLY | FOF_NOCONFIRMATION | FOF_SILENT);
	shOp.AddFile(SH_SRC_FILE, ExtractorPath);
	shOp.AddFile(SH_DEST_FILE, Filename);
	if(shOp.CopyFiles() != 0)
		return COPY_FAILED;

	//Open the archive
	CFile file;
	if(!file.Open(Filename, CFile::modeWrite))
		return OUTPUT_FILE_ERROR;
	else
	{
		//Start at the end of the archive
		file.SeekToEnd();
		CreateArchive(&file, pFn, userData);

		//Close the archive
		file.Close();
	}
	return SUCCESS;
}

/*******************************************************************************
* 
* Function: CSelfExtractor::Create
* 
* Description: 
*    Creates the Self-extracting executable from an extractor in resources.
*	 Simply import the exe into your resources, making sure you specify the type
*	 as "SFX_EXE". Then just past the resource ID as the first parameter
* 
* Parameters: 
*    UINT	 resource:		Resource ID (eg IDR_SFX_EXE)
*    CString Filename:		Filename of the Self Extracting Archive to create 
*	 funcPtr pFn:			Pointer to a user defined callback function
*	 void*   UserData:		User defined data to pass to the callback function
* 
* Return: 
*    int: Error Code
*		NOTHING_TO_DO		- No files have been selected to be archived
*		COPY_FAILED			- Failed to copy the extractor
*		OUTPUT_FILE_ERROR	- Failed to open the copied file for appending
*		INPUT_FILE_ERROR	- Failed to open an input file
*******************************************************************************/
int CSelfExtractor::Create(UINT resource, CString Filename, funcPtr pFn /* = NULL */, void* userData /*=NULL*/)
{
	//Make sure we have some files to add
	if(m_nFiles < 1)
		return NOTHING_TO_DO;

	//Load the extractor from resources
	HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(resource), "SFX_EXE");
	if(hrSrc == NULL)
		return RESOURCE_ERROR;

	HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);
	if(hGlobal == NULL)
		return RESOURCE_ERROR;

	LPVOID lpExe = LockResource(hGlobal);
	if(lpExe == NULL)
		return RESOURCE_ERROR;

	//Create the new archive from the extractor in the resources
	CFile file;
	if(!file.Open(Filename, CFile::modeCreate | CFile::modeWrite))
		return OUTPUT_FILE_ERROR;
	else
	{
		//Write the extractor exe
		file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));
		
		//Do the rest
		CreateArchive(&file, pFn, userData);

		//Close the archive
		file.Close();
	}
	return SUCCESS;
}

int CSelfExtractor::CreateArchive(CFile* pFile, funcPtr pFn, void* userData)
{
	DWORD dwRead = 0;	//Total Data read from input file
	DWORD dw = 0;		//Last amount read from input file
	char buffer[1000];	//Buffer for data
	CFile data;			//Input file
		
	try		
	{
		//Copy all the inout files into the archive
		for(int i = 0; i < m_nFiles; i++)
		{
			//Open the input file
			if(data.Open(m_InfoArray[i].GetPathname(), CFile::modeRead))
			{
				dwRead = 0;

				m_InfoArray[i].SetOffset((int)pFile->GetPosition());

				//Read data in and write it out until the end of file
				while(static_cast<int>(dwRead) < m_InfoArray[i].GetFileSize())
				{
					dw = data.Read(buffer, 1000);
					pFile->Write(buffer, dw);
					dwRead += dw;
				}

				//Close this input file
				data.Close();
				//Call the user defined CallBack
				if(pFn != NULL)
					pFn(static_cast<void*>(&m_InfoArray[i]), userData);
			}
			else
				return INPUT_FILE_ERROR;
		}

		//Now Write the TOC
		for(int j = 0; j < m_nFiles; j++)
		{	
			//Write the File Size
			int Offset = m_InfoArray[j].GetFileOffset();
			pFile->Write(&Offset, sizeof(int));

			//Write the File Size
			int len = m_InfoArray[j].GetFileSize();
			pFile->Write(&len, sizeof(int));
				
			//Write the filename
			len = m_InfoArray[j].GetFilename().GetLength();
			strncpy_s(buffer, m_InfoArray[j].GetFilename(), len);
			pFile->Write(buffer, len);
		
			//Write the length of the filename
			pFile->Write(&len, sizeof(int));

		}
		
		//Write the total number of files
		pFile->Write((void*)&m_nFiles, sizeof(int));
		
		//Write the SIG
		strcpy_s(buffer, SIGNATURE);
		pFile->Write(buffer, (UINT)strlen(SIGNATURE));
	}
	catch(CFileException* e)
	{
		//Got sick of seeing 'unreferenced local variable'
		e->m_cause;
		return OUTPUT_FILE_ERROR;
	}
	return SUCCESS;
}
/*******************************************************************************
* 
* Function: CSelfExtractor::ExtractAll
* 
* Description: 
*	 Extract the current archive to the specified directory 
* 
* Parameters: 
*    CString Dir: Destination Directory 
*	 funcPtr pFn:			Pointer to a user defined callback function
*	 void*   UserData:		User defined data to pass to the callback function
* 
* Return: 
*    int: Error Code
*			INPUT_FILE_ERROR	- Failed to open the input file
*			OUTPUT_FILE_ERROR	- Failed to create an output file
*******************************************************************************/
int CSelfExtractor::ExtractAll(CString Dir, CString ExeDir, funcPtr pFn /*= NULL*/, void * userData /*= NULL*/)
{
	//Make sure the directory name has a trailing backslash
	EnsureTrailingBackSlash(Dir);

	CFile Thisfile;		//Archive (Usually itself)

	//Read the Table of Contents
	int res = ReadTOC( ExeDir );
	if(res != SUCCESS)
		return res;
	
	//Open the archive
	if(!Thisfile.Open( ExeDir, CFile::modeRead))
		return INPUT_FILE_ERROR;
	else
	{

		//Get the files out in reverse order so we can work out the offsets
		//Subtract 1 from the filecount as we are zero-based
		for(int i = (m_nFiles - 1); i >= 0 ; i--)
		{
			ExtractOne(&Thisfile, i, Dir);
			
			//Do the callback
			if(pFn != NULL)
				pFn(static_cast<void*>(&m_InfoArray[i]), userData);
		}

		//Close the archive
		Thisfile.Close();
	}
	return SUCCESS;
}

/*******************************************************************************
* 
* Function: CSelfExtractor::Extract
* 
* Description: 
*	 Extract a single file from the current archive to the specified directory 
* 
* Parameters: 
*	 int index:		index in array of file
*    CString Dir:	Destination Directory 
* 
* Return: 
*    int: Error Code
*			INPUT_FILE_ERROR	- Failed to open the input file
*			OUTPUT_FILE_ERROR	- Failed to create an output file
*******************************************************************************/
int CSelfExtractor::Extract(int index, CString Dir)
{
	//Make sure the directory name has a trailing backslash
	EnsureTrailingBackSlash(Dir);

	CFile Thisfile;		//Archive (Usually itself)

	//Read the Table of Contents
	int res = ReadTOC(GetThisFileName());
	if(res != SUCCESS)
		return res;
	
	//Open the archive
	if(!Thisfile.Open(GetThisFileName(), CFile::modeRead))
		return INPUT_FILE_ERROR;
	else
	{
		ExtractOne(&Thisfile, index, Dir);

		//Close the archive
		Thisfile.Close();
	}
	return SUCCESS;
}

/*******************************************************************************
* 
* Function: CSelfExtractor::ExtractOne
* 
* Description: 
*	Actual Data Extraction. Seeks to required offset in archive 
*	and writes new file 
* 
* Parameters: 
*    CFile* file:	Pointer to the archive 
*    int index:		Index of file in array 
*    CString Dir:	Destination Dir 
* 
* Return: 
*    int: Error Code
*******************************************************************************/
int CSelfExtractor::ExtractOne(CFile* file, int index, CString Dir)
{
	char buffer[1000];	//Buffer to read and write with
	CFile NewFile;		//Extracted File
	
	//Get the file size (in bytes)
	int FileSize = m_InfoArray[index].GetFileSize();

	//Create the new file
	if(!NewFile.Open(Dir + m_InfoArray[index].GetFilename() , CFile::modeCreate | CFile::modeWrite))
		return OUTPUT_FILE_ERROR;
	
	//Seek to the correct Offset
	file->Seek(m_InfoArray[index].GetFileOffset(), CFile::begin);
		
	//Loop the data out from the archive
	DWORD dwWritten = 0;
	DWORD dwRead = 0;
	int AmountToRead = 0;

	while(TRUE)
	{
		//Read out 1000 bytes at a time or the remainder if
		//there is less than 1000 left. Exit if there is none left
		AmountToRead = FileSize - dwWritten;
		if(AmountToRead > 1000)
			AmountToRead = 1000;
		else if(AmountToRead == 0)
		break;

		dwRead = file->Read(buffer, AmountToRead);
		NewFile.Write(buffer, dwRead);
		dwWritten += dwRead;
	}
	//Close the output file
	NewFile.Close();

	return SUCCESS;
}

/*******************************************************************************
* 
* Function: CSelfExtractor::ReadTOC
* 
* Description: 
*	 Read the archive's Table of Contents 
* 
* Parameters: 
*    CString Filename: Filename of the archive (full path) 
* 
* Return: 
*    int: Error Code
*******************************************************************************/
int CSelfExtractor::ReadTOC(CString Filename)
{
	CFile Thisfile;		//Archive file
	char buffer[1000];	//Buffer to read and write with

	//Clear the CSEFileInfo class array
	Reset();
	
	//Open the archive
	if(!Thisfile.Open(Filename, CFile::modeRead))
		return NO_SOURCE;
	else
	{
		//Read in the signature
		Thisfile.Seek(- static_cast<int>(strlen(SIGNATURE)), CFile::end);
		Thisfile.Read(buffer, (UINT)strlen(SIGNATURE));

		//Check that it matches
		if(strncmp(buffer, SIGNATURE, strlen(SIGNATURE)) != 0)
			return INVALID_SIG;
		else
		{
			//Read Number of files
			int LastOffset = (int)strlen(SIGNATURE) + static_cast<int>(sizeof(int));
			Thisfile.Seek(-LastOffset, CFile::end);
			Thisfile.Read(&m_nFiles, sizeof(int));

			//If there are no files in the archive, there is nothing to extract
			if(m_nFiles == 0)
				return NOTHING_TO_DO;

			//Read the TOC in. The array is filled in reverse to ensure that it 
			//corresponds to the data segment
			for(int i = (m_nFiles - 1); i >= 0 ; i--)
			{
				int nSize	= 0;
				int nOffset = 0;
				int len		= 0;
				LastOffset += sizeof(int);
				
				//Get Length of Pathname
				Thisfile.Seek(-LastOffset, CFile::end);
				Thisfile.Read(&len, sizeof(int));
				LastOffset += len;
				
				//Get Path Name
				Thisfile.Seek(-LastOffset, CFile::end);
				Thisfile.Read(buffer, len);
				LastOffset += sizeof(int);

				//Get File Size
				Thisfile.Seek(-LastOffset, CFile::end);
				Thisfile.Read(&nSize, sizeof(int));
				LastOffset += sizeof(int);

				//Get File Offset
				Thisfile.Seek(-LastOffset, CFile::end);
				Thisfile.Read(&nOffset, sizeof(int));

				//Set the data in the array
				m_InfoArray[i].SetSize(nSize);
				CString Temp(buffer);
				m_InfoArray[i].SetFilename(Temp.Left(len));
				m_InfoArray[i].SetOffset(nOffset);
			}

			//Record the total size of the TOC for use 
			//when extracting the data segment
			m_nTOCSize = LastOffset;
		}
	}
	//Close the archive
	Thisfile.Close();

	return SUCCESS;
}

/*******************************************************************************
* 
* Function: CSelfExtractor::AddFile
* 
* Description: 
*	 Add a file to the archive 
* 
* Parameters: 
*    CString File: Input File path 
* 
* Return: 
*    BOOL: Success or Failure
*******************************************************************************/
BOOL CSelfExtractor::AddFile(CString File)
{
	if(m_nFiles == MAX_FILES)
		return FALSE;

	if(m_InfoArray[m_nFiles].SetData(File))
	{
		m_nFiles++;
		return TRUE;
	}
	
	return FALSE;
}


/*******************************************************************************
* 
* Function: CSelfExtractor::Reset
* 
* Description: 
*	 Reset the CSEFileInfo Array 
* 
* Parameters: 
*    None
* 
* Return: 
*    None
*******************************************************************************/
void CSelfExtractor::Reset()
{
	for(int i = 0; i < MAX_FILES; i++)
		m_InfoArray[i].Reset();

	m_nFiles	= 0;
	m_nTOCSize	= 0;
}

/*******************************************************************************
* 
* Function: CSelfExtractor::EnsureTrailingBackSlash
* 
* Description: 
*	 Ensure that the string has a trailing backslash 
* 
* Parameters: 
*    CString &string: Pathname 
* 
* Return: 
*    CString: Pathname
*******************************************************************************/
CString CSelfExtractor::EnsureTrailingBackSlash(CString &string)
{
	int len = string.GetLength();
	if(string[len - 1] != '\\')
		string += "\\";
	return string;
}

/*******************************************************************************
* 
* Function: CSelfExtractor::GetThisFileName
* 
* Description: 
*	 Get this executable's file path 
* 
* Parameters: 
*    None
* 
* Return: 
*    CString: Full Path for this executable
*******************************************************************************/
CString CSelfExtractor::GetThisFileName()
{
	char FullName[MAX_PATH+1];
	GetModuleFileName(NULL,FullName,MAX_PATH);
	return CString(FullName);
}
