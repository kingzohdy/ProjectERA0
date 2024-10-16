/*
**    mcmake.cpp
**    macro file compiler.
**  
**    ZJian created  , 1998.6.15.
**
**    ZJian modified , 1999.1.26.
**       -- Add output command data file .
**
**    ZJian modified , 1999.5.31.
**       -- Modify export header functions(switch by compiler_config.export_header)
**
**    ZJian modified , 1999.6.3.
**       -- Add @no (value stored in variable) surpport
**       exclude the following command:
**          IF [@|#]no1 ...
**          ELSEIF [@|#]no1 ...
**          WHILE [@|#]no1 ...
**          SWITCH [@|#]no1
**          VAR [@|#]no1 ...
**          ADD [@|#]no1 ...
**          SUB [@|#]no1 ...
**          MUL [@|#]no1 ...
**          DEV [@|#]no1 ...
**          RAND [@|#]no1 ...
**       The no1 is always compiled as number which point to
**       a mc variable in interpreter process.
**          
**    ZJian modified , 1999.8.16.
**       -- Modified the corporation copyright to SanChi Group.
**
**    ZJian modified , 1999.9.29.
**       -- Modified the copyright function and add title routines.
**          Corporation  : SANCHI Software.
**          Title        : Macro File Compiler.
**       -- Add defines for the above messages.
**       
**    ZJian modified , 2000.3.20.     -> (Ver5.0)
**       -- Change number(2 bytes) to number(4 bytes)
**          Change : put_number_to_target => put_number_u2_to_target
**          Change : put_number_to_target => #define ...
**          Add    : put_number_s4_to_target
**             (u2 is unsigned short int)
**             (s4 is signed long int)
**
**    ZJian modified, 2000.3.29.
**       -- Change mac variable from signed short int
**          to signed long int
**       -- Change struct MAC_INFO & MAC_HEAD for compatible with
**          ..\include\windll.h
**
**    ZJian modified, 2001.4.27.
**       -- Add a element of "unsigned long head_size" to MAC_HEAD struct,
**          and modified the init_mac_head() function for it.
**       -- Change show_usage() function for use argv[0].
**       -- Change show_title() function for use MAC_VERSION.
**
**    ZJian modified, 2001.5.21.
**       -- Add directory specify in CMD.INI for compiled target files & "MCDEF.H".
**       -- Update the MAC_VERSION to 5.21.
**
**    ZJian modified, 2001.7.9.
**       -- Add LogError specify in CMD.INI for log out error & warnings to file
**          "MCOUT.TXT".
**
**    ZJian modified, 2001.7.28.
**       -- Changed the MAX_PARAMETER from 6 (before) to 12.
**       -- Adjust error of read source file.
**          Before we can't read the last line of a file for we check feof() and 
**          break. Now we process the line first, and at the end of our loop check
**          feof().
**       -- Update the MAC_VERSION to 5.22.
**
**    ZJian modified, 2001.8.3.
**       -- Add UltraEdit language script support for our macro source file.
**       -- When we compile some source file, it will generate a text file which
**          named "mcword.txt". It is the format for UltraEdit.
**       -- We can append it to the "WORDFILE.TXT"(at UltraEdit's working path).
**       -- PS: must use ".MXT" for our macro source file extension.
**
**    ZJian modified, 2001.8.24.
**       -- For easy use, we found the UltraEdit's registry information for it's working path.
**          RegDB Key: SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\UEDIT32.EXE
**          RegDB Val: C:\Program Files\UltraEdit\UEDIT32.EXE
**          RegDB Path: C:\Program Files\UltraEdit
**          we can use these for our auto appending language script.
**       -- Added compiler_config.uedit32_dir for found & save UltraEdit's working path.
**       -- Update the MAC_VERSION to 5.23
**
**    ZJian modified, 2001.11.20.
**       -- Changed the MAX_PARAMETER from 12 (before) to 32.
**       -- Update the MAC_VERSION to 5.24
**
**    ZJian bug fixed, 2002.3.15.
**       -- When compile some command with invalid parameters, there is a 
**          "Invalid char at #" error report without line number.
**          Add line number to error report.
**       -- Update the MAC_VERSION to 5.25.
**
**    ZJian bug fixed, 2002.3.19.
**       -- In function skip_space(...), we skipped 3 characters before. 
**          They are space(' '), tab(0x09), and comma(','). But now we use 2 chars exclude comma.
**          If we are processing a string type parameter, and the string is ",", we can successfully
**          compile ok now.
**       -- Update the MAC_VERSION to 5.26.
**
**    ZJian modified, 2002.12.25.
**       -- Modified the MAX_PROC from 1024 to 20480 for "Psalm of Sea" zone server.
**       -- Fixed bug for support Chine filename.
**       -- Update the MAC_VERSION to 5.27.
**
**    ZJian modified, 2003.1.21.
**       -- Fixed bug for didn't display warning message when found same command lead string in 
**          command define ini file(thanks HawkeSch found this bug!).
**       -- Update the MAC_VERSION to 5.28.
**
**
**    ZJian modified, 2004.2.18.
**      -- Add function encode_cmd_code() for command code encoding.
**      -- Change cmd code from unsigned char(1byte) to unsigned short(2bytes).
**      -- Change EXEC arguments from 1(str) to 2(str + num), means mac_fname and proc_no.
**      -- Add function get_source_number(), get_source_op().
**      -- Update the MAC_VERSION to 6.0.
**
**    ZJian modified, 2004.5.9.
**      -- Modified script target file header for reduce header size.
**      -- Update the MAC_VERSION to 6.1.
**  
**    kent modified, 2004.7.23.
**      -- Modified function cmd_if() for NOT AND OR logic expression.
**
**      -- Modified export module in order to remove the bottleneck that macros exported in the
**         mcdef.h file is 1-byte hex. Now, 2-byte hex macors born ... we have more room now.
*/
#include "mchead.h"

/*added by kent for logic expression, 2004.7.23*/
#include "parselogicexp.h"


#define put_number_to_target    put_number_s4_to_target

#define MESSAGE_COPYRIGHT       "Copyright(C) 1999-2002, Share Software.\nAll rights reserved."
#define MESSAGE_TITLE           "Macro File Compiler Ver%d.%d\n"
#define MESSAGE_AUTHOR          "Jack created, 1998.6.15.\nLast update, 2004.2.18.\n"
#define MESSAGE_USAGE           "Usage:\n      %s  source_filename [target_filename]\n\n"
#define MESSAGE_DUMP_LINE       "****************************************************************************"
#define INI_INFO_FILENAME       "cmd.ini"
#define MAC_VERSION             0x00060002  //ver 6.2
#define MAC_KEYCHECK            0x19750411
#define LOGERROR_FILENAME       "mcout.txt"

#define TINY_COMPILE            1

#define CONVENE_UNKNOWN         0
#define CONVENE_END             1
#define CONVENE_COMPARTMENT     2
#define CONVENE_OPERATOR        3
#define CONVENE_NUMBER          4
#define CONVENE_CHAR            5

#define SYMBOL_NONE             0
#define SYMBOL_IDENTIFY         1
#define SYMBOL_OPERATOR         2
#define SYMBOL_SYSTEM           3
#define SYMBOL_UNKNOWN          9
#define SYSTEM_STRVAR_BEGIN          900
#define MAX_SYSTEM_STRVAR          100

#define COMPILE_OK              1
#define COMPILE_NOT_OK          2

#define INVALID_PROC_ADDRESS    -1


typedef struct  DEFINE_TABLE_STRUCT
{
    char flag;
    char name[256];
    char data[256];
} DEFINE_TABLE;

typedef struct  LABEL_TABLE_STRUCT
{
    char flag;
    char name[256];
    signed long int offset;
} LABEL_TABLE;

typedef struct  GOTO_TABLE_STRUCT
{
    char flag;
    char name[256];
    signed long int offset_address;
} GOTO_TABLE;

typedef struct  CALL_TABLE_STRUCT
{
    char flag;
    int  source_line_number;
    int  call_proc;
} CALL_TABLE;

typedef struct  IF_TABLE_STRUCT
{
    signed long int      offset;
    signed long int      elseif_offset[MAX_ELSEIF];
    signed long int      break_offset[MAX_ELSEIF];
    signed long int      else_offset;
    signed long int      max_elseif;
    signed long int      find_else;
} IF_TABLE;

typedef struct  SWITCH_TABLE_STRUCT
{
    signed long int number;
    signed long int case_offset[MAX_CASE];
    signed long int endcase_offset[MAX_CASE];
    signed long int max_case;
    signed long int default_offset;
    signed long int find_case;
} SWITCH_TABLE;

typedef struct  WHILE_TABLE_STRUCT
{
    signed long int      offset;
    signed long int      break_offset[MAX_BREAK];
    signed long int      start;
    signed long int      max_break;
} WHILE_TABLE;


LINE_DNODE    * source_line             = NULL;
OUTPUT_DNODE  * compile_output          = NULL;
char            source_filename[80]     = "NONAME";
char            target_filename[80]     = "NONAME";
char            print_rec[LINE_BUFFER_SIZE];
char            line_buffer[LINE_BUFFER_SIZE];

MAC_HEAD      * pmac_head=NULL;
DEFINE_TABLE    define_table[MAX_DEFINE];
LABEL_TABLE     label_table[MAX_LABEL];
IF_TABLE        if_table[MAX_IF];
SWITCH_TABLE    switch_table[MAX_SWITCH];
WHILE_TABLE     while_table[MAX_WHILE];
GOTO_TABLE      goto_table[MAX_GOTO];
CALL_TABLE      call_table[MAX_CALL];

int             system_if_index         = 0;
int             system_switch_index     = 0;
int             system_while_index      = 0;
int             system_goto_index       = 0;
int             system_call_index       = 0;
int             system_line_number      = 0;
int             system_source_size      = 0;
int             system_target_size      = 0;
int             system_variable_max     = 0; //store max variable at the source
int             proc_start_flag         = 0;
unsigned char * target_code             = NULL;
unsigned long int target_offset         = 0;
int             system_strvar_number      = 0;

void    init_source_line(void);
void    load_source_file(char *filename);
void    init_compiler(void);
void    change_ext(char *target,char *filename,char *ext);
void    show_usage(char *first_arg);
void    show_copyright(void);
void    show_title( unsigned long version);
void    show_author(void);
void    show_dump_line(void);
void    show_skip_line(void);
int     mc_main(void);
void    skip_space(char *data,int *index);
int     get_symbol(char *in,char *out,int *idx,int max);
void    init_compile_output(void);
void    output_message(char flag,int line_number,char *message);
int     pass1_check(void);
int     pass2_check(void);
int     pass3_check(void);
int     is_remark(char *data);
int     is_define(char *data);
int     is_command(char *data);
int     is_label(char *data);
int		is_strvar(char *data);
int     is_system_command(char *keyword);
int     check_char_convene(char ch);
int     register_define(char *name,char *data);
int     register_label(char *name);
int     register_proc(int index,unsigned long int start_offset);
int     register_goto(char *label_name,signed long int offset_address);
int     register_call(int call_proc,int line_number);
void    update_label(char *name,unsigned long int offset);
void    change_define(char *in,char *out);
int     print_compile_output(void);
void    init_mac_head(unsigned long int max_proc);
void    init_target_code(void);
int     search_command_index(char *keyword);
int     search_label_index(char *name);
int     convert_system_command(char *data);
int     convert_user_command(char *data);
int     put_number_u2_to_target(unsigned char id,unsigned short int number);
int     put_number_s4_to_target(unsigned char id,signed long int number);
int     put_word_to_target(char *data);
int     put_string_to_target(char *data);
int     put_op_to_target(unsigned char op_id);
int     create_target_file(void);
void    replace_define(char *in,char *out);
int     cmd_proc(char *data,int *index);
int     cmd_endproc(char *data,int *index);
int     cmd_if(char *data,int *index);
int     cmd_else(char *data,int *index);
int     cmd_elseif(char *data,int *index);
int     cmd_endif(char *data,int *index);
int     cmd_while(char *data,int *index);
int     cmd_break(char *data,int *index);
int     cmd_endwhile(char *data,int *index);
int     cmd_switch(char *data,int *index);
int     cmd_case(char *data,int *index);
int     cmd_endcase(char *data,int *index);
int     cmd_endswitch(char *data,int *index);
int     cmd_goto(char *data,int *index);
int     cmd_call(char *data,int *index);
int     cmd_return(char *data,int *index);
int     cmd_exec(char *data,int *index);
int     cmd_var(char *data,int *index);
int     cmd_add(char *data,int *index);
int     cmd_sub(char *data,int *index);
int     cmd_mul(char *data,int *index);
int     cmd_dev(char *data,int *index);
int     cmd_rand(char *data,int *index);
int     cmd_exit(char *data,int *index);
int		cmd_mod(char *data,int *index);
int     update_target_code(void);
int     update_all_goto(void);
int     check_all_call(void);
int     check_all_proc(void);
int     check_all_couple(void);
unsigned char   check_op(char *data);
void    get_whole_word(char *in,char *out,int *idx,int max);
void    exit_system(int exit_val);
int     write_target_file_header(FILE *fp, MAC_HEAD *phead);

