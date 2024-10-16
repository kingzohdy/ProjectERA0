/*
**  uedit.cpp
**  UltraEdit32 regular "wordfile.txt" syntax support.
**  
**  geaan, 2001.8.3.
*/
#include "mchead.h"



#define MAC_LANGUAGE_SCRIPT_FILENAME            "mcword.txt"
#define UEDIT32_LANGUAGE_SCRIPT_FILENAME        "WORDFILE.TXT"
#define BACKUP_UEDIT32_FILENAME                 "WORDFILE.GEA"


void    naked_write_langscript(FILE *fp);




unsigned char   language_lines[][256] = 
{
    "/L6\"Game Macro\" Line Comment = ; File Extensions = MXT",
    "/Delimiters = ~!@%%^&*()-+=|\\/{}[]:;\"\'<> ,	.?",
    "/Function String = \"%%[a-zA-Z_]*)[~;]\"",
    "/Indent Strings = ",
    "/Unindent Strings = ",
};

unsigned char   keyword_lines[][256] = 
{
    "/C1\"Keywords\"",
};
unsigned char   keyword_lines2[][256] = 
{
    "#DEFINE",
};

unsigned char   operator_lines[][256] = 
{
    "/C2\"Operators\"",
    "+",
    "-",
    "*",
    "// /",
    "@",
    "=",
    "!",
    ">",
    "<",
};


static  int    keyword_turn[MAX_COMMAND];
static  int    keyword_max;

static  void    sort_command_keyword(void)
{
    int i, j;
    int ikey, jkey;
    int comp_result;

    keyword_max = 0;
    for(i=0; i<MAX_COMMAND; i++)
    {
        keyword_turn[i] = -1;
        if(command_table[i].flag == '1')
        {
            keyword_turn[keyword_max] = i;
            keyword_max ++;
        }
    }

    for(i=0; i<keyword_max; i++)
    {
        for(j=i+1; j<keyword_max; j++)
        {
            ikey = keyword_turn[i];
            jkey = keyword_turn[j];
            comp_result = strcmpi((const char *)command_table[ikey].keyword, 
                (const char *)command_table[jkey].keyword);
            if(comp_result > 0)
            {
                keyword_turn[i] = jkey;
                keyword_turn[j] = ikey;
            }
        }
    }
}



void    naked_write_langscript(FILE *fp)
{
    int i, command_index;
    unsigned char   lead_code, now_code;

    if(!fp) return;

    // language
    for(i=0; i<sizeof(language_lines)/sizeof(language_lines[0]); i++)
    {
        fprintf(fp, (const char *)language_lines[i]);
        fprintf(fp, "\n");
    }
    //keyword
    for(i=0; i<sizeof(keyword_lines)/sizeof(keyword_lines[0]); i++)
    {
        fprintf(fp, (const char *)keyword_lines[i]);
        fprintf(fp, "\n");
    }
    sort_command_keyword();
    if(keyword_max > 0)
    {
        command_index = keyword_turn[0];
        lead_code = command_table[command_index].keyword[0];
        for(i=0; i<keyword_max; i++)
        {
            command_index = keyword_turn[i];
            now_code = command_table[command_index].keyword[0];
            if(lead_code != now_code)
            {
                fprintf(fp, "\n");
                lead_code = now_code;
            }
            fprintf(fp, "%s ", command_table[command_index].keyword);
        }
        fprintf(fp, "\n");
    }
    for(i=0; i<sizeof(keyword_lines2)/sizeof(keyword_lines2[0]); i++)
    {
        fprintf(fp, (const char *)keyword_lines2[i]);
        fprintf(fp, "\n");
    }

    //operator
    for(i=0; i<sizeof(operator_lines)/sizeof(operator_lines[0]); i++)
    {
        fprintf(fp, (const char *)operator_lines[i]);
        fprintf(fp, "\n");
    }
}


