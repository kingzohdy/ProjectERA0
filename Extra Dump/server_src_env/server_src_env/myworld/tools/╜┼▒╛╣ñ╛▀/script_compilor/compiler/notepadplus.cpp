//add by jessie
#include "mchead.h"

#define USERDEFINEFILE		"userDefineLang.xml"
#define USERDEFINETMPFILE	"userDefineLang_tmp.xml"
#define DOLOCALFILE			"doLocalConf.xml"
#define SHORTCUTFILE		"shortcuts.xml"

//' - !" % & ( ) * , . / : ; ? @ [ \ ] ^ { | } ~ + < = >
unsigned char operator_line[] = "- ! &quot; % &amp; ( ) * , . / : ? @ [ \\ ] ^ { | } ~ + &lt; = &gt;";

unsigned char comment_line[] = "1 2 0;";

unsigned char delimiter_line[] = "&quot;&apos;0&quot;&apos;0";

unsigned char file_begin_lines[][256] = 
{
	"<NotepadPlus>",
	"    <UserLang name=\"Game Macro\" ext=\"mxt\">",
	"        <Settings>",
	"            <Global caseIgnored=\"yes\"  />",
	"            <TreatAsSymbol comment=\"no\" commentLine=\"yes\" />",
	"            <Prefix words1=\"yes\" words2=\"no\" words3=\"no\" words4=\"no\" />",
	"        </Settings>",
	"        <KeywordLists>",
};

unsigned char file_mid_lines[][256] = 
{
	"            <Keywords name=\"Folder+\"></Keywords>",
	"            <Keywords name=\"Folder-\"></Keywords>",
};

unsigned char file_end_lines[][256] = 
{
	"            <Keywords name=\"Words3\"></Keywords>",
	"            <Keywords name=\"Words4\"></Keywords>",
	"        </KeywordLists>",
	"        <Styles>",
	"            <WordsStyle name=\"DEFAULT\" styleID=\"11\" fgColor=\"000000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"            <WordsStyle name=\"FOLDEROPEN\" styleID=\"12\" fgColor=\"000000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"            <WordsStyle name=\"FOLDERCLOSE\" styleID=\"13\" fgColor=\"000000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"            <WordsStyle name=\"KEYWORD1\" styleID=\"5\" fgColor=\"0080C0\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"1\" />",
	"            <WordsStyle name=\"KEYWORD2\" styleID=\"6\" fgColor=\"FF8000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"1\" />",
	"            <WordsStyle name=\"KEYWORD3\" styleID=\"7\" fgColor=\"000000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"            <WordsStyle name=\"KEYWORD4\" styleID=\"8\" fgColor=\"000000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"            <WordsStyle name=\"COMMENT\" styleID=\"1\" fgColor=\"000000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"            <WordsStyle name=\"COMMENT LINE\" styleID=\"2\" fgColor=\"008000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"            <WordsStyle name=\"NUMBER\" styleID=\"4\" fgColor=\"FF0000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"            <WordsStyle name=\"OPERATOR\" styleID=\"10\" fgColor=\"800040\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"1\" />",
	"            <WordsStyle name=\"DELIMINER1\" styleID=\"14\" fgColor=\"FF0080\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"2\" />",
	"            <WordsStyle name=\"DELIMINER2\" styleID=\"15\" fgColor=\"FF0080\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"2\" />",
	"            <WordsStyle name=\"DELIMINER3\" styleID=\"16\" fgColor=\"000000\" bgColor=\"FFFFFF\" fontName=\"\" fontStyle=\"0\" />",
	"        </Styles>",
	"    </UserLang>",
	"</NotepadPlus>",
};

unsigned char shortcut_file_lines[][256] = 
{
	"<NotepadPlus>",
	"    <UserDefinedCommands>",
	"         <Command name=\"Compier\" Ctrl=\"yes\" Alt=\"no\" Shift=\"yes\" Key=\"112\">compiler.exe \"$(FILE_NAME)\"</Command>",
	"    </UserDefinedCommands>",
	"</NotepadPlus>",
};