int get_source_number(char *source, int *source_index, 
                      signed long int *number, unsigned char *number_id);
int get_source_op(char *source, int *source_index, unsigned char *op);
int encode_cmd_code(unsigned long cmd_code);
int encode_code_offset(unsigned char *buf, unsigned long code_offset);
int     put_usercmd_word_to_target(char *data);
int     put_usercmd_string_to_target(char *data);


int main(int argc,char **argv)
{
    int     ret;
    
    if((argc<2)||(argc>3))
    {
        show_usage( argv[0] );
        return(0);
    }
    if(argc==2)
    {
        if( (strcmp(argv[1],"/?")==0) || (strcmp(argv[1],"/H")==0)
            ||(strcmp(argv[1],"-?")==0)||(strcmp(argv[1],"-H")==0)
            || (strcmp(argv[1],"/h") == 0) || (strcmp(argv[1], "-h") == 0) )
        {
            show_usage( argv[0] );
            return(0);
        }
    }
    
    show_skip_line();
    show_dump_line();
    show_title( MAC_VERSION );
    show_copyright();
    show_skip_line();
    
    read_ini_info(INI_INFO_FILENAME);
    if(argc==2)
    {
        strcpy(source_filename,argv[1]);
        strcpy(print_rec,source_filename);
        change_ext(target_filename,print_rec,(char *)mc_information.file_ext);
    }
    if(argc==3)
    {
        strcpy(source_filename,argv[1]);
        strcpy(target_filename,argv[2]);
        strcpy(print_rec,argv[2]);
        change_ext(target_filename,print_rec,(char *)mc_information.file_ext);
    }
    printf("source : %s\n",source_filename);
    printf("target : %s\n",target_filename);

    ret = mc_main();
    show_skip_line();
    if(ret==TTN_OK)
    {
        if(compiler_config.data_file=='1')
        {
            printf("*****************************************************************************\n");
            output_command_data_file();
        }
        if(compiler_config.list_code=='1')
        {
            printf("*****************************************************************************\n");
            list_mac(target_filename);
        }
        export_header();
    }
    else
    {
        printf("Compile error.\n");
        printf("\n");
    }

    if(compiler_config.log_langscript == '1')
    {
		if(strcmp((const char*)compiler_config.uedit32_dir, ".") != 0)
		{
			export_ultraedit_langscript();
			update_ultraedit_langscript();
		}

		//add by jessie		
		if(strcmp((const char*)compiler_config.notepad_dir, ".") != 0)
		{
			printf("do notepad\n");
			Config_NotPad_Xml();
		}
    }
    exit_system(ret);
    return(ret);
}


void    exit_system(int exit_val)
{
    if(pmac_head) {free(pmac_head); pmac_head=NULL; }
    exit(exit_val);
}

void    init_source_line(void)
{
    source_line=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
    if(source_line==NULL)
    {
        printf("Memory alloc error!!!\n");
        exit_system(-1);
    }
    source_line->last=NULL;
    source_line->next=NULL;
    memset(source_line->buffer,0x00,LINE_BUFFER_SIZE);
}

void load_source_file(char *filename)
{
    FILE    *fp;
    int     i,len;
    char    line[LINE_BUFFER_SIZE];
    LINE_DNODE      *p,*s;
    
    fp=fopen(filename,"rb");
    if(fp==NULL)
    {
        printf("File '%s' open error!!!\n",filename);
        exit_system(-2);
    }
    p=source_line;
    while(!feof(fp))
    {
        memset(line, 0x00, LINE_BUFFER_SIZE);
        fgets(line, LINE_BUFFER_SIZE-1, fp);
        len = strlen(line);
        for(i=0;i<len;i++)
        {
            if(line[i]==0x0A || line[i]==0x0D)
                line[i]=0x00;
        }
        s = (LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(NULL == s)
        {
            printf("Memory alloc error!!!\n");
            exit_system(-1);
        }
        s->last = NULL;
        s->next = NULL;
        strcpy((char *)s->buffer,(const char *)line);
        
        p->next=s;
        s->last=p;
        p=p->next;

        if(feof(fp))
            break;
    }
    fseek(fp, 0L, SEEK_END);
    system_source_size = ftell(fp);
    fclose(fp);
}


void change_ext(char *target,char *filename,char *ext)
{
    int i;
    int find;
    
    find=0;
    for(i=0;i<(int)strlen(filename);i++)
    {
        if(filename[i]=='.')find=1;
    }

    if(find==0)
    {
        strcpy(target,filename);
        strcat(target,".");
        strcat(target,ext);
        return;
    }
    for(i=0;i<(int)strlen(filename);i++)
    {
        if(filename[i]=='.')
        {
            target[i]=(char)'.';
            target[i+1]=(char)NULL;
            strcat(target,ext);
            break;
        }
        target[i]=filename[i];
        target[i+1]=(char)NULL;
    }
    return;
}

void show_copyright(void)
{
    printf("%s\n",MESSAGE_COPYRIGHT);
}

void show_title(unsigned long version)
{
    printf(MESSAGE_TITLE, (version >> 16) & 0xffff, version & 0xffff);
}

void show_author(void)
{
    printf("%s\n",MESSAGE_AUTHOR);
}

void show_usage(char *first_arg)
{
    show_skip_line();
    show_title(MAC_VERSION);
    show_copyright ();
    show_author ();
    printf(MESSAGE_USAGE, first_arg);
}

void show_dump_line(void)
{
    printf("%s\n",MESSAGE_DUMP_LINE);
}

void show_skip_line(void)
{
    printf("\n");
}

void    init_compiler(void)
{
    int     i,j;
    for(i=0;i<MAX_DEFINE;i++)
    {
        define_table[i].flag='0';
        memset(define_table[i].name,0x00,256);
        memset(define_table[i].data,0x00,256);
    }
    for(i=0;i<MAX_LABEL;i++)
    {
        label_table[i].flag='0';
        memset(label_table[i].name,0x00,256);
        label_table[i].offset=-1;
    }
    for(i=0;i<MAX_IF;i++)
    {
        if_table[i].offset=-1;
        if_table[i].else_offset=-1;
        if_table[i].max_elseif=0;
        if_table[i].find_else=0;
        for(j=0;j<MAX_ELSEIF;j++)
        {
            if_table[i].elseif_offset[j]=-1;
            if_table[i].break_offset[j]=-1;
        }
    }
    system_if_index=0;
    for(i=0;i<MAX_SWITCH;i++)
    {
        switch_table[i].number=0;
        switch_table[i].max_case=0;
        switch_table[i].default_offset=-1;
        for(j=0;j<MAX_CASE;j++)
        {
            switch_table[i].case_offset[j]=-1;
            switch_table[i].endcase_offset[j]=-1;
        }
        switch_table[i].find_case=0;
    }
    system_switch_index=0;
    for(i=0;i<MAX_WHILE;i++)
    {
        while_table[i].offset=-1;
        while_table[i].start=-1;
        for(j=0;j<MAX_BREAK;j++)
        {
            while_table[i].break_offset[j]=-1;
        }
        while_table[i].max_break=0;
    }
    system_while_index=0;
    for(i=0;i<MAX_GOTO;i++)
    {
        goto_table[i].flag='0';
        memset(goto_table[i].name,0x00,256);
        goto_table[i].offset_address=-1;
    }
    system_goto_index=0;
    for(i=0;i<MAX_CALL;i++)
    {
        call_table[i].flag='0';
        call_table[i].source_line_number=0;
        call_table[i].call_proc=-1;
    }
    system_call_index=0;

	system_strvar_number=0;
}

int     mc_main(void)
{
    int     ret;
    int     result;
    
    ret=TTN_NOT_OK;
    init_source_line();
    init_compiler();
    init_compile_output();
    load_source_file(source_filename);
    pass1_check();
    pass2_check();
    pass3_check();
    result = print_compile_output();
    if(result == COMPILE_OK)
    {
        ret = create_target_file();
        if(ret != TTN_OK)
        {
            printf("\n");
            printf("Compile OK, but create target file failed !!! \n");
            printf("%c%c\n",0x07,0x07);
            getch();
        }
    }
    else
    {
        printf("\n");
        printf("Compile failed !!! \n");
        printf("%c%c\n",0x07,0x07);
        getch();
    }
    return(ret);
}

void    skip_space(char *data,int *index)
{
    int     pass;
    int     ii;
    
    pass=0;
    ii=*index;
    while(pass==0)
    {
        switch(data[ii])
        {
        case ' ': case 0x09:
            ii++;
            break;
        default:
            *index=ii;
            pass=1;
            break;
        }
    }
    return;
}

int get_symbol(char *in,char *out,int *idx,int max)
{
    static  char    symbol[256];
    int             in_len;
    int             pass;
    int             ii,jj;
    char            ch;
    int             symbol1;
    int             symbol2;
    char            ch1;
    int             convene1;
    int             convene;
    
    in_len=strlen(in);
    pass=0;
    ch1=in[*idx];
    symbol1=SYMBOL_NONE;
    symbol2=SYMBOL_NONE;
    memset(symbol,0x00,256);
    convene1=check_char_convene(ch1);
    switch(convene1)
    {
    case CONVENE_UNKNOWN:
        symbol1=SYMBOL_UNKNOWN;
        break;
    case CONVENE_END:
        symbol1=SYMBOL_NONE;
        break;
    case CONVENE_COMPARTMENT:
        symbol1=SYMBOL_SYSTEM;
        break;
    case CONVENE_OPERATOR:
        symbol1=SYMBOL_OPERATOR;
        break;
    case CONVENE_NUMBER:
        symbol1=SYMBOL_IDENTIFY;
        break;
    case CONVENE_CHAR:
        symbol1=SYMBOL_IDENTIFY;
        break;
    }
    for(jj=0,ii=*idx;ii<in_len;ii++)
    {
        ch=in[ii];
        convene=check_char_convene(ch);
        switch(convene)
        {
        case CONVENE_UNKNOWN:
            symbol2=SYMBOL_UNKNOWN;
            break;
        case CONVENE_END:
            symbol2=SYMBOL_NONE;
            break;
        case CONVENE_COMPARTMENT:
            symbol2=SYMBOL_SYSTEM;
            break;
        case CONVENE_OPERATOR:
            symbol2=SYMBOL_OPERATOR;
            break;
        case CONVENE_NUMBER:
            symbol2=SYMBOL_IDENTIFY;
            break;
        case CONVENE_CHAR:
            symbol2=SYMBOL_IDENTIFY;
            break;
        default:
            symbol2=SYMBOL_NONE;
            break;
        }
        if(symbol1==symbol2)
        {
            symbol[jj]=in[ii];
            jj++;
            if(jj>=max-1)
                pass=1;
        }
        else
        {
            pass=1;
        }
        if(pass==1)
            break;
    }
    *idx=ii;
    symbol[jj]=0x00;
    strcpy(out,symbol);

    return(symbol1);
}

int     pass1_check(void)
{
    LINE_DNODE      *p;
    int     index;
    char    temp[256];
    char    temp1[256];
    char    temp2[256];
    char    buffer[1024];
    int     sub_pass;
    int     symbol;
    int     ret;
    
    ret = TTN_OK;
    printf("Process Pass1 check ...............................\n");
    system_line_number=0;
    p=source_line;
    symbol=SYMBOL_NONE;
    while(p->next!=NULL)
    {
        p=p->next;
        system_line_number++;
        
        index=0;
        memset(line_buffer,0x00,LINE_BUFFER_SIZE);
        strcpy((char *)line_buffer,(const char *)p->buffer);
        memset(temp,0x00,256);
        skip_space(line_buffer,&index);
        get_whole_word(line_buffer,temp,&index,255);
        
        if(temp[0]!=0x00)
        {
            if(is_define(temp))
            {
                // get define MSG to temp
                memset(temp,0x00,256);
                skip_space(line_buffer,&index);
                get_word(line_buffer,temp,&index,255);
                
                // phase define DATA
                memset(buffer,0x00,256);
                sub_pass=0;
                while(sub_pass==0)
                {
                    // get define DATA symbol(s)
                    memset(temp1,0x00,256);
                    skip_space(line_buffer,&index);
                    symbol=get_symbol(line_buffer,temp1,&index,255);
                    if(symbol==SYMBOL_IDENTIFY)
                    {
                        memset(temp2,0x00,256);
                        change_define(temp1,temp2);
                        strcat(buffer,temp2);
                    }
                    else if(symbol==SYMBOL_OPERATOR)
                    {
                        memset(temp2,0x00,256);
                        strcpy(temp2,temp1);
                        strcat(buffer,temp2);
                    }
                    else
                    {
                        sub_pass=1;
                    }
                }
                register_define(temp,buffer);
            }
            else if(is_remark(temp))
            {
                // printf("[remark]%s\n",line_buffer);
            }
            else if(is_label(temp))
            {
                register_label(temp);
                // printf("[label]%s\n",label);
            }
            else if(is_command(temp))
            {
                // printf("[command]%s\n",temp);
            }
			//v2011-1-27  (string variable  data = @xx)
			else if(is_strvar(temp))
			{
				// phase strvar DATA
				memset(buffer,0x00,256);
				sub_pass=0;
				while(sub_pass==0)
				{
					// get strvar DATA symbol(s)
					memset(temp1,0x00,256);
					skip_space(line_buffer,&index);
					symbol=get_symbol(line_buffer,temp1,&index,255);
					// a~z or 0~9 or _
					if(symbol==SYMBOL_IDENTIFY)
					{
						memset(temp2,0x00,256);
						change_define(temp1,temp2);
						
						//重复定义允许,但无效.
						if(strcmp(temp1, temp2) == 0)
						{
							//变量 采用 901~1000
							if(system_strvar_number >= MAX_SYSTEM_STRVAR)
							{
								sprintf(print_rec,"Strvar overflow(>%d) .",MAX_SYSTEM_STRVAR);
			       				output_message(OUTPUT_ERROR,system_line_number,print_rec);
								return(TTN_ERROR);
							}

							//实际 是宏定义 #define strvarname  @val
							sprintf(temp2,"%d", (++system_strvar_number)+SYSTEM_STRVAR_BEGIN);
							
							register_define(temp1, temp2);
						}
					}
					else if(symbol==SYMBOL_OPERATOR || symbol== SYMBOL_SYSTEM)
					{
	       				output_message(OUTPUT_ERROR,system_line_number,"string variable name must (a~z or 0~9 or _ )");
	        			return(TTN_ERROR);
					}
					else
					{
						sub_pass=1;
					}
				 }
			}
            else
            {
                sprintf(print_rec,"Unknow command '%s'.",temp);
                output_message(OUTPUT_ERROR,system_line_number,print_rec);
                ret = TTN_NOT_OK;
            }
        }
    }
    printf("        Pass1 check finished !\n");
    return  ret;
}


int     pass2_check(void)
{
    LINE_DNODE      *p;
    int         index;
    char        temp[256];
    int         ret;

    ret = TTN_OK;
    printf("Process Pass2 check ...............................\n");
    init_mac_head(MAX_PROC);
    init_target_code();
    system_line_number=0;
    p=source_line;
    while(p->next!=NULL)
    {
        p=p->next;
        system_line_number++;
        
        index=0;
        memset(line_buffer,0x00,LINE_BUFFER_SIZE);
        strcpy((char *)line_buffer,(const char *)p->buffer);
        memset(temp,0x00,256);
        skip_space(line_buffer,&index);
        get_whole_word(line_buffer,temp,&index,255);
        if(temp[0]!=0x00)
        {
            if(is_label(temp))
            {
                update_label(&temp[1],target_offset);
            }
            else if(is_command(temp))
            {
                if(is_system_command(temp))
                {
                    ret = convert_system_command(line_buffer);
                }
                else
                {
                    ret = convert_user_command(line_buffer);
                }
            }
            else
            {
                // is define or remark, have been processed in pass1_check
            }
        }
    }
    update_target_code();
    check_all_couple();
    printf("        Pass2 check finished !\n");
    return  ret;
}

void    init_compile_output(void)
{
    compile_output=(OUTPUT_DNODE *)malloc(sizeof(struct OUTPUT_DNODE_STRUCT));
    if(compile_output==NULL)
    {
        printf("Memory alloc error!!!\n");
        exit_system(-1);
    }
    compile_output->last=NULL;
    compile_output->next=NULL;
    compile_output->flag=OUTPUT_NONE;
    compile_output->line_number=0;
    memset(compile_output->message,0x00,LINE_BUFFER_SIZE);
}

void    output_message(char flag,int line_number,char *message)
{
    OUTPUT_DNODE    *p,*s;
    
    p=compile_output;
    while(p->next!=NULL)
        p=p->next;
    s=(OUTPUT_DNODE *)malloc(sizeof(struct OUTPUT_DNODE_STRUCT));
    if(s==NULL)
    {
        printf("Memory alloc error!!!\n");
        exit_system(-1);
    }
    s->last=NULL;
    s->next=NULL;
    s->flag=flag;
    s->line_number=line_number;
    strcpy(s->message,message);
    
    p->next=s;
    s->last=p;
    return;
}

//v2011-1-27  (string variable:  strvar  varname ....)
int is_strvar(char *data)
{
   return (0 == strcmpi(strupr((char *)data),strupr((char *)mc_information.strvar)));
}

int is_remark(char *data)
{
    return (data[0] == mc_information.remark);
}


int is_define(char *data)
{
    return (0 == strcmpi(strupr((char *)data),strupr((char *)mc_information.define)));
}


int is_command(char *data)
{
    int     i;
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag != '1')
            continue;

        if (0 == strcmpi(strupr((char *)data),strupr((char *)command_table[i].keyword)))
        {
            return 1;
        }
    }
    return 0;
}


