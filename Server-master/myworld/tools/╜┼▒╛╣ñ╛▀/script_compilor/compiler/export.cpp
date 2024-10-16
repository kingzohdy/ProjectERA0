/*
**  EXPORT.C
**  Export Header functions in MC.
**
**  ZJian created,1999/01/29.
**  
**  ZJian modified,1999/05/31
**      -- Modify export_header_file switch by (compiler_config.export_header)
*/
#include "mchead.h"

#define EXPORT_FILENAME         "mcdef.h"
#define PART1_LINES             18
#define PART2_LINES             03
#define PART3_LINES             21

extern  char    print_rec[LINE_BUFFER_SIZE];
LINE_DNODE    * export_line             = NULL;

char    export_part1[PART1_LINES][256]={
    "//                                                                  ",//00
        "// define area -----------------------------------------------------",//01
        "//                                                                  ",//02
        "#define MAX_KEYWORD_LENGTH              32                          ",//03
        "#define MAX_PARAMETER                   32                          ",//04
        "#define MAX_COMMAND                     1024                        ",//05
        "#define MAX_VAR                         4096    // User can change  ",//06
        "#define MAX_PROC                        1024                        ",//07
        "#define DUMMY_TYPE                      '0'                         ",//08
        "#define INT_TYPE                        '1'                         ",//09
        "#define WORD_TYPE                       '2'                         ",//10
        "#define STR_TYPE                        '3'                         ",//11
        "                                                                    ",//12
        "//                                                                  ",//13
        "// macro command =================================                  ",//14
        "//                                                                  ",//15
        "// system use command --------------------                          ",//16
        "//                                                                  ",//17
};

char    export_part2[PART2_LINES][256]={
    "//                                                                  ",//00
        "// user use command ----------------------                          ",//01
        "//                                                                  ",//02
};

char    export_part3[PART3_LINES][256]={
    "                                                                    ",//00
        "//                                                                  ",//01
        "// typedef area ----------------------------------------------------",//02
        "//                                                                  ",//03
        "typedef struct  MAC_INFO_STRUCT                                     ",//04
        "{                                                                   ",//05
        "   char                 filename[80];                               ",//06
        "   unsigned long int    code_size;                                  ",//07
        "   unsigned long int    code_index;                                 ",//08
        "   signed long int      variable[MAX_VAR];                          ",//09
        "} MAC_INFO;                                                         ",//10
        "                                                                    ",//11
        "typedef struct  MC_HEAD_STRUCT                                      ",//12
        "{                                                                   ",//13
        "   unsigned char        copyright[75];                              ",//14
        "   unsigned char        version;                                    ",//15
        "   unsigned long int    key_check;                                  ",//16
        "   signed long int      proc_address[MAX_PROC];                     ",//17
        "} MC_HEAD;                                                          ",//18
        "                                                                    ",//19
        "                                                                    ",//20
};



void    init_export_line(void);
void    clear_export_line(void);
void    export_header_start(void);
void    export_header_end(void);
void    export_header_part1(void);
void    export_header_part2(void);
void    export_header_part3(void);
void    export_header_command_define1(void);
void    export_header_command_define2(void);
int     export_header_file(void);



int     export_header(void)
{
    int     ret;
    char    mask;
    int     val;
    
    ret=TTN_OK;
    mask=compiler_config.export_header;
    if(mask=='0')
        return(ret);
    
    printf("Process Export header file ........................\n");
    init_export_line();
    export_header_start();                  // header start
    if(mask=='1')
    {
        export_header_part1();               // define & system command id
        export_header_command_define1();     // system command define
    }
    export_header_part2();                  // user command id
    export_header_command_define2();        // user command define
    if(mask=='1')
        export_header_part3();               // typedef
    export_header_end();                    // header end
    val=export_header_file();
    clear_export_line();
    if(val)
    {
        sprintf(print_rec,"        FileName : %s\n",EXPORT_FILENAME);
        printf(print_rec);
        sprintf(print_rec,"Export header file '%s' OK !\n",EXPORT_FILENAME);
        printf(print_rec);
        printf("\n");
    }
    else
    {
        sprintf(print_rec,"Same header file , no update !\n");
        printf(print_rec);
        printf("\n");
    }
    return(ret);
}