static int user_keyword[MAX_COMMAND];
static int user_max;
static int system_keyword[MAX_COMMAND];
static int system_max;

static void Get_Command_Keywords(char* buffer1, char* buffer2)
{

	int i, iIndex;
	char szUserCommand[(MAX_COMMAND+1)*(MAX_KEYWORD_LENGTH+1)];
	char szSysCommand[(MAX_COMMAND+1)*(MAX_KEYWORD_LENGTH+1)];
	memset(szUserCommand, 0, sizeof(szUserCommand));
	memset(szSysCommand, 0, sizeof(szSysCommand));
	user_max = 0;
	system_max = 0;
	for(i=0; i<MAX_COMMAND; i++)
	{
		user_keyword[i] = -1;
		system_keyword[i] = -1;
		if(command_table[i].flag == '1')
		{
			if(command_table[i].attrib == COMMAND_ATTRIB_USER)
			{
				user_keyword[user_max] = i;
				user_max++;
			}
			else if(command_table[i].attrib == COMMAND_ATTRIB_SYSTEM)
			{
				system_keyword[system_max] = i;
				system_max++;
			}
		}
	}

	for(i=0; i<user_max; i++)
	{
		iIndex = user_keyword[i];
		strcat((char*)szUserCommand, (const char*)command_table[iIndex].keyword);
		if(i != user_max-1)
			strcat((char*)szUserCommand, " ");
	}

	for(i=0; i<system_max; i++)
	{
		iIndex = system_keyword[i];
		strcat((char*)szSysCommand, (const char*)command_table[iIndex].keyword);
		strcat((char*)szSysCommand, " ");
	}
	strcat(szSysCommand, "#DEFINE");

	strcpy(buffer1, szUserCommand);
	strcpy(buffer2, szSysCommand);
}

int Write_Tmp_Config(char* filename)
{
	FILE *fp = NULL;
	int i;
	char buffer1[(MAX_COMMAND+1)*(MAX_KEYWORD_LENGTH+1)];
	char buffer2[(MAX_COMMAND+1)*(MAX_KEYWORD_LENGTH+1)];
	memset(buffer1, 0, (MAX_COMMAND+1)*(MAX_KEYWORD_LENGTH+1));
	memset(buffer2, 0, (MAX_COMMAND+1)*(MAX_KEYWORD_LENGTH+1));

	Get_Command_Keywords(buffer1, buffer2);
	remove(filename);
	if( (fp=fopen(filename, "a+")) == NULL )
		return -1;

	for(i=0; i<sizeof(file_begin_lines)/sizeof(file_begin_lines[0]); i++)
	{
		fprintf(fp, "%s\n", file_begin_lines[i]);
	}

	fprintf(fp, "            <Keywords name=\"Delimiters\">%s</Keywords>\n", delimiter_line);

	for(i=0; i<sizeof(file_mid_lines)/sizeof(file_mid_lines[0]); i++)
	{
		fprintf(fp, "%s\n", file_mid_lines[i]);
	}

	fprintf(fp, "            <Keywords name=\"Operators\">%s</Keywords>\n", operator_line);
	fprintf(fp, "            <Keywords name=\"Comment\">%s</Keywords>\n", comment_line);	
	fprintf(fp, "            <Keywords name=\"Words1\">%s</Keywords>\n", buffer1);
	fprintf(fp, "            <Keywords name=\"Words2\">%s</Keywords>\n", buffer2);

	for(i=0; i<sizeof(file_end_lines)/sizeof(file_end_lines[0]); i++)
	{
		fprintf(fp, "%s\n", file_end_lines[i]);
	}

	fflush(fp);
	fclose(fp);
	fp = NULL;
	return 0;
}