int is_label(char *data)
{
    return (data[0] == mc_information.label);
}


int check_char_convene(char ch)
{
    int     convene;
    int     i,len;
    char    char_end[3]={0x0a,0x0d,0x00};
    char    char_com[5]={ ' ', ',', ':', ';' ,0x09};
    char  * char_op="~!@#$%^&*()-+=\\|[]{}'\".<>/?";
    char  * char_num="0123456789";
    char  * char_char="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    
    convene = CONVENE_UNKNOWN;
    for(i=0;i<3;i++)
    {
        if(ch==char_end[i])
        {
            convene=CONVENE_END;
            return(convene);
        }
    }
    for(i=0;i<5;i++)
    {
        if(ch==char_com[i])
        {
            convene=CONVENE_COMPARTMENT;
            return(convene);
        }
    }
    len=strlen(char_op);
    for(i=0;i<len;i++)
    {
        if(ch==char_op[i])
        {
            convene=CONVENE_OPERATOR;
            return(convene);
        }
    }
    len=strlen(char_num);
    for(i=0;i<len;i++)
    {
        if(ch==char_num[i])
        {
            convene=CONVENE_NUMBER;
            return(convene);
        }
    }
    len=strlen(char_char);
    for(i=0;i<len;i++)
    {
        if(ch==char_char[i])
        {
            convene=CONVENE_CHAR;
            return(convene);
        }
    }
    return(convene);
}

void    change_define(char *in,char *out)
{
    int     i;
    int     find;
    
    strcpy(out,in);
    find=0;
    for(i=0;i<MAX_DEFINE;i++)
    {
        if(define_table[i].flag=='1')
        {
            if(strcmpi(in,define_table[i].name)==0)
            {
                strcpy(out,define_table[i].data);
                find=1;
            }
        }
        if(find==1)
            break;
    }
    return;
}

int     register_define(char *name,char *data)
{
    int     i;
    int     index;
    
    if(strlen(data)>255)
    {
        output_message(OUTPUT_ERROR,system_line_number,"Define too long.");
        data[255]=0x00;
    }
    for(i=0;i<MAX_DEFINE;i++)
    {
        if(define_table[i].flag=='1')
        {
            if(strcmpi(name,define_table[i].name)==0)
            {
				sprintf(print_rec,"Duplicate define '%s' .",name);
				output_message(OUTPUT_ERROR,system_line_number,print_rec);
				return(TTN_ERROR);
            }
        }
    }
    index=-1;
    for(i=0;i<MAX_DEFINE;i++)
    {
        if(define_table[i].flag=='0')
        {
            index=i;
            i=MAX_DEFINE+100;
        }
    }
    if(index<0)
    {
        sprintf(print_rec,"Define overflow(>%d) .",MAX_DEFINE);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    strcpy(define_table[index].name,name);
    strcpy(define_table[index].data,data);
    define_table[index].flag='1';
    return(TTN_OK);
}

int     register_label(char *name)
{
    int     index;
    int     i;
    for(i=0;i<MAX_LABEL;i++)
    {
        if(label_table[i].flag=='1')
        {
            if(strcmpi(&name[1],label_table[i].name)==0)
            {
                sprintf(print_rec,"Duplicate label '%s' .",&name[1]);
                output_message(OUTPUT_ERROR,system_line_number,print_rec);
                return(TTN_ERROR);
            }
        }
    }
    index=-1;
    for(i=0;i<MAX_LABEL;i++)
    {
        if(label_table[i].flag=='0')
        {
            index=i;
            i=MAX_LABEL+100;
        }
    }
    if(index<0)
    {
        sprintf(print_rec,"Label overflow(>%d) .",MAX_LABEL);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    strcpy(label_table[index].name,&name[1]);
    label_table[index].flag='1';
    return(TTN_OK);
}

int     print_compile_output(void)
{
    FILE *fp = NULL;
    OUTPUT_DNODE  * p;
    int             ret;
    char    filename[_MAX_FNAME];
    int log_file;

    ret=COMPILE_OK;

    log_file = 0;
    if(compiler_config.log_error == '1')
    {
        sprintf( (char *)filename,"%s", LOGERROR_FILENAME);
        remove((const char *)filename);
        fp = fopen((const char *)filename, "a");
        if(fp) log_file = 1;

        printf("open file %s ok\n", filename);
    }

    p=compile_output;
    while(p->next!=NULL)
    {
        p=p->next;
        
        switch(p->flag)
        {
        case OUTPUT_WARNING:
            {
                sprintf(print_rec,"  (WARNING) ## Line[%d]: %s\n",p->line_number,
                    p->message);
                printf(print_rec);
                if(log_file)
                {
                    fprintf(fp, (const char *)print_rec);
                }
                ret=COMPILE_NOT_OK;
            }
            break;
        case OUTPUT_ERROR:
            {
                sprintf(print_rec,"  (ERROR)  ## Line[%d]: %s\n",p->line_number,
                    p->message);
                printf(print_rec);
                if(log_file)
                {
                    fprintf(fp, (const char *)print_rec);
                }
                ret=COMPILE_NOT_OK;

                break;
            }
        case OUTPUT_NONE:
            break;
        case OUTPUT_MESSAGE:
            {
                sprintf(print_rec,"  (MESSAGE)##  %s\n",p->message);
                printf(print_rec);
                if(log_file)
                {
                    fprintf(fp, (const char *)print_rec);
                }
                break;
            }
        }
    }

    if(log_file)
    {
        fclose(fp);
    }
    return(ret);
}

void    init_mac_head(unsigned long int max_proc)
{
    unsigned long int    i;
    unsigned long int   head_size;
    
    if(pmac_head) { free(pmac_head); pmac_head=NULL; }
    head_size = sizeof(*pmac_head)+(max_proc-1)*sizeof(signed long int);
    pmac_head = (MAC_HEAD *)malloc(head_size);
    if(NULL==pmac_head)
    {
        printf("ERROR : memory alloc error\n");
        exit_system(-1);
    }
    memset(pmac_head,0,sizeof(*pmac_head)+(max_proc-1)*sizeof(signed long int));
    pmac_head->copyright[0] = '3';
    pmac_head->copyright[1] = 'S';
    pmac_head->copyright[2] = 'S';
    pmac_head->copyright[3] = 0x1a;
    pmac_head->id[0] = 'M';
    pmac_head->id[1] = 'A';
    pmac_head->id[2] = 'C';
    pmac_head->id[3] = ' ';
    pmac_head->version = MAC_VERSION;
    pmac_head->head_size = head_size;
    pmac_head->data_size = 0;
    pmac_head->key_check = MAC_KEYCHECK;
    pmac_head->max_proc = max_proc;
    for(i=0;i<pmac_head->max_proc;i++)
    {
        pmac_head->proc_address[i] = INVALID_PROC_ADDRESS;
    }
}

void    init_target_code(void)
{
    if(system_source_size<=0)
    {
        printf("Can't create target !!!\n");
        exit_system(-6);
    }
    system_target_size = system_source_size * 4;
    target_code=(unsigned char *)malloc(system_target_size);
    if(target_code==NULL)
    {
        printf("Memory alloc error !!!\n");
        exit_system(-1);
    }
    memset(target_code,0xff,system_target_size);
    target_offset=0;
}

int search_command_index(char *keyword)
{
    int i;
    int index;
    
    index=-1;
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag=='1')
        {
            if(strcmpi(strupr((char *)keyword),strupr((char *)command_table[i].keyword))==0)
            {
                index=i;
                i=MAX_COMMAND+100;
            }
        }
    }
    return(index);
}