void    init_export_line(void)
{
    export_line=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
    if(export_line==NULL)
    {
        printf("Memory alloc error !!!\n");
        exit(-1);
    }
    export_line->last=NULL;
    export_line->next=NULL;
    memset(export_line->buffer,0x00,LINE_BUFFER_SIZE);
}

void    clear_export_line(void)
{
    LINE_DNODE      *p,*s;
    p=export_line;
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

void    export_header_start(void)
{
    LINE_DNODE      *h,*p,*s;
    h=export_start;
    p=export_line;
    while(p->next!=NULL)
        p=p->next;
    while(h->next!=NULL)
    {
        h=h->next;
        
        s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(s==NULL)
        {
            printf("Memory alloc error !!!\n");
            exit(-1);
        }
        s->last=NULL;
        s->next=NULL;
        memset(s->buffer,0x00,LINE_BUFFER_SIZE);
        strcpy((char *)s->buffer,(const char *)h->buffer);
        
        p->next=s;
        s->last=p;
        p=p->next;
    }
}

void    export_header_end(void)
{
    LINE_DNODE      *h,*p,*s;
    h=export_end;
    p=export_line;
    while(p->next!=NULL)
        p=p->next;
    while(h->next!=NULL)
    {
        h=h->next;
        
        s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(s==NULL)
        {
            printf("Memory alloc error !!!\n");
            exit(-1);
        }
        s->last=NULL;
        s->next=NULL;
        memset(s->buffer,0x00,LINE_BUFFER_SIZE);
        strcpy((char *)s->buffer,(const char *)h->buffer);
        
        p->next=s;
        s->last=p;
        p=p->next;
    }
}

void    export_header_part1(void)
{
    LINE_DNODE      *p,*s;
    int     i;
    p=export_line;
    while(p->next!=NULL)
        p=p->next;
    for(i=0;i<PART1_LINES;i++)
    {
        s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(s==NULL)
        {
            printf("Memory alloc error !!!\n");
            exit(-1);
        }
        s->last=NULL;
        s->next=NULL;
        memset(s->buffer,0x00,LINE_BUFFER_SIZE);
        strcpy((char *)s->buffer,(const char *)&export_part1[i][0]);
        
        p->next=s;
        s->last=p;
        p=p->next;
    }
}

void    export_header_part2(void)
{
    LINE_DNODE      *p,*s;
    int     i;
    p=export_line;
    while(p->next!=NULL)
        p=p->next;
    for(i=0;i<PART2_LINES;i++)
    {
        s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(s==NULL)
        {
            printf("Memory alloc error !!!\n");
            exit(-1);
        }
        s->last=NULL;
        s->next=NULL;
        memset(s->buffer,0x00,LINE_BUFFER_SIZE);
        strcpy((char *)s->buffer,(const char *)&export_part2[i][0]);
        
        p->next=s;
        s->last=p;
        p=p->next;
    }
}

void    export_header_part3(void)
{
    LINE_DNODE      *p,*s;
    int     i;
    p=export_line;
    while(p->next!=NULL)
        p=p->next;
    for(i=0;i<PART3_LINES;i++)
    {
        s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
        if(s==NULL)
        {
            printf("Memory alloc error !!!\n");
            exit(-1);
        }
        s->last=NULL;
        s->next=NULL;
        memset(s->buffer,0x00,LINE_BUFFER_SIZE);
        strcpy((char *)s->buffer,(const char *)&export_part3[i][0]);
        
        p->next=s;
        s->last=p;
        p=p->next;
    }
}

void    export_header_command_define1(void)
{
    LINE_DNODE      *p,*s;
    int     i;
    char    temp[256];
    
    p=export_line;
    while(p->next!=NULL)
        p=p->next;
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag=='1')
        {
            if(command_table[i].attrib==COMMAND_ATTRIB_SYSTEM)
            {
                sprintf(temp,"CMD_%s                                      ",command_table[i].keyword);
                temp[36]=0x00;
                memset(print_rec,0x00,LINE_BUFFER_SIZE);
                sprintf(print_rec,"#define %s    0x%04x   ",temp,command_table[i].command_code);
                
                s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
                if(s==NULL)
                {
                    printf("Memory alloc error !!!\n");
                    exit(-1);
                }
                s->last=NULL;
                s->next=NULL;
                memset(s->buffer,0x00,LINE_BUFFER_SIZE);
                strcpy((char *)s->buffer,(const char *)print_rec);
                
                p->next=s;
                s->last=p;
                p=p->next;
            }
        }
    }
    return;
}

