/*
**  READINI.C
**  Read macro command define file.
**  Copyright(C) 1999, TTN Info. Co.,Ltd.
*/
#ifdef  WIN32
#include <windows.h>
#endif//WIN32
#include "mchead.h"

//#define PRINT_KEYCHECK          1
#define DEFAULT_KEYCHECK        1761820543

//define this for debug loaded ini information
//#define DEBUG_INI

COMPILER_CONFIG         compiler_config;
MC_INFORMATION          mc_information;
COMMAND_TABLE           command_table[MAX_COMMAND];
LINE_DNODE            * export_start = NULL;
LINE_DNODE            * export_end   = NULL;
LINE_DNODE            * inport_line  = NULL;
int                     copyright_change_flag = 0;

void    read_ini_info(char *filename);
void    init_export_start(void);
void    init_export_end(void);
void    init_inport_line(void);
void    init_compiler_config(void);
void    init_mc_information(void);
void    init_command_table(void);
void    clear_inport_line(void);
int     find_group_section(char *id,LINE_DNODE *ss,LINE_DNODE **tt);
int     load_ini_file(unsigned char *filename);
void    read_information();
void    read_config();
void    read_system_command();
void    read_user_command();
void    read_export_start();
void    read_export_end();
void    skip_compartment(unsigned char *data,int *index);
void    get_word(char *in,char *out,int *idx,int max);
void    get_string(char *in,char *out,int *idx,int max);
signed long int word_to_int(char *data);
void    debug_read_ini_info(void);
int     get_unregister_command(void);
int     check_ini_info(void);
void    found_uedit32_dir(void);
//add by jessie begin
void	found_notepad_dir(void);
//add by jessie end
int     is_command_exist(char *cmd_keyword);


int     load_ini_file(unsigned char *filename)
{
    FILE    *fp;
    int     ret,i,len;
    char    line[LINE_BUFFER_SIZE];
    LINE_DNODE      *p,*s;
    
    ret=TTN_OK;
    fp=fopen((const char *)filename,"rb");
    if(fp==NULL)
    {
        printf("File %s open error!!!\n",filename);
        exit(-2);
    }
    p=inport_line;
    while(!feof(fp))
    {
        memset(line,0x00,LINE_BUFFER_SIZE);
        fgets(line,LINE_BUFFER_SIZE-1,fp);
        if(feof(fp))
            break;
        len=strlen(line);
        for(i=0;i<len;i++)
        {
            if(line[i]==0x0A || line[i]==0x0D)
                line[i]=0x00;
        }
        s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(s==NULL)
        {
            printf("Memory alloc error!!!\n");
            exit(-1);
        }
        s->last=NULL;
        s->next=NULL;
        strcpy((char *)s->buffer,(const char *)line);
        
        p->next=s;
        s->last=p;
        p=p->next;
    }
    fclose(fp);
    return(ret);
}

void    init_export_start(void)
{
    export_start=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
    if(export_start==NULL)
    {
        printf("Memory alloc error!!!\n");
        exit(-1);
    }
    export_start->last=NULL;
    export_start->next=NULL;
    memset(export_start->buffer,0x00,LINE_BUFFER_SIZE);
    return;
}

void    init_export_end(void)
{
    export_end=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
    if(export_end==NULL)
    {
        printf("Memory alloc error!!!\n");
        exit(-1);
    }
    export_end->last=NULL;
    export_end->next=NULL;
    memset(export_end->buffer,0x00,LINE_BUFFER_SIZE);
    return;
}

void    init_compiler_config(void)
{
    compiler_config.list_code='0';
    compiler_config.data_file='0';
    compiler_config.export_header='0';
    compiler_config.test_target='0';
    compiler_config.log_error = '0';
    compiler_config.log_langscript = '0';
    strcpy((char *)compiler_config.target_dir,".");
    strcpy((char *)compiler_config.header_dir,".");
    strcpy((char *)compiler_config.uedit32_dir, ".");
	//add by jessie
	strcpy((char *)compiler_config.notepad_dir, ".");
    return;
}