void update_label(char *name,unsigned long int offset)
{
    int i;
    for(i=0;i<MAX_LABEL;i++)
    {
        if(label_table[i].flag=='1')
        {
            if(strcmpi(name,label_table[i].name)==0)
                label_table[i].offset=offset;
        }
    }
    return;
}

int is_system_command(char *keyword)
{
    int i;
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag != '1')
            continue;
            
        if ( command_table[i].attrib != COMMAND_ATTRIB_SYSTEM )
            continue;

        if(0 == strcmpi(strupr((char *)keyword),strupr((char *)command_table[i].keyword)))
        {
            return 1;
        }
    }
    return 0;
}

int convert_system_command(char *data)
{
    unsigned long   command_code;
    int             command_index;
    char            temp[256];
	char            temp1[256];
	char            temp2[256];
	char            buffer[256];
    int             index;
    int             ret;
	int				sub_pass;
	int				symbol;
	int				realindex;
    
    ret=TTN_OK;
    index=0;
    memset(temp,0x00,256);
    
    //v2011-1-28 check define DATA
    memset(buffer,0x00,256);
    sub_pass=0;
    while(sub_pass==0)
    {
        memset(temp1,0x00,256);
		realindex = index;
        skip_space(data,&index);
		if(index > realindex)
		{
			strcat(buffer," ");
		}

        symbol=get_symbol(data,temp1,&index,255);
        if(symbol==SYMBOL_IDENTIFY)
        {
            memset(temp2,0x00,256);
            change_define(temp1,temp2);
            strcat(buffer,temp2);
        }
        else if(symbol==SYMBOL_OPERATOR)
        {
            memset(temp2,0x00,256);
            strcpy(temp2,temp1);
            strcat(buffer,temp2);
        }
        else
        {
            sub_pass=1;
        }
    }

	index=0;
    memset(temp,0x00,256);
    skip_space(buffer,&index);
    get_whole_word(buffer,temp,&index,255);
    command_index=search_command_index(temp);
    if(command_index<0)
        return(TTN_ERROR);
    command_code = command_table[command_index].command_code;
    switch(command_code)
    {
    case CMD_PROC:
        ret=cmd_proc(buffer,&index);
        break;
    case CMD_ENDPROC:
        ret=cmd_endproc(buffer,&index);
        break;
    case CMD_IF:
        ret=cmd_if(buffer,&index);
        break;
    case CMD_ELSE:
        ret=cmd_else(buffer,&index);
        break;
    case CMD_ELSEIF:
        ret=cmd_elseif(buffer,&index);
        break;
    case CMD_ENDIF:
        ret=cmd_endif(buffer,&index);
        break;
    case CMD_WHILE:
        ret=cmd_while(buffer,&index);
        break;
    case CMD_BREAK:
        ret=cmd_break(buffer,&index);
        break;
    case CMD_ENDWHILE:
        ret=cmd_endwhile(buffer,&index);
        break;
    case CMD_SWITCH:
        ret=cmd_switch(buffer,&index);
        break;
    case CMD_CASE:
        ret=cmd_case(buffer,&index);
        break;
    case CMD_ENDCASE:
        ret=cmd_endcase(buffer,&index);
        break;
    case CMD_ENDSWITCH:
        ret=cmd_endswitch(buffer,&index);
        break;
    case CMD_GOTO:
        ret=cmd_goto(buffer,&index);
        break;
    case CMD_CALL:
        ret=cmd_call(buffer,&index);
        break;
    case CMD_RETURN:
        ret=cmd_return(buffer,&index);
        break;
    case CMD_EXEC:
        ret=cmd_exec(buffer,&index);
        break;
    case CMD_VAR:
        ret=cmd_var(buffer,&index);
        break;
    case CMD_ADD:
        ret=cmd_add(buffer,&index);
        break;
    case CMD_SUB:
        ret=cmd_sub(buffer,&index);
        break;
    case CMD_MUL:
        ret=cmd_mul(buffer,&index);
        break;
    case CMD_DEV:
        ret=cmd_dev(buffer,&index);
        break;
    case CMD_RAND:
        ret=cmd_rand(buffer,&index);
        break;
    case CMD_EXIT:
        ret=cmd_exit(buffer,&index);
        break;
	case CMD_MOD:
        ret=cmd_mod(buffer,&index);
        break;
    default:
        ret=TTN_ERROR;
        break;
    }
    return(ret);
}

void string_define_check(char *in, char *out)
{
    char    temp1[1024];
    char    temp2[1024];
    int     sub_pass;
    int     index;
	int     realindex;
    int     symbol;
    static  char    buffer[1024];

    index=0;
    sub_pass=0;
    memset(buffer,0x00,1024);
    while(sub_pass==0)
    {
        memset(temp1,0x00,1024);
		realindex = index;
        skip_space(in,&index);
		while(index > realindex)
		{
			strcat(buffer," ");
			realindex++;
		}
        symbol=get_symbol(in,temp1,&index,1024);
        switch(symbol)
        {
        case SYMBOL_IDENTIFY:
            memset(temp2,0x00,1024);
            change_define(temp1,temp2);
            strcat(buffer,temp2);
            break;
        case SYMBOL_NONE:
			sub_pass = 1;
			break;
		default:
            memset(temp2,0x00,1024);
            strcpy(temp2,temp1);
            strcat(buffer,temp2);
            break;
        }
    }
    if(strlen(buffer)>1024)
    {
        output_message(OUTPUT_ERROR,system_line_number,"Parameter too long .");
        buffer[255]=0x00;
    }
    strcpy(out,buffer);

    return;
}

int convert_user_command(char *data)
{
    unsigned long   command_code;
    int             command_index;
    unsigned char   parameter_type;
    char            keyword[1024];
    char            temp[1024];
	char            temp1[1024];
	char            temp2[1024];
	
	int             index;
    int             ret;
    int             i;
    int             skip_len;
    int             command_skip_len;
    unsigned long int       command_length_address;
    signed long   int       number;
    unsigned char           number_id;
    
    ret=TTN_OK;
    index=0;
    memset(temp,0x00,1024);
    skip_space(data,&index);
    get_whole_word(data,temp,&index,255);
    command_index=search_command_index(temp);
    if(command_index<0)
        return(TTN_ERROR);
    memset(keyword,0x00,256);
    strcpy(keyword,temp);
    command_code = command_table[command_index].command_code;
    
    encode_cmd_code(command_code);

    target_code[target_offset]=0;
    command_length_address=target_offset;
    target_code[target_offset++]=0; 	
    target_offset++;
    
    command_skip_len=0;
    for(i=0;i<MAX_PARAMETER;i++)
    {
        skip_len=0;
        parameter_type=command_table[command_index].parameter_type[i];
        switch(parameter_type)
        {
        case INT_TYPE:
            if(TTN_OK == get_source_number(data, &index, &number, &number_id))
            {
                skip_len = put_number_to_target(number_id,number);
            }
            else
            {
                sprintf(print_rec,"%s parameter %d error(must be int)",keyword,i+1);
                output_message(OUTPUT_ERROR,system_line_number,print_rec);
            }
            break;
        case WORD_TYPE:
            memset(temp,0x00,256);
            skip_space(data,&index);
            get_whole_word(data,temp,&index,255);
            if(temp[0]!=0x00)
            {
                skip_len = put_usercmd_word_to_target(temp);
            }
            else
            {
                sprintf(print_rec,"%s parameter %d error(must be word)",keyword,i+1);
                output_message(OUTPUT_ERROR,system_line_number,print_rec);
            }
            break;
        case STR_TYPE:
            memset(temp,0x00,1024);
            skip_space(data,&index);
            //get_string(data,temp,&index,255);
			//warning the writer
			get_string(data,temp1,&index,1024);
            if(temp1[0]!=0x00)
            {
				string_define_check(temp1,temp2);
				skip_len=put_usercmd_string_to_target(temp2);
            }
            else
            {
                sprintf(print_rec,"%s parameter %d error(must be string)",keyword,i+1);
                output_message(OUTPUT_ERROR,system_line_number,print_rec);
            }
			break;
        case DUMMY_TYPE:
            break;
        }
        command_skip_len+=skip_len;
    }
    if(command_skip_len>1023)
    {
        output_message(OUTPUT_ERROR,system_line_number,"Command parameter too long .");
        ret = TTN_NOT_OK;
    }
    target_code[command_length_address]= command_skip_len-(command_skip_len/0x100)*0x100;
    target_code[command_length_address+1]= command_skip_len/0x100;
    return(ret);
}

int put_number_u2_to_target(unsigned char id,unsigned short int number)
{
    int     skip_len;
    
    skip_len=3;
    target_code[target_offset++] = id;
    target_code[target_offset++] = (unsigned char) (number % 0x100);
    target_code[target_offset++] = (unsigned char) (number / 0x100);
    return(skip_len);
}

int     put_number_s4_to_target(unsigned char id,signed long int number)
{
    int     skip_len;
    
    skip_len=5;
    target_code[target_offset++]=id;
    *(signed long int *)&target_code[target_offset] = (signed long int)number;
    target_offset += 4;
    return(skip_len);
}

int     put_word_to_target(char *data)
{
    int     skip_len;
    int     len;
    int     i;
    
    skip_len=0;
    len=strlen(data);
    target_code[target_offset++]=(unsigned char)len;
    for(i=0;i<len;i++)
        target_code[target_offset++]=data[i];
    skip_len=len+1;
    return(skip_len);
}

int     put_string_to_target(char *data)
{
    int     skip_len;
    int     len;
    int     i;
    
    skip_len=0;
    len=strlen(data);
    target_code[target_offset++]=(unsigned char)len;
    for(i=0;i<len;i++)
        target_code[target_offset++]=data[i];
    skip_len=len+1;
    return(skip_len);
}

int     put_usercmd_word_to_target(char *data)
{
    int     skip_len;
    int     len;
    int     i;
    
    skip_len=0;
    len=strlen(data);
    target_code[target_offset++]=len-(len/0x100)*0x100;
    target_code[target_offset++]=len/0x100;
    for(i=0;i<len;i++)
        target_code[target_offset++]=data[i];
    skip_len=len+2;
    return(skip_len);
}

int     put_usercmd_string_to_target(char *data)
{
    int     skip_len;
    int     len;
    int     i;
    
    skip_len=0;
    len=strlen(data);
     target_code[target_offset++]=len-(len/0x100)*0x100;
    target_code[target_offset++]=len/0x100;
    for(i=0;i<len;i++)
        target_code[target_offset++]=data[i];
    skip_len=len+2;
    return(skip_len);
}

