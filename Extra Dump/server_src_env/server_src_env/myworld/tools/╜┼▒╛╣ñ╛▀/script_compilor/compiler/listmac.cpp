/*
**  LISTMAC.C
**  List mac command function.
**  ZJian created,1999/01/29.
**
**  ZJian modified, 1999/06/01.
**    -- Add list number id
**  ZJian modified, 1999/06/03.
**    -- Change the list output device from console to file.
**  
**  ZJian modified, 2000/3/20.
**    -- Change mac_get_number to 4 bytes support.
**
**    kent modified, 2004.7.27.
**      -- Modified function mac_load_file() for the optimized mac head.	
*/
#include "mchead.h"

#define LIST_FILE_EXT   "lst"
#define mac_get_number  mac_get_number_s4

static  MAC_HEAD      * pmac_head               = NULL;
unsigned char         * mac_code                = NULL;
unsigned long int       mac_code_size           = 0;
char                    mac_filename[256]       ="noname";
unsigned char           system_number_id        = '#';
char                    list_filename[80];

int     mac_load_file(char *filename);
int     find_command_index(unsigned long code);
void    mac_list_command(void);
int     list_mac(char *filename);
int     get_total_proc(void);
void    myprintf(char *first,...);
unsigned char   mac_get_char(unsigned char *data,unsigned long int *index);
unsigned char * mac_get_string(unsigned char *data,unsigned long int *index);
unsigned char * get_op_string(unsigned char op);
unsigned long int       mac_get_long(unsigned char *data,unsigned long int *index);
unsigned short int      mac_get_number_u2(unsigned char *data,unsigned long int *index);
signed  long int        mac_get_number_s4(unsigned char *data,unsigned long int *index);
unsigned long mac_get_cmd_code(unsigned char *data, unsigned long int *index);
unsigned char * mac_get_usercmd_string(unsigned char *data,unsigned long int *index);

int list_mac(char *filename)
{
    change_ext(list_filename,filename,(char *)LIST_FILE_EXT);
    remove(list_filename);

    //Jim, TODO [5/9/2004]
    //{{{
    //mac_load_file(filename);
    //mac_list_command();
    //}}}

	//restored by kent, 2004.7.27
	mac_load_file(filename);
	mac_list_command();

    return(TTN_OK);
}

/*modified by kent, 2004.7.27, for the optimized mac head
*/
int mac_load_file(char *filename)
{
    FILE    *fp=NULL;
    int     mac_head_size;
    char    fullname[_MAX_PATH + _MAX_FNAME];
    
    // open mac file
    sprintf((char *)fullname, "%s%c%s", compiler_config.target_dir, DIR_SEPARATE_CHAR, filename);
    fp=fopen((const char *)fullname,"rb");
    if(fp==NULL)
    {
        sprintf(print_rec,"File %s open error !!!\n",filename);
        printf(print_rec);
        exit(-7);
        return(TTN_ERROR);
    }
    
    // try to get real mac head size
    if(pmac_head) { free(pmac_head); pmac_head=NULL; }
    pmac_head = (MAC_HEAD *)malloc(sizeof(*pmac_head));
    if(NULL==pmac_head)
    {
        printf("ERROR : memory alloc error!\n");
        exit(-1);
        return(TTN_ERROR);
    }
    memset(pmac_head,0,sizeof(*pmac_head));
    fread(pmac_head,1,sizeof(*pmac_head),fp);
    //mac_head_size = sizeof(*pmac_head)+(pmac_head->max_proc-1)*sizeof(signed long int);
	//added by kent 2004.7.27
	mac_head_size = sizeof(*pmac_head) - sizeof(signed long int) //size of base header
              + pmac_head->max_proc * (sizeof(unsigned short) + sizeof(signed long int));   //size of proc index-address pairs
    
    // reread mac head
    if(pmac_head) { free(pmac_head); pmac_head=NULL; }
    pmac_head = (MAC_HEAD *)malloc(mac_head_size);
    if(NULL==pmac_head)
    {
        printf("ERROR : memory alloc error!\n");
        exit(-1);
        return(TTN_ERROR);
    }
    memset(pmac_head,0,mac_head_size);
    fseek(fp,0,SEEK_SET);
    fread(pmac_head,1,mac_head_size,fp);
    
    // read mac codes
    mac_code_size = pmac_head->data_size;
    if(mac_code) { free(mac_code); mac_code=NULL; }
    mac_code = (unsigned char *)malloc(mac_code_size);
    if(NULL==mac_code)
    {
        printf("ERROR : memory alloc error!\n");
        exit(-1);
        return(TTN_ERROR);
    }
    memset(mac_code,0,mac_code_size);
    fread(mac_code,1,mac_code_size,fp);
    fclose(fp);
    strcpy((char *)mac_filename,(const char *)strupr((char *)filename));
    return(TTN_OK);
}