//same 0 different 1
int Compare_Cofig_Xml(char* filename, char* filename_tmp)
{
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	char* buffer1 = NULL;
	char* buffer2 = NULL;
	long size1, size2;
	int i;

	if( (fp2=fopen(filename_tmp, "r")) == NULL )
		goto error;
	
	if( (fp1=fopen(filename, "r")) == NULL )
	{
		fclose(fp2);
		fp2 = NULL;
		return 1;
	}

	fseek(fp1, 0L, SEEK_END);
	size1 = ftell(fp1);
	rewind(fp1);

	fflush(fp2);
	fseek(fp2, 0L, SEEK_END);
	size2 = ftell(fp2);
	rewind(fp2);
	printf("##%d %d\n", size1, size2);

	if(size1 != size2 )
	{
		fclose(fp1);
		fclose(fp2);
		fp1 = NULL;
		fp2 = NULL;
		return 1;
	}

	if( (buffer1=(char*)malloc(size1)) == NULL )
		goto error;

	if( (buffer2=(char*)malloc(size2)) == NULL )
		goto error;

	fread(buffer1, 1, size1, fp1);
	fread(buffer2, 1, size2, fp2);
	fclose(fp1); fp1 = NULL;
	fclose(fp2); fp2 = NULL;
		
	for(i=0; i<size1; i++)
	{
		if( buffer1[i] != buffer2[i] )
		{
			free(buffer1);
			free(buffer2);
			buffer1 = NULL;
			buffer2 = NULL;
			return 1;
		}
	}
	free(buffer1);
	free(buffer2);	
	buffer1 = NULL;
	buffer2 = NULL;
	return 0;

error:
	if( fp1 )	{ fclose(fp1); fp1=NULL; }
	if( fp2 )	{ fclose(fp2); fp2 = NULL; }
	if( buffer1 )	{ free(buffer1); buffer1=NULL; }
	if( buffer2 )	{ free(buffer2); buffer2=NULL; }
	return -1;
}

int Get_Compiler_File()
{
	FILE* fp = NULL;
	int i;
	char shortfile[512];
	memset(shortfile, 0, 512);
	sprintf((char*)shortfile, "%s%c%s", compiler_config.notepad_dir, DIR_SEPARATE_CHAR, SHORTCUTFILE);

	remove(shortfile);
	if( (fp=fopen(shortfile, "a+")) == NULL )
		return -1;

	for(i=0; i<sizeof(shortcut_file_lines)/sizeof(shortcut_file_lines[0]); i++)
	{
		fprintf(fp, "%s\n", shortcut_file_lines[i]);
	}

	fclose(fp);
	return 0;
}

int Config_NotPad_Xml()
{
	FILE *fp;
	int iFlag;
	char localfile[512];
	char filename[512];
	char filename_tmp[512];
	memset(localfile, 0, 512);
	memset(filename, 0, 512);
	memset(filename_tmp, 0, 512);
	sprintf((char*)localfile, "%s%c%s", compiler_config.notepad_dir, DIR_SEPARATE_CHAR, DOLOCALFILE);
	sprintf((char*)filename, "%s%c%s", compiler_config.notepad_dir, DIR_SEPARATE_CHAR, USERDEFINEFILE);
	sprintf((char*)filename_tmp, "%s%c%s", compiler_config.notepad_dir, DIR_SEPARATE_CHAR, USERDEFINETMPFILE);

	remove(localfile);
	if( (fp=fopen(localfile, "a+")) == NULL )
		return -1;
	fclose(fp);
	fp = NULL;
	
	Get_Compiler_File();
	Write_Tmp_Config(filename_tmp);
	if( (iFlag=Compare_Cofig_Xml(filename, filename_tmp)) >= 0 )
	{
		if( iFlag == 0)
			return 0;

		remove(filename);
		rename(filename_tmp, filename);

		return 0;
	}

	return -1;
}