void    init_mc_information(void)
{
    memset(mc_information.copyright,0x00,80);
    strcpy((char *)mc_information.copyright,
        "Macro File Compiler(V5.0),Copyright(C) 3SSoft,ZJian.");
    mc_information.version=0x00050000;
    strcpy((char *)mc_information.file_ext,"MAC");
    mc_information.key_check=DEFAULT_KEYCHECK;      // 0x19750411 ^ mask(blocks)
    mc_information.remark=';';
    mc_information.label=':';
    mc_information.variable='@';
    mc_information.number='#';
    strcpy((char *)mc_information.define,"#DEFINE");
	strcpy((char *)mc_information.strvar,"STRVAR");
    return;
}

void    init_command_table(void)
{
    int     i,j;
    for(i=0;i<MAX_COMMAND;i++)
    {
        command_table[i].flag='0';
        command_table[i].attrib=COMMAND_ATTRIB_UNKNOW;
        command_table[i].command_code=0xFFFF;
        strcpy((char *)command_table[i].keyword,"UNKNOWKEYWORD");
        for(j=0;j<MAX_PARAMETER;j++)
            command_table[i].parameter_type[j]=DUMMY_TYPE;
    }
    return;
}

void    init_inport_line(void)
{
    inport_line=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
    if(inport_line==NULL)
    {
        printf("Memory alloc error!!!\n");
        exit(-1);
    }
    inport_line->last=NULL;
    inport_line->next=NULL;
    memset(inport_line->buffer,0x00,LINE_BUFFER_SIZE);
    return;
}

void    clear_inport_line(void)
{
    LINE_DNODE      *p,*s;
    p=inport_line;
    while(p->next!=NULL)
        p=p->next;
    while(p->last!=NULL)
    {
        s=p;
        p=p->last;
        if(s!=NULL)
            free(s);
    }
}

void    read_ini_info(char *filename)
{
    init_export_start();
    init_export_end();
    init_inport_line();
    init_compiler_config();
    init_mc_information();
    init_command_table();
    load_ini_file((unsigned char *)filename);
    read_information();
    read_config();
    read_system_command();
    read_user_command();
    read_export_start();
    read_export_end();
    clear_inport_line();
    if(copyright_change_flag==1)
    {
        if(check_ini_info()!=TTN_OK)
        {
            printf("\n\n");
            printf("INVALID COPYRIGHT INFORMATION !!!\n");
            printf("SYSTEM ABORT !!!\n");
            printf("%c%c%c\n",0x07,0x07,0x07);
            exit(-10);
        }
    }

#ifdef	DEBUG_INI
    debug_read_ini_info();
#endif//DEBUG_INI
}


int     find_group_section(char *id,LINE_DNODE *ss,LINE_DNODE **tt)
{
    int             lines;
    unsigned char   buffer[1024];
    unsigned char   temp[256];
    int             index;
    int             find;
    LINE_DNODE      *p;
    
    lines=0;
    find=0;
    p=ss;
    if(p==NULL)
        return(lines);
    while(p->next!=NULL)
    {
        p=p->next;
        
        index=0;
        memset(buffer,0x00,1024);
        strcpy((char *)buffer,(const char *)p->buffer);
        skip_compartment(buffer,&index);
        get_string((char *)buffer,(char *)temp,&index,255);
        if(temp[0]=='[')
        {
            if(strcmpi(strupr((char *)id),strupr((char *)temp))==0)
            {
                find=1;
                *tt=p;
                lines=0;
            }
            else
            {
                if(find==1)
                    break;
            }
        }
        else
        {
            if(find==1)
                lines++;
        }
    }
    return(lines);
}


void    skip_compartment(unsigned char *data,int *index)
{
    int             pass;
    static int      ii;
    
    pass=0;
    ii=*index;
    while(pass==0)
    {
        switch(data[ii])
        {
        case ' ':
        case ',':
        case '=':
        case 0x09:
            ii++;
            break;
        default:
            *index=ii;
            pass=1;
            break;
        }
    }
}


void    get_word(char *in,char *out,int *idx,int max)
{
    static int      ii,jj;
    int             in_len;
    char            ch;
    int             pass;
    
    in_len=strlen(in);
    pass=0;
    for(jj=0,ii=*idx;ii<in_len;ii++)
    {
        ch=in[ii];
        if(ch==' '||ch==','||ch=='='||ch==0x09)
        {
            out[jj]=(char)NULL;
            *idx=ii;
            pass=1;
        }
        else
        {
            out[jj]=in[ii];
            jj=jj+1;
            if(jj>=max-1)
            {
                out[jj]=0x00;
                pass=1;
            }
        }
        if(pass==1)
            break;
    }
    out[jj]=0x00;
    *idx=ii;
}