void    mac_list_command(void)
{
    char            disp_rec[1024];
    char            temp[1024];
    char            before[1024];
    unsigned long   command_code;
    int             command_len;
    int             command_index;
    int             pass;
    int             i;
    int             num1,num2,num3,num4,num5,num6;
    int             total_proc;
    int             lo_ver;
    int             hi_ver;
    int             insert_line;
    int             insert_blank;
    unsigned long int       ver;
    unsigned long int       code_offset;
    unsigned long int       get_offset;
    unsigned long int       offset;
    unsigned char           op;
    unsigned char           para_type;
    
    total_proc=get_total_proc();
    ver=pmac_head->version;
    lo_ver=ver & 0x0000ffff;
    hi_ver=((ver >>16)&0x0000fffff);
    myprintf("\n");
    myprintf("/*-------------------*/\n");
    myprintf("/* LIST  MAC  CODE   */\n");
    myprintf("/*-------------------*/\n");
    
    myprintf("MacroFileName : %s\n",mac_filename);
    myprintf("FileVersion   : %d.%d\n",hi_ver,lo_ver);
    myprintf("TotalProc     : %d\n",total_proc);
    myprintf("TotalCodeSize : %d\n",mac_code_size);
    
    myprintf("/*---------------------------------------------------------------------------*/\n");
    num1=num2=num3=num4=num5=num6=0;
    code_offset=0;
    pass=0;
    while(pass==0)
    {
        insert_line=0;
        insert_blank=4;
        
        memset(disp_rec,0x00,1024);
        sprintf(temp,"[%6d]",code_offset);
        strcat(disp_rec,temp);

        //Jim, change [2/18/2004]
        //command_code=mac_code[code_offset++];
        command_code = mac_get_cmd_code(mac_code, &code_offset);

	 if (command_code > MAX_SYS_CMD)
	 {
        	command_len = mac_code[code_offset] + mac_code[code_offset+1] * 0x100;
		code_offset += 2;	
	 }
	 else
	 {
        	command_len = mac_code[code_offset++];
	 }
	 
        sprintf(temp,"[%3d]",command_len);
        strcat(disp_rec,temp);
        strcpy(temp,"    ");
        strcat(disp_rec,temp);
        
        memset(before,0x00,1024);
        strcpy(before,disp_rec);
        memset(disp_rec,0x00,1024);
        
        get_offset=code_offset;
        command_index=find_command_index(command_code);
        if(command_index>=0)
        {
            switch(command_code)
            {
            case CMD_PROC:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d",system_number_id,num1);
                    strcat(disp_rec,temp);
                    insert_blank=0;
                    break;
                }
            case CMD_ENDPROC:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    insert_line=1;
                    insert_blank=0;
                    break;
                }
            case CMD_IF:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    offset=mac_get_long(mac_code,&get_offset);
                    sprintf(temp,"(%d) ",offset);
                    strcat(disp_rec,temp);
                    
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num1);
                    strcat(disp_rec,temp);
                    
                    op=mac_get_char(mac_code,&get_offset);
                    sprintf(temp,"%s ",get_op_string(op));
                    strcat(disp_rec,temp);
                    
                    num2=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num2);
                    strcat(disp_rec,temp);
                    
                    break;
                }
            case CMD_ELSE:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_ELSEIF:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    offset=mac_get_long(mac_code,&get_offset);
                    sprintf(temp,"(%d) ",offset);
                    strcat(disp_rec,temp);
                    
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num1);
                    strcat(disp_rec,temp);
                    
                    op=mac_get_char(mac_code,&get_offset);
                    sprintf(temp,"%s ",get_op_string(op));
                    strcat(disp_rec,temp);
                    
                    num2=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num2);
                    strcat(disp_rec,temp);
                    
                    break;
                }
            case CMD_ENDIF:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_WHILE:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_BREAK:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_ENDWHILE:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_SWITCH:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_CASE:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_ENDCASE:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_ENDSWITCH:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_GOTO:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    offset=mac_get_long(mac_code,&get_offset);
                    sprintf(temp,"(%d) ",offset);
                    strcat(disp_rec,temp);
                    
                    break;
                }
            case CMD_CALL:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    sprintf((char *)temp," %d",mac_get_number(mac_code,&get_offset));
                    strcat(disp_rec,temp);
                    break;
                }
            case CMD_RETURN:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    break;
                }
            case CMD_EXEC:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    sprintf((char *)temp," %s",mac_get_string(mac_code,&get_offset));
                    strcat(disp_rec, temp);

                    num1 = mac_get_number(mac_code,&get_offset);
                    sprintf((char *)temp, " %c%d", system_number_id, num1);
                    strcat(disp_rec,temp);
                    break;
                }
            case CMD_VAR:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num1);
                    strcat(disp_rec,temp);
                    
                    num2=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num2);
                    strcat(disp_rec,temp);
                    break;
                }
            case CMD_ADD:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num1);
                    strcat(disp_rec,temp);
                    
                    num2=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num2);
                    strcat(disp_rec,temp);
                    break;
                }
            case CMD_SUB:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num1);
                    strcat(disp_rec,temp);
                    
                    num2=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num2);
                    strcat(disp_rec,temp);
                    break;
                }
            case CMD_MUL:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num1);
                    strcat(disp_rec,temp);
                    
                    num2=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num2);
                    strcat(disp_rec,temp);
                    break;
                }
            case CMD_DEV:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num1);
                    strcat(disp_rec,temp);
                    
                    num2=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num2);
                    strcat(disp_rec,temp);
                    break;
                }
            case CMD_RAND:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    num1=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num1);
                    strcat(disp_rec,temp);
                    
                    num2=mac_get_number(mac_code,&get_offset);
                    sprintf(temp,"%c%d ",system_number_id,num2);
                    strcat(disp_rec,temp);
                    break;
                }
            default:
                {
                    strcat(disp_rec,(const char *)command_table[command_index].keyword);
                    strcat(disp_rec," ");
                    
                    for(i=0;i<MAX_PARAMETER;i++)
                    {
                        para_type=command_table[command_index].parameter_type[i];
                        switch(para_type)
                        {
                        case DUMMY_TYPE:
                            break;
                        case INT_TYPE:
                            num1=mac_get_number(mac_code,&get_offset);
                            sprintf(temp,"%c%d ",system_number_id,num1);
                            strcat(disp_rec,temp);
                            break;
                        case WORD_TYPE:
                            strcpy(temp,(const char *)mac_get_usercmd_string(mac_code,&get_offset));
                            strcat(disp_rec,temp);
                            strcat(disp_rec," ");
                            break;
                        case STR_TYPE:
                            strcpy(temp,(const char *)mac_get_usercmd_string(mac_code,&get_offset));
                            //strcat(disp_rec,temp);
                            strcat(disp_rec," ");
                            break;
                        }
                    }
                    break;
                }
            }
            sprintf(temp," ## %s",before);
            myprintf(temp);
            memset(temp,0x00,1024);
            memset(temp,' ',insert_blank);
            myprintf(temp);
            sprintf(print_rec," %s\n",disp_rec);
            myprintf(print_rec);
            if(insert_line==1)
                myprintf("\n");
        }
        else
        {
            sprintf(temp," ?? %s",before);
            myprintf(temp);
            memset(temp,0x00,1024);
            memset(temp,' ',insert_blank);
            myprintf(temp);
            sprintf(print_rec,"  UnknowCommand\n",disp_rec);
            myprintf(print_rec);
        }
        code_offset+=command_len;
        if(code_offset>=mac_code_size)
            pass=1;
    }
    myprintf("/*---------------------------------------------------------------------------*/\n");
    return;
}