int     create_target_file(void)
{
    char    filename[_MAX_PATH + _MAX_FNAME];
    FILE    *fp = NULL;
    int     ret;
    
    ret=TTN_OK;
    printf("Process Create target file ........................\n");
    sprintf((char *)filename,"%s%c%s",compiler_config.target_dir,
                DIR_SEPARATE_CHAR, target_filename);
    fp = fopen(filename,"wb");
    if(fp == NULL)
    {
        printf("        Can't create target file(%s) !!!\n",filename);
        return(TTN_NOT_OK);
    }
    pmac_head->data_size = target_offset;

    //Jim, changed [5/9/2004]
    //{{{
    //fwrite(pmac_head, 1, sizeof(*pmac_head)+(pmac_head->max_proc-1)*sizeof(signed long int), fp);
    write_target_file_header(fp, pmac_head);
    //}}}

    fwrite(&target_code[0],1,target_offset,fp);
    fclose(fp);

    printf("        FileName : %s\n",target_filename);
    printf("        FileSize : %d\n",sizeof(*pmac_head)+(pmac_head->max_proc-1)*sizeof(signed long int)+pmac_head->data_size);
    printf("        CodeSize : %d\n",pmac_head->data_size);
    printf("Create target file %s OK !\n",target_filename);
    printf("\n");
    return(ret);
}

void    replace_define(char *in,char *out)
{
    char    temp1[256];
    char    temp2[256];
    int     sub_pass;
    int     index;
    int     symbol;
    static  char    buffer[1024];

    index=0;
    sub_pass=0;
    memset(buffer,0x00,1024);
    while(sub_pass==0)
    {
        memset(temp1,0x00,256);
        skip_space(in,&index);
        symbol=get_symbol(in,temp1,&index,255);
        switch(symbol)
        {
        case SYMBOL_IDENTIFY:
            memset(temp2,0x00,256);
            change_define(temp1,temp2);
            strcat(buffer,temp2);
            break;
        case SYMBOL_OPERATOR:
            memset(temp2,0x00,256);
            strcpy(temp2,temp1);
            strcat(buffer,temp2);
            break;
        case SYMBOL_NONE:
        case SYMBOL_SYSTEM:
            sub_pass = 1;
            break;
        case SYMBOL_UNKNOWN:
            sprintf((char *)print_rec, "Unknown symbol (%s) in (%s).", temp1, in);
            output_message(OUTPUT_ERROR,system_line_number, print_rec);
            sub_pass = 1;
            break;
        }
    }
    if(strlen(buffer)>255)
    {
        output_message(OUTPUT_ERROR,system_line_number,"Parameter too long .");
        buffer[255]=0x00;
    }
    strcpy(out,buffer);

    return;
}