int update_ultraedit_langscript(void)
{
    FILE * fp = NULL, *fp2 = NULL;
    unsigned char   uedit32_filename[_MAX_PATH+_MAX_FNAME];
    unsigned char   backup_filename[_MAX_PATH+_MAX_FNAME];
    unsigned long   size;
    unsigned char   *buffer = NULL;
    //Jim, add, 2004.2.9.
    unsigned long last_offset;   
 

    sprintf((char *)backup_filename,"%s%c%s", compiler_config.uedit32_dir, 
        DIR_SEPARATE_CHAR, BACKUP_UEDIT32_FILENAME);
    sprintf((char *)uedit32_filename,"%s%c%s", compiler_config.uedit32_dir, 
        DIR_SEPARATE_CHAR, UEDIT32_LANGUAGE_SCRIPT_FILENAME);

    //(1) ready uedit32 file -----------------------------
    //backup the uedit32 file if it's not exist
    if(NULL == (fp = fopen((const char *)backup_filename, "rb")) )
    {
        if(NULL == (fp2 = fopen((const char*)uedit32_filename, "rb")) )
            goto error;
        
        //Jim, change,2004.2.9.
        //size = filelength(fileno(fp2));
        last_offset = ftell(fp2);
        fseek(fp2, 0L, SEEK_END);
        size = ftell(fp2);        
        fseek(fp2, last_offset, SEEK_SET);


        if(NULL == (buffer = (unsigned char *)malloc(size)) )
            goto error;
        memset(buffer, 0, size);
        fread(buffer, 1, size, fp2);
        if(fp2) { fclose(fp2); fp2 = NULL; }

        if(NULL == (fp = fopen((const char *)backup_filename, "wb")) )
            goto error;
        fwrite(buffer, 1, size, fp);
        if(fp) { fclose(fp); fp = NULL; }

        if(buffer) { free(buffer); buffer = NULL; }
    }
    //restore orig uedit32 file if there is a backup.
    else 
    {
        //Jim, change, 2004.2.9.
        //size = filelength(fileno(fp));
        last_offset = ftell(fp);
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp);
        fseek(fp, last_offset, SEEK_SET);

        if(NULL == (buffer = (unsigned char *)malloc(size)) )
            goto error;
        memset(buffer, 0, size);
        fread(buffer, 1, size, fp);
        if(fp) { fclose(fp); fp = NULL; }

        if(NULL == (fp2 = fopen((const char *)uedit32_filename, "wb")) )
            goto error;
        fwrite(buffer, 1, size, fp2);
        if(fp2) { fclose(fp2); fp2 = NULL; }

        if(buffer) { free(buffer); buffer = NULL; }
    }

    //(2) update uedit32 file for our language
    if(NULL == (fp = fopen((const char *)uedit32_filename, "a+")) )
        goto error;
    naked_write_langscript(fp);
    if(fp) { fclose(fp); fp = NULL; }

    return 0;
error:
    if(buffer) { free(buffer); buffer = NULL; }
    if(fp) { fclose(fp); fp = NULL; }
    if(fp2) {fclose(fp2); fp2 = NULL; }
    return -1;
}



int export_ultraedit_langscript(void)
{
    unsigned char   filename[_MAX_FNAME + _MAX_PATH];
    unsigned char   temp_filename[_MAX_FNAME + _MAX_PATH];
    FILE    *fp = NULL, *fp2 = NULL;
    int i;
    int replace_flag;
    unsigned char   *buffer1 = NULL, *buffer2 = NULL;
    int size1, size2;

    //Jim, add, 2004.2.9.
    unsigned long last_offset;
 

    //(1) create langscript file as a temp file ----------------------------------
    sprintf((char *)filename, "%s", MAC_LANGUAGE_SCRIPT_FILENAME);
    sprintf((char *)temp_filename, "%s", "$mcword$.$$$");
    remove((const char *)temp_filename);
    if(NULL == (fp = fopen((const char *)temp_filename, "a")) )
    {
        printf("error open temp file %s.\n", temp_filename);
        getch();
        goto error;
    }
    naked_write_langscript(fp);
    if(fp) fclose(fp);

    //(2) check if need to replace the file
    replace_flag = 0;
    fp2 = fopen((const char *)filename, "rb");
    if(fp2 == NULL)
        replace_flag = 1;
    else
    {
        //Jim, change, 2004.2.9.
        //size2 = filelength(fileno(fp2));
        last_offset = ftell(fp2);
        fseek(fp2, 0L, SEEK_END);
        size2 = ftell(fp2);
        fseek(fp2, last_offset, SEEK_SET);

        if(NULL == (buffer2 = (unsigned char *)malloc(size2)) )
            goto error;
        fread(buffer2, 1, size2, fp2);
        if(fp2) fclose(fp2);

        fp = fopen((const char *)temp_filename, "rb");
        
        //Jim, change, 2004.2.9.
        //size1 = filelength(fileno(fp));
        last_offset = ftell(fp);
        fseek(fp, 0L, SEEK_END);
        size1 = ftell(fp);
        fseek(fp, last_offset, SEEK_SET);

        if(NULL == (buffer1 = (unsigned char *)malloc(size1)) )
            goto error;
        fread(buffer1, 1, size1, fp);
        if(fp) fclose(fp);


        if(size1 != size2)
            replace_flag = 1;
        else
        {
            for(i=0; i<size1; i++)
            {
                if(buffer1[i] != buffer2[i])
                {
                    replace_flag = 1;
                    break;
                }
            }
        }
        if(buffer1) { free(buffer1); buffer1 = NULL; }
        if(buffer2) { free(buffer2); buffer2 = NULL; }
    }

    if(replace_flag)
    {
        remove((const char *)filename);
        rename((const char *)temp_filename, (const char *)filename);
    }
    else
    {
        remove((const char *)temp_filename);
    }
    return  0;
error:
    if(fp) { fclose(fp); fp = NULL; }
    if(fp2) { fclose(fp2); fp2 = NULL; }
    if(buffer1) { free(buffer1); buffer1 = NULL; }
    if(buffer2) { free(buffer2); buffer2 = NULL; }
    return -1;
}