int find_command_index(unsigned long code)
{
    int     i;
    int     index;
    
    index=-1;
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag=='1')
        {
            if(command_table[i].command_code==code)
            {
                index=i;
                i=MAX_COMMAND+100;
            }
        }
    }
    return(index);
}

unsigned short int      mac_get_number_u2(unsigned char *data,unsigned long int *index)
{
    static unsigned long int        ii;
    static unsigned short int       number;
    static unsigned char            number_id;
    
    ii=*index;
    number_id=data[ii+0];
    number=(unsigned short int)(data[ii+1]+data[ii+2]*0x100);
    ii=ii+3;
    *index=ii;
    system_number_id=number_id;
    return(number);
}

signed  long int      mac_get_number_s4(unsigned char *data,unsigned long int *index)
{
    static unsigned long int        ii;
    static signed long int          number;
    static unsigned char            number_id;
    
    ii=*index;
    number_id=data[ii+0];
    number=*(signed long int *)&data[ii+1];
    ii=ii+5;
    *index=ii;
    system_number_id=number_id;
    return(number);
}

unsigned long mac_get_cmd_code(unsigned char *data, unsigned long int *index)
{
    unsigned long int idx;
    unsigned long cmd_code;

    idx = *index;
    cmd_code = mac_code[idx] + mac_code[idx+1] * 0x100;
    *index = idx + 2;

    return cmd_code;
}