void    get_string(char *in,char *out,int *idx,int max)
{
    static int      ii,jj;
    int             in_len;
    char            ch;
    int             pass;
    
    in_len=strlen(in);
    pass=0;
    for(jj=0,ii=*idx;ii<in_len;ii++)
    {
        ch=in[ii];
        if(ch==0x00)
        {
            out[jj]=(char)NULL;
            *idx=ii;
            pass=1;
        }
        else
        {
            out[jj]=in[ii];
            jj=jj+1;
            if(jj>=max-1)
            {
                out[jj]=0x00;
                pass=1;
            }
        }
        if(pass==1)
            break;
    }
    out[jj]=(char)NULL;
    *idx=ii;
}

void    read_information(void)
{
    LINE_DNODE      *p;
    int             lines;
    int             line_index;
    int             index;
    char            buffer[1024];
    char            temp[256];
    char            temp1[256];
    
    lines=find_group_section(GROUP_ID_INFORMATION,inport_line,&p);
    line_index=0;
    while(p->next != NULL)
    {
        p=p->next;
        
        index=0;
        memset(buffer,0x00,1024);
        memset(temp,0x00,256);
        strcpy((char *)buffer,(const char *)p->buffer);
        skip_compartment((unsigned char *)buffer,&index);
        get_word(buffer,temp,&index,255);
        if((temp[0]!=0x00) && (temp[0]!=';'))
        {
            if(strcmpi(strupr(ITEM_ID_COPYRIGHT),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_string(buffer,temp1,&index,255);
                strcpy((char *)mc_information.copyright,(const char *)temp1);
                copyright_change_flag=1;
            }
            else if(strcmpi(strupr(ITEM_ID_VERSION),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                mc_information.version=(unsigned char)word_to_int(temp1);
            }
            else if(strcmpi(strupr(ITEM_ID_FILEEXTENSION),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                strcpy((char *)mc_information.file_ext,(const char *)temp1);
            }
            else if(strcmpi(strupr(ITEM_ID_KEYCHECK),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                mc_information.key_check=word_to_int(temp1);
            }
            else
            {
            }
        }
        line_index++;
        if(line_index>=lines)
            break;
    }
    return;
}

void    read_config(void)
{
    LINE_DNODE      *p;
    int             lines;
    int             line_index;
    int             index;
    char            buffer[1024];
    char            temp[256];
    char            temp1[256];

    found_uedit32_dir();
	found_notepad_dir();
    
    lines=find_group_section(GROUP_ID_CONFIG,inport_line,&p);
    line_index=0;
    while(p->next != NULL)
    {
        p=p->next;
        
        index=0;
        memset(buffer,0x00,1024);
        memset(temp,0x00,256);
        strcpy(buffer,(const char *)p->buffer);
        skip_compartment((unsigned char *)buffer,&index);
        get_word(buffer,temp,&index,255);
        if((temp[0]!=0x00) && (temp[0]!=';'))
        {
            if(strcmpi(strupr(ITEM_ID_DATAFILE),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    compiler_config.data_file=temp1[0];
            }
            else if(strcmpi(strupr(ITEM_ID_LISTCODE),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    compiler_config.list_code=temp1[0];
            }
            else if(strcmpi(strupr(ITEM_ID_EXPORTHEADER),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    compiler_config.export_header=temp1[0];
            }
            else if(strcmpi(strupr(ITEM_ID_TESTTARGET),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    compiler_config.test_target=temp1[0];
            }
            else if(strcmpi(strupr(ITEM_ID_LOGERROR),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    compiler_config.log_error=temp1[0];
            }
            else if(strcmpi(strupr(ITEM_ID_LOGLANGSCRIPT),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    compiler_config.log_langscript=temp1[0];
            }
            else if(strcmpi(strupr(ITEM_ID_TARGETDIR),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_string(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    strcpy((char *)compiler_config.target_dir, (const char *)temp1);
            }
            else if(strcmpi(strupr(ITEM_ID_HEADERDIR),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_string(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    strcpy((char *)compiler_config.header_dir, (const char *)temp1);
            }
            else if(strcmpi(strupr(ITEM_ID_UEDIT32DIR),strupr(temp))==0)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_string(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    strcpy((char *)compiler_config.uedit32_dir, (const char *)temp1);
            }
			//add by jessie
            else if(strcmpi(strupr(ITEM_ID_NOTEPADDIR),strupr(temp))==0)
            {
				memset(temp1, 0x00, 256);
                skip_compartment((unsigned char *)buffer,&index);
                get_string(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    strcpy((char *)compiler_config.notepad_dir, (const char *)temp1);
            }
        }
        line_index++;
        if(line_index>=lines)
            break;
    }
    return;
}

void    read_system_command()
{
    LINE_DNODE      *p;
    int             lines;
    int             line_index;
    int             index;
    char            buffer[1024];
    char            temp[256];
    char            temp1[256];
    int             cmd_index;
    int             i;
    
    lines=find_group_section(GROUP_ID_SYSTEM_COMMAND,inport_line,&p);
    line_index=0;
    cmd_index=0;
    while(p->next != NULL)
    {
        p=p->next;
        
        index=0;
        memset(buffer,0x00,1024);
        memset(temp,0x00,256);
        strcpy(buffer,(const char *)p->buffer);
        
        skip_compartment((unsigned char *)buffer,&index);
        get_word(buffer,temp,&index,255);
        if((temp[0]!=0x00)&&(temp[0]!=';'))
        {
            cmd_index=get_unregister_command();
            if(cmd_index < 0)
            {
                printf("Fatal error: Command overflow(>%d) !\n",MAX_COMMAND);
                exit(-4);
            }

            if(is_command_exist((char *)temp))
            {
                printf("Fatal error: System command %s redefine ! \n", temp);
                exit(-5);
            }

            command_table[cmd_index].flag='1';
            command_table[cmd_index].attrib=COMMAND_ATTRIB_SYSTEM;
            strcpy((char *)command_table[cmd_index].keyword,(const char *)strupr((char *)temp));
            
            memset(temp1,0x00,256);
            skip_compartment((unsigned char *)buffer,&index);
            get_word(buffer,temp1,&index,255);
            command_table[cmd_index].command_code=(unsigned long)word_to_int(temp1);
            
            for(i=0; i<MAX_PARAMETER; i++)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    command_table[cmd_index].parameter_type[i]=temp1[0];
            }
        }
        line_index++;
        if(line_index>=lines)
            break;
    }
    return;
}

void    read_user_command()
{
    LINE_DNODE      *p;
    int             lines;
    int             line_index;
    int             index;
    char            buffer[1024];
    char            temp[256];
    char            temp1[256];
    int             cmd_index;
    int             i;
    
    lines=find_group_section(GROUP_ID_USER_COMMAND,inport_line,&p);
    line_index=0;
    while(p->next != NULL)
    {
        p=p->next;
        
        index=0;
        memset(buffer,0x00,1024);
        memset(temp,0x00,256);
        strcpy(buffer,(const char *)p->buffer);
        
        skip_compartment((unsigned char *)buffer,&index);
        get_word(buffer,temp,&index,255);
        if((temp[0]!=0x00)&&(temp[0]!=';'))
        {
            //read command keyword
            cmd_index = get_unregister_command();

            if(cmd_index < 0)
            {
                printf("Fatal error: Command overflow(>%d) !\n",MAX_COMMAND);
                exit(-4);
            }

            if(is_command_exist((char *)temp))
            {
                printf("Fatal error: User command %s redefine ! \n", temp);
                exit(-5);
            }


            command_table[cmd_index].flag='1';
            command_table[cmd_index].attrib=COMMAND_ATTRIB_USER;
            strcpy((char *)command_table[cmd_index].keyword,(const char *)strupr((char *)temp));
            
            //read command code
            memset(temp1,0x00,256);
            skip_compartment((unsigned char *)buffer,&index);
            get_word(buffer,temp1,&index,255);
            command_table[cmd_index].command_code=(unsigned long)word_to_int(temp1);

            //read command parameter type
            for(i=0; i<MAX_PARAMETER; i++)
            {
                memset(temp1,0x00,256);
                skip_compartment((unsigned char *)buffer,&index);
                get_word(buffer,temp1,&index,255);
                if(temp1[0]!=0x00)
                    command_table[cmd_index].parameter_type[i]=temp1[0];
            }
        }
        line_index++;
        if(line_index>=lines)
            break;
    }
    return;
}

void    read_export_start()
{
    LINE_DNODE      *p,*t,*s;
    int             lines;
    int             line_index;
    
    lines=find_group_section(GROUP_ID_EXPORT_START,inport_line,&p);
    line_index=0;
    t=export_start;
    while(p->next!=NULL)
    {
        p=p->next;
        
        s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(s==NULL)
        {
            printf("Memory alloc error!!!\n");
            exit(-1);
        }
        s->last=NULL;
        s->next=NULL;
        strcpy((char *)s->buffer,(const char *)p->buffer);
        
        t->next=s;
        s->last=t;
        t=t->next;
        
        line_index++;
        if(line_index>=lines)
            break;
    }
    return;
}

void    read_export_end()
{
    LINE_DNODE      *p,*t,*s;
    int             lines;
    int             line_index;
    
    lines=find_group_section(GROUP_ID_EXPORT_END,inport_line,&p);
    line_index=0;
    t=export_end;
    while(p->next!=NULL)
    {
        p=p->next;
        
        s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(s==NULL)
        {
            printf("Memory alloc error!!!\n");
            exit(-1);
        }
        s->last=NULL;
        s->next=NULL;
        strcpy((char *)s->buffer,(const char *)p->buffer);
        
        t->next=s;
        s->last=t;
        t=t->next;
        
        line_index++;
        if(line_index>=lines)
            break;
    }
    return;
}

signed long int word_to_int(char *data)
{
    signed long int number;
    int             len;
    int             i,index;
    int             multi_base;
    int             nn;
    char            ch;
    
    len=strlen(data);
    multi_base=10;
    index=0;
    if(len>2)
    {
        if( (data[0]=='0') && ((data[1]=='x')||(data[1]=='X')) )
        {
            multi_base=16;
            index=2;
        }
    }
    number=0;
    nn=0;
    if(multi_base==10)
    {
        for(i=index;i<len;i++)
        {
            ch=data[i];
            nn=0;
            switch(ch)
            {
            case '0':case '1':case '2':case '3':case '4':
            case '5':case '6':case '7':case '8':case '9':
                nn=ch-'0';
                break;
            default:
                printf("Number data error!!!\n");
                exit(-3);
                break;
            }
            number=number*multi_base+nn;
        }
    }
    else if(multi_base==16)
    {
        for(i=index;i<len;i++)
        {
            ch=data[i];
            nn=0;
            switch(ch)
            {
            case '0':case '1':case '2':case '3':case '4':
            case '5':case '6':case '7':case '8':case '9':
                nn=ch-'0';
                break;
            case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':
                nn=ch-'a'+10;
                break;
            case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':
                nn=ch-'A'+10;
                break;
            default:
                printf("Number data error!!!\n");
                exit(-3);
                break;
            }
            number=number*multi_base+nn;
        }
    }
    else
    {
    }
    return(number);
}

int     get_unregister_command(void)
{
    int     index;
    int     i;
    index=-1;
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag=='0')
        {
            index=i;
            i=MAX_COMMAND+100;
        }
    }
    return(index);
}


int     is_command_exist(char *cmd_keyword)
{
    int     exist_flag;
    int     i;

    exist_flag = 0;
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag=='1')
        {
            if(0 == strcmpi((const char *)cmd_keyword, (const char*)command_table[i].keyword))
            {
                exist_flag = 1;
                break;
            }
        }
    }
    return  exist_flag;
}

void    debug_read_ini_info(void)
{
    int             i, j;
    LINE_DNODE      *p;
    unsigned char   buf[256];
    
    printf("==========================================================\n");
    printf("mc_information.copyright = %s\n",mc_information.copyright);
    printf("mc_information.version   = 0x%0x\n",mc_information.version);
    printf("mc_information.file_ext  = %s\n",mc_information.file_ext);
    printf("mc_information.key_check = %d\n",mc_information.key_check);
    printf("----------------------------------------------------------\n");
    printf("compiler_config.list_code     = %c\n",compiler_config.list_code);
    printf("compiler_config.data_file     = %c\n",compiler_config.data_file);
    printf("compiler_config.export_header = %c\n",compiler_config.export_header);
    printf("compiler_config.test_target   = %c\n",compiler_config.test_target);
    printf("compiler_config.log_error     = %c\n",compiler_config.log_error);
    printf("compiler_config.log_langscript= %c\n",compiler_config.log_langscript);
    printf("compiler_config.target_dir    = %s\n",compiler_config.target_dir);
    printf("compiler_config.header_dir    = %s\n",compiler_config.header_dir);
    printf("compiler_config.uedit32_dir   = %s\n",compiler_config.uedit32_dir);
    printf("==========================================================\n");
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag=='1')
        {
            memset(buf, '\0', 256);
            for(j=0; j<MAX_PARAMETER; j++)
                buf[j] = command_table[i].parameter_type[j];

            printf("command[%d] code[0x%0x] keyword[%s] parameter[%s]\n",
                i,
                command_table[i].command_code,
                command_table[i].keyword,
                buf
                );
        }
    }
    printf("==========================================================\n");
    printf("\n\n\n\n");
    
    printf("EXPORT_START:\n");
    p=export_start;
    while(p->next!=NULL)
    {
        p=p->next;
        printf((const char *)p->buffer);
        printf("\n");
    }
    
    printf("EXPORT_END:\n");
    p=export_end;
    while(p->next!=NULL)
    {
        p=p->next;
        printf((const char *)p->buffer);
        printf("\n");
    }
}

int     check_ini_info(void)
{
    unsigned long   right_keycheck;
    int             i;
    int             len;
    int             total;
    unsigned long   keycheck;
    unsigned long   mask;
    unsigned long   check;
    unsigned char   temp[256];
    
    memset(temp,0x00,256);
    strcpy((char *)temp,(const char *)mc_information.copyright);
    right_keycheck=0;
    check=0x19750411;
    len=strlen((const char *)temp);
    total=len/4;
    for(i=0;i<total;i++)
    {
        mask=(unsigned long)(*(unsigned long *)&temp[i*4]);
        check=check ^ mask;
    }
    right_keycheck=check;
    keycheck=mc_information.key_check;
    
#ifdef  PRINT_KEYCHECK
    printf("\n\nKeyCheck = %d\n",right_keycheck);
    printf("\n\nKeyCheck = 0x%08x\n",right_keycheck);
#endif
    
    if(keycheck==right_keycheck)
        return(TTN_OK);
    return(TTN_NOT_OK);
}


void    found_uedit32_dir(void)
{
#ifdef  WIN32
    static unsigned char    shell_str[_MAX_PATH];
    unsigned long   slRet;
    unsigned long   dwlength;
    unsigned long   dwDataType;
    HKEY  hReadKey;

    slRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\UEDIT32.EXE",
        0,
        KEY_READ,
        &hReadKey);

    if(slRet == ERROR_SUCCESS)
    {
        dwlength = sizeof(shell_str);
        memset(shell_str,0,dwlength);
        dwlength = _MAX_PATH;
        RegQueryValueEx(hReadKey, "Path", NULL, &dwDataType, shell_str, &dwlength);
        RegCloseKey( hReadKey );

        strcpy((char *)compiler_config.uedit32_dir, (const char *)shell_str);
    }
#endif//WIN32
}

void    found_notepad_dir(void)
{
#ifdef  WIN32
    static unsigned char    shell_str[_MAX_PATH];
	char *p;
    unsigned long   slRet;
    unsigned long   dwlength;
    unsigned long   dwDataType;
    HKEY  hReadKey;

    slRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\NOTEPAD++.EXE",
        0,
        KEY_READ,
        &hReadKey);

    if(slRet == ERROR_SUCCESS)
    {
        dwlength = sizeof(shell_str);
        memset(shell_str,0,dwlength);
        dwlength = _MAX_PATH;
        RegQueryValueEx(hReadKey, "", NULL, &dwDataType, shell_str, &dwlength);
        RegCloseKey( hReadKey );
		p = strrchr((char*)shell_str, '\\');
        strncpy((char *)compiler_config.notepad_dir, (char*)shell_str, strlen((char*)shell_str)-strlen(p));
		printf("***%s***\n", compiler_config.notepad_dir);
    }
#endif//WIN32
}


/******************************************************************************/
/*
void    main(void)
{
read_ini_info("MC50.INI");
debug_read_ini_info();
}
*/

