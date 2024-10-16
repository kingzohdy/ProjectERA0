/*
**  OUTDATA.C
**  Output command table data functions.
**
**  ZJian,1999/02/01.
*/
#include "mchead.h"

#define COMMAND_DATA_VERSION    0x40
#define COMMAND_DATA_FILENAME   "Macrocmd.dat"

COMMAND_HEAD    command_head;

void    init_command_head(void);
void    output_command_data_file(void);

void    init_command_head(void)
{
    memset(command_head.copyright,0x00,75);
    strcpy((char *)command_head.copyright,"TTN Macro Command Table,Copyright(C) 1999,TTN Infomation Co.,Ltd.");
    command_head.copyright[74]=0x1a;
    command_head.version=COMMAND_DATA_VERSION;
    command_head.key_check=0x19750411;
}

void    output_command_data_file(void)
{
    FILE    *fp;
    init_command_head();
    fp=fopen((const char *)COMMAND_DATA_FILENAME,"wb");
    if(fp==NULL)
    {
        printf("Output command data error!!!\n");
        exit(-11);
    }
    fwrite(&command_head,1,sizeof(struct COMMAND_HEAD_STRUCT),fp);
    fwrite(&command_table,1,sizeof(struct COMMAND_TABLE_STRUCT)*MAX_COMMAND,fp);
    fclose(fp);
    printf("Output command data file %s OK !\n",COMMAND_DATA_FILENAME);
    printf("\n");
}