unsigned long int       mac_get_long(unsigned char *data,unsigned long int *index)
{
    static unsigned long int        ii;
    static unsigned long int        offset;
    
    ii=*index;
    offset=(unsigned long int)0;
    offset+=(unsigned char)data[ii+0];
    offset+=(unsigned char)data[ii+1]*0x100;
    offset+=(unsigned char)data[ii+2]*0x10000;
    offset+=(unsigned char)data[ii+3]*0x1000000;
    ii=ii+4;
    *index=ii;
    return(offset);
}

unsigned char * mac_get_string(unsigned char *data,unsigned long int *index)
{
    static unsigned char            buffer[256];
    static unsigned long int        ii;
    int             len;
    int             i;
    
    ii=*index;
    len=(int)data[ii];
    memset(buffer,0x00,256);
    for(i=0;i<len;i++)
        buffer[i]=data[ii+1+i];
    *index=ii+1+len;
    return((unsigned char *)buffer);
}

unsigned char * mac_get_usercmd_string(unsigned char *data,unsigned long int *index)
{
    static unsigned char            buffer[USER_CMD_STR_LEN];
    static unsigned long int        ii;
    int             len;
    int             i;
    
    ii=*index;
    len=(int)(data[ii] + data[ii+1]*0x100);
    memset(buffer,0x00,USER_CMD_STR_LEN);
    for(i=0;i<len;i++)
        buffer[i]=data[ii+1+i];
    *index=ii+2+len;
    return((unsigned char *)buffer);
}

unsigned char   mac_get_char(unsigned char *data,unsigned long int *index)
{
    unsigned long int       ii;
    static  unsigned char   cc;
    
    ii=*index;
    cc=data[ii];
    *index=ii+1;
    return(cc);
}

unsigned char * get_op_string(unsigned char op)
{
    char    op_word[12][8]={
        OP_WORD_EQ,
            OP_WORD_LT,
            OP_WORD_GT,
            OP_WORD_LE,
            OP_WORD_GE,
            OP_WORD_NE,
            OP_WORD_ADD,
            OP_WORD_SUB,
            OP_WORD_MUL,
            OP_WORD_DEV,
            OP_WORD_MOD,
            OP_WORD_POWER
    };
    unsigned char   op_id[12]={
        OP_ID_EQ,
            OP_ID_LT,
            OP_ID_GT,
            OP_ID_LE,
            OP_ID_GE,
            OP_ID_NE,
            OP_ID_ADD,
            OP_ID_SUB,
            OP_ID_MUL,
            OP_ID_DEV,
            OP_ID_MOD,
            OP_ID_POWER
    };
    int     i;
    static  unsigned char   op_str[8];
    
    memset(op_str,0x00,8);
    for(i=0;i<12;i++)
    {
        if(op==op_id[i])
        {
            strcpy((char *)op_str,op_word[i]);
            i=12+100;
        }
    }
    return(op_str);
}

int     get_total_proc(void)
{
    int     i;
    int     total;
    
	//added by kent, 2004.7.27, for the optimized mac head
	if (pmac_head->version >= 0x00060001)
		return pmac_head->max_proc;

    total=0;
    for(i=0;i<MAX_PROC;i++)
    {
        if(pmac_head->proc_address[i]>=0)
            total++;
    }
    return(total);
}

void    myprintf(char *first,...)
{
    FILE    *fp;
    va_list para;
    int     len,i;
    char    ch,ch1;
    char    line[1024];
    char    temp[4];
    char    str[256];
    int     number;
    int     index;
    
    memset(line,0x00,1024);
    memset(temp,0x00,4);
    index=0;
    va_start(para,first);
    len=strlen(first);
    for(i=0;i<len;i++)
    {
        ch=first[i];
        if(ch=='%')
        {
            ch1=first[++i];
            switch(ch1)
            {
            case 's':
                strcpy(str,va_arg(para,char *));
                strcat(line,str);
                break;
            case 'd':
                number=va_arg(para,int);
                
                //Jim, changed, 2004.2.9.
                //itoa(number,str,10);
                sprintf(str,"%d",number);

                strcat(line,str);
                break;
            case 'c':
                temp[0]=(char)va_arg(para,int);
                strcat(line,temp);
                break;
            default:
                printf("Error lead code in myprintf !!!\n");
                return;
                break;
            }
        }
        else
        {
            temp[0]=ch;
            strcat(line,temp);
        }
    }
    va_end(para);
    
    // printf(line);
    fp = fopen((const char *)list_filename,"a+");
    fputs((const char *)line,fp);
    fclose(fp);
}