void    export_header_command_define2(void)
{
    LINE_DNODE      *p,*s;
    int     i;
    char    temp[256];
    
    p=export_line;
    while(p->next!=NULL)
        p=p->next;
    for(i=0;i<MAX_COMMAND;i++)
    {
        if(command_table[i].flag=='1')
        {
            if(command_table[i].attrib==COMMAND_ATTRIB_USER)
            {
                sprintf(temp,"CMD_%s                                      ",command_table[i].keyword);
                temp[36]=0x00;
                memset(print_rec,0x00,LINE_BUFFER_SIZE);
                sprintf(print_rec,"#define %s    0x%04x   ",temp,command_table[i].command_code);
                
                s=(LINE_DNODE *)malloc(sizeof(struct LINE_DNODE_STRUCT));
                if(s==NULL)
                {
                    printf("Memory alloc error !!!\n");
                    exit(-1);
                }
                s->last=NULL;
                s->next=NULL;
                memset(s->buffer,0x00,LINE_BUFFER_SIZE);
                strcpy((char *)s->buffer,(const char *)print_rec);
                
                p->next=s;
                s->last=p;
                p=p->next;
            }
        }
    }
    return;
}

int     export_header_file(void)
{
    FILE    *fp;
    LINE_DNODE      *p;
    char    temp_filename[_MAX_PATH+_MAX_FNAME];
    char    export_filename[_MAX_PATH+_MAX_FNAME];
    char    *buffer1=NULL;
    char    *buffer2=NULL;
    int     size1,size2,i;
    int     replace_flag;
    
    printf("\n");
    printf("Starting export header file ...\n");

    sprintf((char *)temp_filename,"%s%c%s",compiler_config.header_dir,
                DIR_SEPARATE_CHAR, "mcdef.h.tmp");
    remove((const char *)temp_filename);

    fp=fopen((const char *)temp_filename,"w");
    if(fp==NULL)
    {
        printf("Export file %s open error !!!\n",temp_filename);
        exit(-7);
    }
    p=export_line;
    while(p->next!=NULL)
    {
        p=p->next;
        memset(print_rec,0x00,LINE_BUFFER_SIZE);
        strcpy((char *)print_rec,(const char *)p->buffer);
        fputs((const char *)print_rec,fp);
        fputs("\n",fp);
    }
    fclose(fp);

   
    sprintf((char *)export_filename,"%s%c%s", compiler_config.header_dir, 
                DIR_SEPARATE_CHAR, EXPORT_FILENAME);
    
    printf("   open file %s for compare defference ...\n", export_filename);

    fp=fopen((const char *)export_filename,"rb");
    if(fp==NULL)
        goto error;
    else
    {
        //jim,2004.2.9. changed.
        //size1 = filelength(fileno(fp));
        fseek(fp, 0L, SEEK_SET);
        fseek(fp, 0L, SEEK_END);
        size1 = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
  
        buffer1 = (char *)malloc(size1*4);  //align 4
        if(buffer1==NULL)
            goto error;
        else
            fread(buffer1,1,size1,fp);
        fclose(fp);
    }

    fp=fopen((const char *)temp_filename,"rb");
    
    //jim,2004.2.9. changed
    //size2 = filelength(fileno(fp));
    fseek(fp, 0L, SEEK_SET);
    fseek(fp, 0L, SEEK_END);
    size2 = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    buffer2 = (char *)malloc(size2*4);  //align 4
    if(buffer2==NULL)
        goto error;
    fread(buffer2,1,size2,fp);
    if(fp)
    {
        fclose(fp);
        fp = NULL;
    }
    if(size1 != size2)
        goto error;
    replace_flag=0;
    for(i=0;i<size1;i++)
    {
        if(buffer1[i] != buffer2[i])
        {
            replace_flag=1;
            break;
        }
    }
    if(replace_flag==0)
    {
        printf("   compare result same, dont need replace \n");
        remove((const char *)temp_filename);
        return(0);
    }
    printf("   comprare result defference, need replace \n");

error:
    if(fp)
    {
        fclose(fp);
        fp = NULL;
    }
    remove((const char *)export_filename);
    rename((const char *)temp_filename,export_filename);
    return(1);
}