int cmd_proc(char *data,int *index)
{
    int     ret;
    int     skip_len;
    int     command_skip_length;
    signed  long int        number;
    unsigned char           number_id;
    unsigned long int       command_length_address;
    unsigned long int       proc_start_address;
    
    ret=TTN_OK;
    if(proc_start_flag==1)
    {
        strcpy(print_rec,"PROC already start,must ENDPROC first .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    proc_start_flag=1;

    proc_start_address = target_offset;
    command_skip_length=0;

    encode_cmd_code(CMD_PROC);

    command_length_address=target_offset;
    target_code[target_offset++]=0;

    if(TTN_OK == get_source_number(data, index, &number, &number_id))
    {
        number_id = mc_information.number; //force number
        skip_len=put_number_to_target(number_id,number);
    }
    else
    {
        sprintf(print_rec,"PROC parameter 1 error(must be int)");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }

    command_skip_length = skip_len;
    target_code[command_length_address]=(unsigned char)command_skip_length;
    
    ret = register_proc(number,proc_start_address);
    return(ret);
}

int     register_proc(int index,unsigned long int start_offset)
{
    if((index<0)||(index>MAX_PROC-1))
    {
        sprintf(print_rec,"PROC index overflow(%d not in 0~%d) .",index,MAX_PROC-1);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    if(pmac_head->proc_address[index]>=0)
    {
        sprintf(print_rec,"PROC %d duplicate .",index);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    pmac_head->proc_address[index]=(signed long int)start_offset;
    return(TTN_OK);
}

int     cmd_endproc(char *data,int *index)
{
    int     command_skip_len;
    
    if(proc_start_flag==0)
    {
        strcpy(print_rec,"PROC have not start .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    proc_start_flag=0;
    
    /*      push ENDPROC command            */
    command_skip_len=0;

    encode_cmd_code(CMD_ENDPROC);

    target_code[target_offset++]=(unsigned char)command_skip_len;
    
    return(TTN_OK);
}

/*modified by kent for logic expression, 2004.7.23*/
int     cmd_if(char *data,int *index)
{
	/*
    char    temp[256];
    int     command_skip_len;
    int     skip_len;
    signed long int number1,number2;
    unsigned char   op;
    unsigned char   number_id;
    signed long int else_offset_address;
    signed long int offset_address;
    unsigned char   number_id1;
    unsigned char   number_id2;

    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"IF parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.variable; //force variable

    if(TTN_OK != get_source_op(data, index, &op))
    {
        sprintf(print_rec,"IF parameter 2 op '%s' must be one of (<,=,>,<=,!=,>=).",temp);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        op = OP_ID_EQ;
    }
    
    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"IF parameter 3 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    command_skip_len=0;

    encode_cmd_code(CMD_IF);

    offset_address=target_offset;
    target_code[target_offset++]=(unsigned char)command_skip_len;
    
    else_offset_address = target_offset;
    target_offset += encode_code_offset(target_code + target_offset, 0);
    command_skip_len = command_skip_len + 4;
    
    number_id=number_id1;
    skip_len=put_number_to_target(number_id,number1);
    command_skip_len+=skip_len;
    
    skip_len=put_op_to_target(op);
    command_skip_len+=skip_len;
    
    number_id=number_id2;
    skip_len=put_number_to_target(number_id,number2);
    command_skip_len+=skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;

	if(system_if_index>(MAX_IF-1))
    {
        strcpy(print_rec,"IF nesting overflow .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    if_table[system_if_index].offset=else_offset_address;
    system_if_index++;*/
	
	long int else_offset_address;

	else_offset_address = parselogicexp(data, index);
	
	if(system_if_index>(MAX_IF-1))
    {
        strcpy(print_rec,"IF nesting overflow .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    if_table[system_if_index].offset=else_offset_address;
    system_if_index++;

    return(TTN_OK);
}

int     cmd_else(char *data,int *index)
{
    int     if_index;
    int     find_else;
    int     max_elseif;
    int     elseif_index;
    int     command_skip_len;
	int     tmp;
	int     tmp1;
    unsigned long int       code_offset;
//    signed long int         offset_address;
    signed long int         goto_offset_address;
    
    if_index=system_if_index-1;
    if(if_index<0)
    {
        strcpy(print_rec,"ELSE must after IF .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    find_else=if_table[if_index].find_else;
    max_elseif=if_table[if_index].max_elseif;
    if(find_else!=0)     /* have ELSE    */
    {
        strcpy(print_rec,"ELSE duplicate .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    else                 /* no ELSE      */
    {
        if_table[if_index].find_else=1;
        if(max_elseif<=0) /* no ELSEIF    */// IF...ELSE...ENDIF
        {
            /* insert GOTO command                  */
            command_skip_len=4;
            encode_cmd_code(CMD_GOTO);

            target_code[target_offset++]=(unsigned char)command_skip_len;
            goto_offset_address=target_offset;
            target_offset += encode_code_offset(target_code + target_offset, 0);
            if_table[if_index].else_offset=goto_offset_address;
            
            /* update couple IF jump offset         */
            /*code_offset=target_offset;
            offset_address=if_table[if_index].offset;
            encode_code_offset(target_code + offset_address, code_offset);*/

			/*modified by kent for logic expression, 2004.7.23
			  update false link
			*/
			code_offset=target_offset;
			tmp = if_table[if_index].offset;
			if (tmp != -1)
			{
				while(tmp != 0)
				{
					tmp1 = *(int *)(target_code + tmp);
					encode_code_offset(target_code + tmp, code_offset);
					tmp = tmp1;
				}
			}
			            
            /* output ELSE command                  */
#ifndef TINY_COMPILE
            command_skip_len=0;
            encode_cmd_code(CMD_ELSE);
            target_code[target_offset++]=(unsigned char)command_skip_len;
#endif//TINY_COMPILE
        }
        else              /* have ELSEIF  */// IF...ELSEIF...ELSEIF...ELSE...ENDIF
        {
            /* insert GOTO command                  */
            command_skip_len=4;
            encode_cmd_code(CMD_GOTO);

            target_code[target_offset++]=(unsigned char)command_skip_len;
            goto_offset_address = target_offset;
            target_offset += encode_code_offset(target_code + target_offset, 0);
            if_table[if_index].else_offset = goto_offset_address;
            
			/*modified by kent for logic expression, 2004.7.23*/
            /* update fore ELSEIF jump offset       
            code_offset=target_offset;
            elseif_index=max_elseif-1;
            offset_address=if_table[if_index].elseif_offset[elseif_index];
            encode_code_offset(target_code + offset_address, code_offset);*/


			/*modified by kent for logic expression, 2004.7.23
			  update false link
			*/
			code_offset=target_offset;
            elseif_index=max_elseif-1;
			tmp=if_table[if_index].elseif_offset[elseif_index];
			if (tmp != -1)
			{
				while(tmp != 0)
				{
					tmp1 = *(int *)(target_code + tmp);
					encode_code_offset(target_code + tmp, code_offset);
					tmp = tmp1;
				}
			}
            
            /* output ELSE command                  */
#ifndef TINY_COMPILE
            command_skip_len=0;
            encode_cmd_code(CMD_ELSE);
            target_code[target_offset++]=(unsigned char)command_skip_len;
#endif//TINY_COMPILE
        }
    }
    return(TTN_OK);
}

int     cmd_elseif(char *data,int *index)
{
    int     if_index;
    int     elseif_index;
    int     last_elseif_index;
    int     max_elseif;
    int     command_skip_len;
//    signed long int         number1,number2;
//    int     skip_len;
//    unsigned char           op;
    unsigned long int       code_offset;
//    signed long int         command_len_address;
    signed long int         offset_address;
    signed long int         goto_offset_address;
    unsigned char           number_id1;
    unsigned char           number_id2;
	int tmp;
	int tmp1;
    
    number_id1=number_id2=mc_information.number;
    if_index=system_if_index-1;
    if(if_index<0)
    {
        strcpy(print_rec,"ELSEIF must after IF .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    if(if_table[if_index].max_elseif>=MAX_ELSEIF-1)
    {
        strcpy(print_rec,"ELSEIF too much(must < 256) .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    if(if_table[if_index].max_elseif<=0)
        if_table[if_index].max_elseif=0;
    max_elseif=if_table[if_index].max_elseif;
    if(max_elseif==0)       /* ELSEIF after IF              */
    {
        /* insert GOTO command                  */
        command_skip_len=4;
        encode_cmd_code(CMD_GOTO);

        target_code[target_offset++]=(unsigned char)command_skip_len;
        goto_offset_address=target_offset;
        target_offset += encode_code_offset(target_code + target_offset, 0);
        
        /* update couple IF jump offset         */
		/*
        code_offset=target_offset;
        offset_address=if_table[if_index].offset;
        encode_code_offset(target_code + offset_address, code_offset);*/
		
		/*modified by kent for logic expression, 2004.7.23
		  update false link
		*/
		code_offset=target_offset;
		tmp = if_table[if_index].offset;
		if (tmp != -1)
		{
			while(tmp != 0)
			{
				tmp1 = *(int *)(target_code + tmp);
				encode_code_offset(target_code + tmp, code_offset);
				tmp = tmp1;
			}
		}
	
        /*modified by kent for logic expression, 2004.7.23*/
#if 0
        /* output ELSEIF command                */
        if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
        {
            strcpy(print_rec,"ELSEIF parameter 1 must be int .");
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
        }
        number_id1 = mc_information.variable; //force variable

        if(TTN_OK != get_source_op(data, index, &op))
        {
            strcpy(print_rec,"ELSEIF parameter 2 must be one of(<,=,>,<=,!=,>=) .");
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
            op = OP_ID_EQ;
        }

        if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
        {
            strcpy(print_rec,"ELSEIF parameter 3 must be int .");
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
        }
        
        /*      ELSEIF is transfer to IF        */
        command_skip_len=0;
        encode_cmd_code(CMD_IF); //ELSEIF convert to IF

        command_len_address=target_offset;
        target_code[target_offset++]=0;
        
        skip_len=4;
        offset_address=target_offset;
        target_offset += encode_code_offset(target_code + target_offset, 0);
        command_skip_len+=skip_len;
        
        skip_len = put_number_to_target(number_id1,number1);
        command_skip_len+=skip_len;
        
        skip_len = put_op_to_target(op);
        command_skip_len+=skip_len;
        
        skip_len = put_number_to_target(number_id2,number2);
        command_skip_len+=skip_len;
        
        target_code[command_len_address]=(unsigned char)command_skip_len;

		/* update if_table                      */
        elseif_index=0;
        if_table[if_index].break_offset[elseif_index]=goto_offset_address;
        if_table[if_index].elseif_offset[elseif_index]=offset_address;
#endif

		offset_address = parselogicexp(data, index);
		elseif_index=0;
		if_table[if_index].break_offset[elseif_index]=goto_offset_address;
		if_table[if_index].elseif_offset[elseif_index]=offset_address;
	       
    }
    else                    /* ELSEIF after ELSEIF          */
    {
        /* insert GOTO command                  */
        command_skip_len=4;
        encode_cmd_code(CMD_GOTO);

        target_code[target_offset++]=(unsigned char)command_skip_len;
        goto_offset_address=target_offset;
        target_offset += encode_code_offset(target_code + target_offset, 0);
        
        /* update couple ELSEIF jump offset     
        last_elseif_index=max_elseif-1;
        code_offset=target_offset;
        offset_address=if_table[if_index].elseif_offset[last_elseif_index];
        encode_code_offset(target_code + offset_address, code_offset);*/

		/*modified by kent for logic expression, 2004.7.23
		  update false link
		*/
		last_elseif_index=max_elseif-1;
        code_offset=target_offset;
        tmp=if_table[if_index].elseif_offset[last_elseif_index];
		if (tmp != -1)
		{
			while(tmp != 0)
			{
				tmp1 = *(int *)(target_code + tmp);
				encode_code_offset(target_code + tmp, code_offset);
				tmp = tmp1;
			}
		}
				
		/*modified by kent for logic expression, 2004.7.23*/
#if 0
        /* output ELSEIF command */
        if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
        {
            strcpy(print_rec,"ELSEIF parameter 1 must be int .");
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
        }
        number_id1 = mc_information.variable;

        if(TTN_OK != get_source_op(data, index, &op))
        {
            strcpy(print_rec,"ELSEIF parameter 2 must be one of(<,=,>,<=,!=,>=) .");
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
            op = OP_ID_EQ;
        }
        if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
        {
            strcpy(print_rec,"ELSEIF parameter 3 must be int .");
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
        }
        
        command_skip_len=0;
        encode_cmd_code(CMD_IF);

        command_len_address=target_offset;
        target_code[target_offset++]=0;
        
        skip_len=4;
        offset_address=target_offset;
        target_offset += encode_code_offset(target_code + target_offset, 0);
        command_skip_len+=skip_len;
        
        skip_len=put_number_to_target(number_id1,number1);
        command_skip_len+=skip_len;
        
        skip_len=put_op_to_target(op);
        command_skip_len+=skip_len;
        
        skip_len=put_number_to_target(number_id2,number2);
        command_skip_len+=skip_len;
        
        target_code[command_len_address]=(unsigned char)command_skip_len;
        
        /* update if_table                      */
        elseif_index=max_elseif;
        if_table[if_index].break_offset[elseif_index]=goto_offset_address;
        if_table[if_index].elseif_offset[elseif_index]=offset_address;
#endif

		offset_address = parselogicexp(data, index);
		elseif_index=max_elseif;
		if_table[if_index].break_offset[elseif_index]=goto_offset_address;
		if_table[if_index].elseif_offset[elseif_index]=offset_address;
		
    }
           
    if_table[if_index].max_elseif++;
    return(TTN_OK);
}

int     cmd_endif(char *data,int *index)
{
    int     if_index;
    int     max_elseif;
    int     find_else;
    int     i;
    int     ret;
    signed long int         offset_address;
    unsigned long int       code_offset;
	int     tmp;
	int     tmp1;
    
    
    ret=TTN_OK;
    if_index=system_if_index-1;
    if(if_index<0)
    {
        strcpy(print_rec,"ENDIF must after IF .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    code_offset=target_offset;
    max_elseif=if_table[if_index].max_elseif;
    find_else=if_table[if_index].find_else;
    
    if(max_elseif<=0)       /* no ELSEIF    */
    {
        if(find_else==0)     /* no ELSE      *///(1) IF...ENDIF
        {
            /*      update  couple IF jump offset           
            offset_address=if_table[if_index].offset;
            encode_code_offset(target_code + offset_address, code_offset);*/

			/*modified by kent for logic expression, 2004.7.23
			  update false link
			*/
			tmp = if_table[if_index].offset;
			if (tmp != -1)
			{
				while(tmp != 0)
				{
					tmp1 = *(int *)(target_code + tmp);
					encode_code_offset(target_code + tmp, code_offset);
					tmp = tmp1;
				}
			}
			
        }
        else                 /* have ELSE    *///(2) IF...ELSE...ENDIF
        {
            /*      update  couple ELSE (GOTO) offset       */
            offset_address=if_table[if_index].else_offset;
            encode_code_offset(target_code + offset_address, code_offset);
        }
    }
    else                    /* have ELSEIF  *///(3) IF...ELSEIF....ELSE...ENDIF
    {
        if(find_else==0)     /* no ELSE      */
        {
            strcpy(print_rec,"ELSE needed between ELSEIF and ENDIF.");
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
            ret=TTN_ERROR;
        }
        else                 /* have ELSE    */
        {
            /*      update  couple ELSE(GOTO) offset        */
            offset_address=if_table[if_index].else_offset;
            encode_code_offset(target_code + offset_address, code_offset);
            
            /*      update couple ELSEIF(IF) jump offset    */
            for(i=0;i<max_elseif;i++)
            {
                offset_address=if_table[if_index].break_offset[i];
                encode_code_offset(target_code + offset_address, code_offset);
            }
        }
    }
    /*      push ENDIF command                      */
#ifndef TINY_COMPILE
    command_skip_len=0;
    encode_cmd_code(CMD_ENDIF);
    target_code[target_offset++]=(unsigned char)command_skip_len;
#endif//TINY_COMPILE
    
    /*      update if_table                         */
    if_table[if_index].offset=-1;
    if_table[if_index].else_offset=-1;
    if_table[if_index].max_elseif=0;
    if_table[if_index].find_else=0;
    for(i=0;i<MAX_ELSEIF;i++)
    {
        if_table[if_index].elseif_offset[i]=-1;
        if_table[if_index].break_offset[i]=-1;
    }
    system_if_index--;
    
    return(ret);
}

int     cmd_while(char *data,int *index)
{
    //char    temp[256];
    //char    temp1[256];
//    signed long int         number1,number2;
//    int     command_skip_len;
//    int     skip_len;
//    unsigned char   op;
//    unsigned char   number_id;
    signed long int goto_offset_address;
//    signed long int offset_address;
    signed long int start_offset;
//    unsigned char   number_id1;
//    unsigned char   number_id2;
    

	
	/*modified by kent for logic expression, 2004.7.23*/
#if 0
    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"WHILE parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.variable; //force variable
    
    if(TTN_OK != get_source_op(data, index, &op))
    {
        sprintf(print_rec,"WHILE parameter 2 op must be one of (<,=,>,<=,!=,>=).");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        op = OP_ID_EQ;
    }

    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"WHILE parameter 3 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    /* insert IF command            */
    start_offset=target_offset;
    command_skip_len=0;
    encode_cmd_code(CMD_IF);

    offset_address=target_offset;
    target_code[target_offset++]=(unsigned char)command_skip_len;
    
    /* insert IF jump offset        */
    goto_offset_address=target_offset;
    target_offset += encode_code_offset(target_code + target_offset, 0);
    command_skip_len=command_skip_len+4;
    
    /*      push relation exp       */
    number_id=number_id1;
    skip_len=put_number_to_target(number_id,number1);
    command_skip_len+=skip_len;
    skip_len=put_op_to_target(op);
    command_skip_len+=skip_len;
    number_id=number_id2;
    skip_len=put_number_to_target(number_id,number2);
    command_skip_len+=skip_len;
    target_code[offset_address]=(unsigned char)command_skip_len;
#endif

	start_offset=target_offset;
	goto_offset_address = parselogicexp(data, index);
    
    /*      update while_table      */
    if(system_while_index>(MAX_WHILE-1))
    {
        strcpy(print_rec,"WHILE nesting overflow .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    while_table[system_while_index].start=start_offset;
    while_table[system_while_index].offset=goto_offset_address;
    system_while_index++;
    return(TTN_OK);
}

int     cmd_break(char *data,int *index)
{
    int     while_index;
    int     command_skip_len;
    int     break_index;
    unsigned long int       offset_address;
    
    while_index=system_while_index-1;
    if(while_index<0)
    {
        strcpy(print_rec,"BREAK must after WHILE .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    /* insert GOTO command          */
    command_skip_len=4;
    encode_cmd_code(CMD_GOTO);

    target_code[target_offset++]=(unsigned char)command_skip_len;
    offset_address = target_offset;
    target_offset += encode_code_offset(target_code + target_offset, 0);
    
    /* update while_table           */
    break_index=while_table[while_index].max_break;
    if(break_index>=MAX_BREAK-1)
    {
        strcpy(print_rec,"BREAK overflow(must < 255) .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    while_table[while_index].break_offset[break_index]=offset_address;
    while_table[while_index].max_break++;
    return(TTN_OK);
}

int     cmd_exit(char *data,int *index)
{
    int     command_skip_len;
    
    command_skip_len=0;
    encode_cmd_code(CMD_EXIT);
    target_code[target_offset++]=(unsigned char)command_skip_len;
    return(TTN_OK);
}

int     cmd_endwhile(char *data,int *index)
{
    int     ret;
    int     while_index;
    int     max_break;
    int     i;
    int     command_skip_len;
    signed long int         offset_address;
    unsigned long int       code_offset;
    unsigned long int       goto_offset;
	int     tmp;
	int     tmp1;
    
    ret=TTN_OK;
    while_index=system_while_index-1;
    if(while_index<0)
    {
        strcpy(print_rec,"ENDWHILE must after WHILE .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    /*      insert GOTO command             */
    command_skip_len=4;
    encode_cmd_code(CMD_GOTO);

    target_code[target_offset++]=(unsigned char)command_skip_len;
    goto_offset=while_table[while_index].start;
    target_offset += encode_code_offset(target_code + target_offset, goto_offset);
    
    /*      update couple BREAK offset      */
    code_offset=target_offset;
    max_break=while_table[while_index].max_break;
    for(i=0;i<max_break;i++)
    {
        offset_address=while_table[while_index].break_offset[i];
        encode_code_offset(target_code + offset_address, code_offset);
    }



	/*modified by kent for logic expression, 2004.7.23*/
    /*      update couple WHILE goto offset */
    /*offset_address=while_table[while_index].offset;
    encode_code_offset(target_code + offset_address, code_offset);*/


	/*modified by kent for logic expression, 2004.7.23
			  update false link
	*/
	tmp = while_table[while_index].offset;
	if (tmp != -1)
	{
		while(tmp != 0)
		{
			tmp1 = *(int *)(target_code + tmp);
			encode_code_offset(target_code + tmp, code_offset);
			tmp = tmp1;
		}	
	}

    
    /*      output ENDWHILE command         */
#ifndef TINY_COMPILE
    command_skip_len=0;
    encode_cmd_code(CMD_ENDWHILE);
    target_code[target_offset++]=(unsigned char)command_skip_len;
#endif//TINY_COMPILE
    
    /*      update while_table              */
    while_table[while_index].offset=-1;
    while_table[while_index].start=-1;
    for(i=0;i<MAX_BREAK;i++)
        while_table[while_index].break_offset[i]=-1;
    while_table[while_index].max_break=0;
    system_while_index--;
    
    return(TTN_OK);
}

int     cmd_switch(char *data,int *index)
{
    signed long int     number;
    
    if(system_switch_index>MAX_SWITCH-1)
    {
        strcpy(print_rec,"SWITCH nesting overflow(>255) .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    /* get SWITCH parameter(int) */
    if(TTN_OK != get_source_number(data, index, &number, NULL))
    {
        strcpy(print_rec,"SWITCH parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    /* push SWITCH command */
#ifndef TINY_COMPILE
    command_skip_len=0;
    encode_cmd_code(CMD_SWITCH);
    target_code[target_offset++]=(unsigned char)command_skip_len;
#endif//TINY_COMPILE
    
    
    /* update  switch_table */
    switch_table[system_switch_index].number=number;
    system_switch_index++;
    
    return(TTN_OK);
}

int cmd_case(char *data,int *index)
{
    int     switch_index;
    int     case_index;
    int     find_case;
    signed long int number;
    signed long int number1;
    int     command_skip_len;
    int     skip_len;
    unsigned char           op;
    unsigned char           number_id;
    unsigned long int       offset_address;
    unsigned long int       command_len_address;
    
    
    switch_index=system_switch_index-1;
    if(switch_index<0)
    {
        strcpy(print_rec,"CASE must after SWITCH .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    case_index=switch_table[switch_index].max_case;
    if(case_index>=MAX_CASE-1)
    {
        strcpy(print_rec,"CASE overflow(must < 255) .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    find_case=switch_table[switch_index].find_case;
    if(find_case==1)
    {
        strcpy(print_rec,"ENDCASE needed before .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    switch_table[switch_index].find_case=1;
    
    /*      get CASE paremeter (int)                */
    if(TTN_OK != get_source_number(data, index, &number1, NULL))
    {
        strcpy(print_rec,"CASE parameter must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    op = OP_ID_EQ;
    number = switch_table[switch_index].number;
    
    /*      insert IF (op=EQ) command               */
    command_skip_len=0;
    encode_cmd_code(CMD_IF);

    command_len_address=target_offset;
    target_code[target_offset++]=(unsigned char)command_skip_len;
    
    offset_address=target_offset;
    target_offset += encode_code_offset(target_code + target_offset, 0);
    command_skip_len+=4;
    
    number_id=mc_information.variable; //force variable
    skip_len=put_number_to_target(number_id,number);
    command_skip_len+=skip_len;
    
    skip_len=put_op_to_target(op);
    command_skip_len+=skip_len;
    
    number_id=mc_information.number;
    skip_len=put_number_to_target(number_id,number1);
    command_skip_len+=skip_len;
    target_code[command_len_address]=(unsigned char)command_skip_len;
    
    
    /*      push CASE command               */
#ifndef TINY_COMPILE
    command_skip_len=0;
    encode_cmd_code(CMD_CASE);

    target_code[target_offset++]=(unsigned char)command_skip_len;
#endif//TINY_COMPILE
    
    /*      update switch_table             */
    switch_table[switch_index].case_offset[case_index]=offset_address;
    switch_table[switch_index].max_case++;
    
    return(TTN_OK);
}

int     cmd_endcase(char *data,int *index)
{
    int     switch_index;
    int     case_index;
    int     find_case;
    int     command_skip_len;
    unsigned long int       case_offset_address;
    unsigned long int       code_offset;
    unsigned long int       offset_address;
    unsigned long int       command_len_address;
    
    switch_index=system_switch_index-1;
    if(switch_index<0)
    {
        strcpy(print_rec,"CASE must after SWITCH .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    case_index=switch_table[switch_index].max_case-1;
    if(case_index<0)
    {
        strcpy(print_rec,"ENDCASE must after CASE .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    find_case=switch_table[switch_index].find_case;
    if(find_case==0)
    {
        strcpy(print_rec,"ENDCASE must after CASE .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    switch_table[switch_index].find_case=0;
    
    
    /*      insert GOTO command                     */
    command_skip_len=0;
    encode_cmd_code(CMD_GOTO);

    command_len_address=target_offset;
    target_code[target_offset++]=(unsigned char)command_skip_len;
    offset_address=target_offset;
    target_offset += encode_code_offset(target_code + target_offset, 0);
    command_skip_len+=4;
    target_code[command_len_address]=(unsigned char)command_skip_len;
    
    
    /*      update couple CASE(IF) jump offset      */
    case_offset_address=switch_table[switch_index].case_offset[case_index];
    code_offset=target_offset;
    encode_code_offset(target_code + case_offset_address, code_offset);
    
    
    /*      push ENDCASE command                    */
#ifndef TINY_COMPILE
    command_skip_len=0;
    encode_cmd_code(CMD_ENDCASE);
    target_code[target_offset++]=(unsigned char)command_skip_len;
#endif//TINY_COMPILE
    
    /*      update  switch_table            */
    switch_table[switch_index].endcase_offset[case_index]=offset_address;
    
    return(TTN_OK);
    
}

int     cmd_endswitch(char *data,int *index)
{
    int     ret;
    int     switch_index;
    int     max_case;
    int     i;
    int     find_case;
    unsigned long int       code_offset;
    unsigned long int       offset_address;
    
    ret=TTN_OK;
    switch_index=system_switch_index-1;
    if(switch_index<0)
    {
        printf(print_rec,"ENDSWITCH must after SWITCH .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        ret=TTN_ERROR;
    }
    find_case=switch_table[switch_index].find_case;
    if(find_case==1)
    {
        strcpy(print_rec,"ENDCASE and CASE  not matching .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        ret=TTN_ERROR;
    }
    
    /*      update  ENDCASE offset          */
    code_offset=target_offset;
    max_case=switch_table[switch_index].max_case;
    for(i=0;i<max_case;i++)
    {
        offset_address=switch_table[switch_index].endcase_offset[i];
        encode_code_offset(target_code + offset_address, code_offset);
    }
    
    /*      push ENDSWITCH command          */
#ifndef TINY_COMPILE
    command_skip_len=0;
    encode_cmd_code(CMD_ENDSWITCH);
    target_code[target_offset++]=(unsigned char)command_skip_len;
#endif//TINY_COMPILE
    /*      update switch_table             */
    switch_table[switch_index].number=0;
    switch_table[switch_index].max_case=0;
    switch_table[switch_index].find_case=0;
    switch_table[switch_index].default_offset=-1;
    for(i=0;i<MAX_CASE;i++)
    {
        switch_table[switch_index].case_offset[i]=-1;
        switch_table[switch_index].endcase_offset[i]=-1;
    }
    system_switch_index--;
    
    return(ret);
}

int     cmd_goto(char *data,int *index)
{
    int     ret;
    char    temp[256];
    int     label_index;
    int     command_skip_len;
    signed long int offset_address;
    
    ret=TTN_OK;
    memset(temp,0x00,256);
    skip_space(data,index);
    get_whole_word(data,temp,index,255);
    label_index=search_label_index(temp);
    if(label_index<0)
    {
        sprintf(print_rec,"GOTO label '%s' not found .",temp);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    command_skip_len=4;
    encode_cmd_code(CMD_GOTO);
    target_code[target_offset++]=(unsigned char)command_skip_len;
    offset_address=target_offset;
    
    target_offset += encode_code_offset(target_code + target_offset, 0);
    
    ret=register_goto(temp,offset_address);
    return(ret);
}

int     register_goto(char *label_name,signed long int offset_address)
{
    int     i;
    int     index;
    index=-1;
    for(i=0;i<MAX_GOTO;i++)
    {
        if(goto_table[i].flag=='0')
        {
            index=i;
            i=MAX_GOTO+100;
        }
    }
    if(index<0)
    {
        sprintf(print_rec,"GOTO overflow(>%d) .",MAX_GOTO);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    strcpy(goto_table[index].name,label_name);
    goto_table[index].offset_address=offset_address;
    goto_table[index].flag='1';
    return(TTN_OK);
}

int     cmd_call(char *data,int *index)
{
    int     command_skip_len;
    int     skip_len;
    signed long int number;
    signed long int offset_address;
    unsigned char number_id;

    if(TTN_OK != get_source_number(data, index, &number, &number_id))
    {
        sprintf(print_rec,"CALL parameter must be int.");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    if((number<0)||(number>MAX_PROC-1))
    {
        sprintf(print_rec,"CALL proc %d out of range(0~%d) .",number,MAX_PROC-1);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    
    command_skip_len=0;
    encode_cmd_code(CMD_CALL);

    offset_address=target_offset;
    target_code[target_offset++]=(unsigned char)command_skip_len;
    
    //number_id=mc_information.number;
    skip_len=put_number_to_target(number_id,number);
    command_skip_len+=skip_len;
    target_code[offset_address]=(unsigned char)command_skip_len;
    
    register_call(number,system_line_number);
    return(TTN_OK);
}

int register_call(int call_proc,int line_number)
{
    int     i;
    int     index;
    index=-1;
    for(i=0;i<MAX_CALL;i++)
    {
        if(call_table[i].flag=='0')
        {
            index=i;
            i=MAX_CALL+100;
        }
    }
    if(index<0)
    {
        strcpy(print_rec,"CALL overflow(must < 255) .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        return(TTN_ERROR);
    }
    call_table[index].call_proc=call_proc;
    call_table[index].source_line_number=line_number;
    call_table[index].flag='1';
    return(TTN_OK);
}

int     cmd_return(char *data,int *index)
{
    int     command_skip_len;
    
    command_skip_len=0;
    encode_cmd_code(CMD_RETURN);
    target_code[target_offset++]=(unsigned char)command_skip_len;
    return(TTN_OK);
}

int     cmd_exec(char *data,int *index)
{
    char    temp[256];
    int     command_skip_len;
    int     skip_len;
    signed long int offset_address;
    signed long int number;
    unsigned char   number_id;
    
    encode_cmd_code(CMD_EXEC);

    offset_address=target_offset;
    target_code[target_offset++]=0;
    
    memset(temp,0x00,256);
    skip_space(data,index);
    get_whole_word(data,temp,index,255);
    if(temp[0] == 0)
    {
        sprintf(print_rec, "EXEC parameter 1 error(must be word)");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }

    command_skip_len=0;
    skip_len = put_word_to_target(temp);
    command_skip_len += skip_len;

    if(TTN_OK != get_source_number(data, index, &number, &number_id))
    {
        sprintf(print_rec,"EXEC parameter 2 error(must be int)");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id =  mc_information.number; // force number

    skip_len = put_number_to_target(number_id,number);
    command_skip_len += skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;
    return(TTN_OK);
}


int     cmd_var(char *data,int *index)
{
    int     ret;
    signed long int     number1,number2;
    int     command_skip_len;
    int     skip_len;
    signed long int offset_address;
    unsigned char   number_id1;
    unsigned char   number_id2;
    
    ret=TTN_OK;
    
    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"VAR parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.number; //force number

    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"VAR parameter 2 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    command_skip_len=0;
    encode_cmd_code(CMD_VAR);

    offset_address=target_offset;
    target_code[target_offset++]=0;
    
    skip_len=put_number_to_target(number_id1,number1);
    command_skip_len+=skip_len;
    
    skip_len=put_number_to_target(number_id2,number2);
    command_skip_len+=skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;
    return(ret);
}

int cmd_add(char *data,int *index)
{
    int     ret;
    signed long int     number1,number2;
    int     command_skip_len;
    int     skip_len;
    signed long int offset_address;
    unsigned char   number_id1;
    unsigned char   number_id2;
    
    ret=TTN_OK;

    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"ADD parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.number; //force number
    
    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"ADD parameter 2 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    command_skip_len=0;
    encode_cmd_code(CMD_ADD);

    offset_address=target_offset;
    target_code[target_offset++]=0;
    
    skip_len=put_number_to_target(number_id1,number1);
    command_skip_len+=skip_len;
    
    skip_len=put_number_to_target(number_id2,number2);
    command_skip_len+=skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;
    return(ret);
}

int cmd_sub(char *data,int *index)
{
    int ret;
    long number1,number2;
    int     command_skip_len;
    int     skip_len;
    signed long int offset_address;
    unsigned char   number_id;
    unsigned char   number_id1;
    unsigned char   number_id2;
    
    ret = TTN_OK;
    
    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"SUB parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.number; //force number
    
    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"SUB parameter 2 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    command_skip_len=0;
    encode_cmd_code(CMD_SUB);

    offset_address=target_offset;
    target_code[target_offset++]=0;
    
    number_id=number_id1;
    skip_len=put_number_to_target(number_id,number1);
    command_skip_len+=skip_len;
    
    number_id=number_id2;
    skip_len=put_number_to_target(number_id,number2);
    command_skip_len+=skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;
    return(ret);
}

int cmd_mul(char *data,int *index)
{
    int     ret;
    long    number1,number2;
    int     command_skip_len;
    int     skip_len;
    signed long int offset_address;
    unsigned char   number_id1;
    unsigned char   number_id2;
    
    ret=TTN_OK;

    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"MUL parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.number; //force number
    
    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"MUL parameter 2 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    command_skip_len=0;
    encode_cmd_code(CMD_MUL);

    offset_address=target_offset;
    target_code[target_offset++]=0;
    
    skip_len=put_number_to_target(number_id1,number1);
    command_skip_len+=skip_len;
    
    skip_len=put_number_to_target(number_id2,number2);
    command_skip_len+=skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;
    return(ret);
}

int cmd_dev(char *data,int *index)
{
    int     ret;
    long    number1,number2;
    int     command_skip_len;
    int     skip_len;
    signed long int offset_address;
    unsigned char   number_id;
    unsigned char   number_id1;
    unsigned char   number_id2;
    
    ret=TTN_OK;

    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"DEV parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.number; //force number
    
    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"DEV parameter 2 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    if((number2==0)&&(number_id2==mc_information.number))
    {
        strcpy(print_rec,"DEV parameter 2 can't be zero(0) .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    command_skip_len=0;
    encode_cmd_code(CMD_DEV);

    offset_address=target_offset;
    target_code[target_offset++]=0;
    
    number_id=number_id1;
    skip_len=put_number_to_target(number_id,number1);
    command_skip_len+=skip_len;
    
    number_id=number_id2;
    skip_len=put_number_to_target(number_id,number2);
    command_skip_len+=skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;
    return(ret);
}

int cmd_rand(char *data,int *index)
{
    int     ret;
    long    number1,number2;
    int     command_skip_len;
    int     skip_len;
    signed long int offset_address;
    unsigned char   number_id;
    unsigned char   number_id1;
    unsigned char   number_id2;
    
    ret=TTN_OK;

    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"RAND parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.number; //force number
    
    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"RAND parameter 2 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    if((number2==0)&&(number_id2==mc_information.number))
    {
        strcpy(print_rec,"RAND parameter 2 can't be zero(0) .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    command_skip_len=0;
    encode_cmd_code(CMD_RAND);

    offset_address=target_offset;
    target_code[target_offset++]=0;
    
    number_id=number_id1;
    skip_len=put_number_to_target(number_id,number1);
    command_skip_len+=skip_len;
    
    number_id=number_id2;
    skip_len=put_number_to_target(number_id,number2);
    command_skip_len+=skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;
    return(ret);
}

int cmd_mod(char *data,int *index)
{
    int     ret;
    long    number1,number2;
    int     command_skip_len;
    int     skip_len;
    signed long int offset_address;
    unsigned char   number_id;
    unsigned char   number_id1;
    unsigned char   number_id2;
    
    ret=TTN_OK;

    if(TTN_OK != get_source_number(data, index, &number1, &number_id1))
    {
        strcpy(print_rec,"Mod parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    number_id1 = mc_information.number; //force number
    
    if(TTN_OK != get_source_number(data, index, &number2, &number_id2))
    {
        strcpy(print_rec,"Mod parameter 2 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    if((number2==0)&&(number_id2==mc_information.number))
    {
        strcpy(print_rec,"MOd parameter 2 can't be zero(0) .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
    }
    
    command_skip_len=0;
    encode_cmd_code(CMD_MOD);

    offset_address=target_offset;
    target_code[target_offset++]=0;
    
    number_id=number_id1;
    skip_len=put_number_to_target(number_id,number1);
    command_skip_len+=skip_len;
    
    number_id=number_id2;
    skip_len=put_number_to_target(number_id,number2);
    command_skip_len+=skip_len;
    
    target_code[offset_address]=(unsigned char)command_skip_len;
    return(ret);
}

int     update_target_code(void)
{
    update_all_goto();
    return(TTN_OK);
}

int     update_all_goto(void)
{
    int     i;
    char    name[256];
    int     label_index;
    signed long int         offset_address;
    unsigned long int       goto_offset;
    
    for(i=0;i<MAX_GOTO;i++)
    {
        if(goto_table[i].flag=='1')
        {
            memset(name,0x00,256);
            strcpy(name,goto_table[i].name);
            offset_address=goto_table[i].offset_address;
            label_index=search_label_index(name);
            if(label_index>=0)
            {
                goto_offset=label_table[label_index].offset;
                if(goto_offset>=0)
                {
                    encode_code_offset(target_code + offset_address, goto_offset);
                }
            }
        }
    }
    return(TTN_OK);
}

int     search_label_index(char *name)
{
    int     index;
    int     i;
    index=-1;
    for(i=0;i<MAX_LABEL;i++)
    {
        if(label_table[i].flag=='1')
        {
            if(strcmpi(label_table[i].name,name)==0)
            {
                index=i;
                i=MAX_LABEL+100;
            }
        }
    }
    return(index);
}


int     pass3_check(void)
{
    int ret;

    ret = TTN_OK;

    printf("Process Pass3 check ...............................\n");
    if(TTN_OK != check_all_proc())
        ret = TTN_NOT_OK;

   // if(TTN_OK != check_all_call())
   //     ret = TTN_NOT_OK;

    printf("        Pass3 check finished !\n");

    return  ret;
}


int     check_all_call(void)
{
    int     i;
    int     call_proc;
    int     line_number;
    signed long int proc_address;
    
    for(i=0;i<MAX_CALL;i++)
    {
        if(call_table[i].flag=='1')
        {
            call_proc=call_table[i].call_proc;
            line_number=call_table[i].source_line_number;
            proc_address=pmac_head->proc_address[call_proc];
            if(proc_address<0)
            {
                sprintf(print_rec,"CALL proc %d not found .",call_proc);
                output_message(OUTPUT_ERROR,line_number,print_rec);
            }
        }
    }
    return(TTN_OK);
}

int     check_all_couple(void)
{
    int ret;

    ret = TTN_OK;
    if(proc_start_flag!=0)
    {
        strcpy(print_rec,"PROC not end .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        ret = TTN_NOT_OK;
    }
    if(system_if_index!=0)
    {
        strcpy(print_rec,"IF and ENDIF not matching .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        ret = TTN_NOT_OK;
    }
    if(system_while_index!=0)
    {
        strcpy(print_rec,"WHILE and ENDWHILE not matching .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        ret = TTN_NOT_OK;
    }
    if(system_switch_index!=0)
    {
        strcpy(print_rec,"SWITCH and ENDSWITCH not matching .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        ret = TTN_NOT_OK;
    }

    return(ret);
}


int     check_all_proc(void)
{
    int     i;
    int     find;
    int     ret;
    
    ret = TTN_OK;
    find = 0;
    for(i=0;i<MAX_PROC;i++)
    {
        if(pmac_head->proc_address[i]>=0)
            find++;
    }
    if(find==0)
    {
        strcpy(print_rec,"Have no PROC .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
        ret = TTN_NOT_OK;
    }
    return  ret;
}


unsigned char   check_op(char *data)
{
    char    op_word[12][8]=
    {
        {OP_WORD_EQ    },
        {OP_WORD_LT    },
        {OP_WORD_GT    },
        {OP_WORD_LE    },
        {OP_WORD_GE    },
        {OP_WORD_NE    },
        {OP_WORD_ADD   },
        {OP_WORD_SUB   },
        {OP_WORD_MUL   },
        {OP_WORD_DEV   },
        {OP_WORD_MOD   },
        {OP_WORD_POWER }
    };
    unsigned char   op_id[12]=
    {
        OP_ID_EQ   ,      /* == */
        OP_ID_LT   ,      /* <  */
        OP_ID_GT   ,      /* >  */
        OP_ID_LE   ,      /* <= */
        OP_ID_GE   ,      /* >= */
        OP_ID_NE   ,      /* != */
        OP_ID_ADD  ,      /* +  */
        OP_ID_SUB  ,      /* -  */
        OP_ID_MUL  ,      /* *  */
        OP_ID_DEV  ,      /* /  */
        OP_ID_MOD  ,      /* %  */
        OP_ID_POWER       /* ^  */
    };
    unsigned char   op;
    int     i;
    
    op=OP_ID_UNKNOWN;
    for(i=0;i<12;i++)
    {
        if(strcmpi(data,op_word[i])==0)
        {
            op=op_id[i];
            i=12+100;
        }
    }
    return(op);
}

int put_op_to_target(unsigned char op_id)
{
    const int encode_size = 1;
    target_code[target_offset++]=(unsigned char)op_id;
    return encode_size;
}

int encode_cmd_code(unsigned long cmd_code)
{
    //Jim, change [2/18/2004]
    //{{{
    // new version
    const int encode_size = 2;
    target_code[target_offset++] = (unsigned char)(cmd_code & 0xff);
    target_code[target_offset++] = (unsigned char)((cmd_code & 0xff00) >> 8);
    return encode_size;

    /* old version
    const int encode_size = 1;
    target_code[target_offset++] = (unsigned char)cmd_code;
    return encode_size;
    */
    //}}}
}

void get_whole_word(char *in,char *out,int *idx,int max)
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
        if(ch==' '||ch==','||ch==0x09)
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


int get_source_number(char *source, int *source_index, 
                      signed long int *number, unsigned char *number_id)
{
    char temp[256], temp1[256];
    int num;
    unsigned char id;

    num = 0;
    memset(temp, 0x00, 256);
    memset(temp1, 0x00, 256);
    skip_space(source, source_index);
    get_whole_word(source, temp, source_index, 255);
    if(temp[0] != 0x00)
    {
        if( (temp[0] == mc_information.number)
            || (temp[0]==mc_information.variable))
        {
            replace_define(temp,temp1);
            num = string_2_number(&temp1[1]);
            id = temp1[0];
        }
        else
        {
			replace_define(temp,temp1);
			
			num = string_2_number(&temp1[0]);
        	id = mc_information.number;
        }

        if(number) *number = num;
        if(number_id) *number_id = id;

        return TTN_OK;
    }
    else
    {
        return TTN_ERROR;
    }
}

int get_source_op(char *source, int *source_index, unsigned char *op)
{
    char temp[256];
    unsigned char my_op;
    int ret;

    ret = TTN_NOT_OK;
    my_op = OP_ID_EQ;
    memset(temp,0x00,256);
    skip_space(source, source_index);
    get_whole_word(source, temp, source_index, 255);
    if(temp[0]!=0x00)
    {
        my_op = check_op(temp);
        if((my_op==OP_ID_EQ) || (my_op==OP_ID_LT) || (my_op==OP_ID_GT)
            || (my_op==OP_ID_LE) || (my_op == OP_ID_GE) || (my_op==OP_ID_NE))
        {
            if(op) *op = my_op;
            ret = TTN_OK;
        }
        else
        {
            if(op) *op = my_op;
        }
    }
    else
    {
        if(op) *op = my_op;
    }
    return ret;
}


//
//@para     fp --> file ptr of opened target file
//@para     phead --> ptr to file header
//@return   int --> size of header size
//
int write_target_file_header(FILE *fp, MAC_HEAD *phead)
{
    MAC_HEAD base_head;
    int head_size;
    unsigned short max_proc, i;

    if(!phead || !fp)
        return 0;

    // Count valid procs
    //
    head_size = 0;
    max_proc = 0;
    for(i=0; i<phead->max_proc; i++)
    {
        if(phead->proc_address[i] >= 0)
        {
            max_proc++;
        }
    }

    // Initialize & write base header
    //
    base_head = *phead;
    base_head.max_proc = max_proc;
    base_head.head_size = sizeof(*pmac_head) - sizeof(signed long int) //size of base header
        + max_proc * (sizeof(unsigned short) + sizeof(signed long int));   //size of proc index-address pairs
    fwrite(&base_head, 1, sizeof(base_head) - sizeof(signed long int), fp);

    // Write valid proc address pair
    //
    for(i=0; i<phead->max_proc; i++)
    {
        if(phead->proc_address[i] >= 0)
        {
            fwrite(&i, 1, sizeof(unsigned short), fp);
            fwrite(&phead->proc_address[i], 1, sizeof(signed long int), fp);
        }
    }

    return base_head.head_size;
}


int encode_code_offset(unsigned char *buf, unsigned long code_offset)
{
    *buf++ = (unsigned char) ((code_offset>> 0) & 0x000000ff);
    *buf++ = (unsigned char) ((code_offset>> 8) & 0x000000ff);
    *buf++ = (unsigned char) ((code_offset>>16) & 0x000000ff);
    *buf++ = (unsigned char) ((code_offset>>24) & 0x000000ff);
    return 4; //size of encode bytes
}
