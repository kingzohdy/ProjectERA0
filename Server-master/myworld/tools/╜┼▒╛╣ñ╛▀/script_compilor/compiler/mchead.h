/*
**      MCHEAD.H
**      Macro header.
*/
/*----------------------*/
/*      PROTOTYPE       */
/*--------------------------------------------------------------*/
#ifndef __mchead_h
#define __mchead_h

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdarg.h>

#ifdef  WIN32
#include <conio.h>
#endif//WIN32

//#include <sys\types.h>
//#include <sys\stat.h>
//#include <io.h>
//#include <dos.h>
//#include <process.h>
//#include <direct.h>


//Jim, added, 2004.2.9.
#ifdef  WIN32
#define DIR_SEPARATE_CHAR '\\'  


#else//!WIN32
#define DIR_SEPARATE_CHAR '/'

#ifndef _MAX_PATH
#define _MAX_PATH   255
#endif//_MAX_PATH

#ifndef _MAX_FNAME
#define _MAX_FNAME  255
#endif//_MAX_FNAME

#define getch   getchar
#define strcmpi strcasecmp
#define strupr(a) (a)

#endif//WIN32


#pragma warning(disable:4514)   //nreferenced inline function has been removed
#pragma warning(disable:4100)   //unreferenced formal parameter

/*----------------------*/
/*      TYPEDEF         */
/*--------------------------------------------------------------*/
#define TTN_OK                          0
#define TTN_NOT_OK                      1
#define TTN_ERROR                       -1


#define MAX_KEYWORD_LENGTH              32
#define MAX_PARAMETER                   32
#define MAX_COMMAND                     1024


#define DUMMY_TYPE                      '0'
#define INT_TYPE                        '1'
#define WORD_TYPE                       '2'
#define STR_TYPE                        '3'


#define LINE_BUFFER_SIZE                1024
#define USER_CMD_STR_LEN              1024

#define GROUP_ID_INFORMATION            "[INFORMATION]"
#define GROUP_ID_CONFIG                 "[CONFIG]"
#define GROUP_ID_SYSTEM_COMMAND         "[SYSTEM COMMAND]"
#define GROUP_ID_USER_COMMAND           "[USER COMMAND]"
#define GROUP_ID_EXPORT_START           "[EXPORT START]"
#define GROUP_ID_EXPORT_END             "[EXPORT END]"
#define GROUP_ID_REMARK                 "[REMARK]"

#define ITEM_ID_COPYRIGHT               "COPYRIGHT"
#define ITEM_ID_VERSION                 "VERSION"
#define ITEM_ID_FILEEXTENSION           "FILEEXTENSION"
#define ITEM_ID_KEYCHECK                "KEYCHECK"

#define ITEM_ID_DATAFILE                "DATAFILE"
#define ITEM_ID_LISTCODE                "LISTCODE"
#define ITEM_ID_EXPORTHEADER            "EXPORTHEADER"
#define ITEM_ID_TESTTARGET              "TESTTARGET"
#define ITEM_ID_TARGETDIR               "TARGETDIR"
#define ITEM_ID_HEADERDIR               "HEADERDIR"
#define ITEM_ID_LOGERROR                "LOGERROR"
#define ITEM_ID_LOGLANGSCRIPT           "LOGLANGSCRIPT"
#define ITEM_ID_UEDIT32DIR              "UEDIT32DIR"
#define ITEM_ID_NOTEPADDIR				"NOTEPADDIR"

#define OUTPUT_NONE                     '0'
#define OUTPUT_MESSAGE                  '1'
#define OUTPUT_WARNING                  '2'
#define OUTPUT_ERROR                    '3'

#define COMMAND_ATTRIB_UNKNOW           '0'
#define COMMAND_ATTRIB_SYSTEM           '1'
#define COMMAND_ATTRIB_USER             '2'

#define MAX_DEFINE                      256
#define MAX_LABEL                       256
#define MAX_IF                          256
#define MAX_ELSEIF                      256
#define MAX_SWITCH                      256
#define MAX_CASE                        256
#define MAX_WHILE                       256
#define MAX_BREAK                       256
#define MAX_GOTO                        256
#define MAX_CALL                        256

#define OP_WORD_EQ                      "="
#define OP_WORD_LT                      "<"
#define OP_WORD_GT                      ">"
#define OP_WORD_LE                      "<="
#define OP_WORD_GE                      ">="
#define OP_WORD_NE                      "!="
#define OP_WORD_ADD                     "+"
#define OP_WORD_SUB                     "-"
#define OP_WORD_MUL                     "*"
#define OP_WORD_DEV                     "/"
#define OP_WORD_MOD                     "%"
#define OP_WORD_POWER                   "^"

#define OP_ID_UNKNOWN                   0xff
#define OP_ID_EQ                        0x3d            //'='
#define OP_ID_LT                        0x3c            //'<'
#define OP_ID_GT                        0x3e            //'>'
#define OP_ID_LE                        0xf3            //'?
#define OP_ID_GE                        0xf2            //'?
#define OP_ID_NE                        0xf7            //'?
#define OP_ID_ADD                       0x2b            //'+'
#define OP_ID_SUB                       0x2d            //'-'
#define OP_ID_MUL                       0x2a            //'*'
#define OP_ID_DEV                       0x2f            //'/'
#define OP_ID_MOD                       0x25            //'%'
#define OP_ID_POWER                     0x5e            //'^'

//-------------------------------------------------------------------------
//      Copies of include\windll.h
//-------------------------------------------------------------------------
#ifndef DEFINE_MAC_DEFINES
#define DEFINE_MAC_DEFINES              1

#define MAX_VAR                         16000
#define MAX_PROC                        2048

//
// system macro command ================================
//
#define CMD_PROC                        0x00
#define CMD_ENDPROC                     0x01
#define CMD_IF                          0x02
#define CMD_ELSE                        0x03
#define CMD_ELSEIF                      0x04
#define CMD_ENDIF                       0x05
#define CMD_WHILE                       0x06
#define CMD_BREAK                       0x07
#define CMD_ENDWHILE                    0x08
#define CMD_SWITCH                      0x09
#define CMD_CASE                        0x0a
#define CMD_ENDCASE                     0x0b
#define CMD_ENDSWITCH                   0x0c
#define CMD_GOTO                        0x0d
#define CMD_CALL                        0x0e
#define CMD_RETURN                      0x0f
#define CMD_EXEC                        0x10
#define CMD_VAR                         0x11
#define CMD_ADD                         0x12
#define CMD_SUB                         0x13
#define CMD_MUL                         0x14
#define CMD_DEV                         0x15
#define CMD_RAND                        0x16
#define CMD_EXIT                        0x17
#define CMD_MOD				0x18

#define MAX_SYS_CMD      CMD_MOD 

typedef struct  MAC_INFO_STRUCT
{
   char                 filename[80];   // mac filename
   unsigned long int    code_size;      // mac code size
   unsigned long int    code_index;     // mac code index
   unsigned long int    max_var;        // max mac variables(=MAX_VAR)
   signed   long int    variable[1];    // mac variable list
} MAC_INFO, *LPMAC_INFO;

typedef struct  MAC_HEAD_STRUCT
{
   unsigned char        copyright[4];   // '3SS'+0x1a
   unsigned char        id[4];          // "MAC "
   unsigned long int    version;        // file version
   unsigned long int    head_size;      // file header size

   unsigned long int    data_size;      // total data size
   unsigned long int    key_check;      // check key
   unsigned long int    max_proc;       // max proc
   signed long int      proc_address[1];// proc address list
} MAC_HEAD, *LPMAC_HEAD;

#endif  // DEFINE_MAC_DEFINES

//---------------------------------------------------------------------------

typedef struct  COMPILER_CONFIG_STRUCT
{
   unsigned char        list_code;
   unsigned char        data_file;
   unsigned char        export_header;
   unsigned char        test_target;
   unsigned char        log_error;
   unsigned char        log_langscript;
   //
   unsigned char        target_dir[_MAX_PATH];  // V5.21
   unsigned char        header_dir[_MAX_PATH];  // V5.21
   unsigned char        uedit32_dir[_MAX_PATH]; // V5.22, support for UEDIT32.
   unsigned char		notepad_dir[_MAX_PATH];	//add by jessie, support for notepad++
} COMPILER_CONFIG;


typedef struct  MC_INFORMATION_STRUCT
{
   unsigned char        copyright[80];
   unsigned long int    version;
   unsigned char        file_ext[8];
   unsigned long        key_check;
   unsigned char        remark;
   unsigned char        label;
   unsigned char        variable;
   unsigned char        number;
   unsigned char        define[32];
   unsigned char        strvar[32];
} MC_INFORMATION;

typedef struct  COMMAND_TABLE_STRUCT
{
   unsigned char        flag;
   unsigned char        attrib;
   unsigned long        command_code;
   unsigned char        keyword[MAX_KEYWORD_LENGTH];
   unsigned char        parameter_type[MAX_PARAMETER];
} COMMAND_TABLE;

typedef struct  COMMAND_HEAD_STRUCT
{
   unsigned char        copyright[75];
   unsigned char        version;
   unsigned long int    key_check;
} COMMAND_HEAD;


typedef struct  LINE_DNODE_STRUCT
{
   struct LINE_DNODE_STRUCT    *last,*next;
   unsigned char                buffer[LINE_BUFFER_SIZE];
} LINE_DNODE;


typedef struct  OUTPUT_DNODE_STRUCT
{
   struct OUTPUT_DNODE_STRUCT   *last,*next;
   char                         flag;
   int                          line_number;
   char                         message[LINE_BUFFER_SIZE];
} OUTPUT_DNODE;



extern  COMPILER_CONFIG         compiler_config;
extern  MC_INFORMATION          mc_information;
extern  COMMAND_TABLE           command_table[MAX_COMMAND];
extern  LINE_DNODE            * export_start;
extern  LINE_DNODE            * export_end;
extern  char                    print_rec[LINE_BUFFER_SIZE];
extern  int                     system_line_number;

extern  void   read_ini_info(char *filename);
extern  void   skip_compartment(unsigned char *data,int *index);
extern  void   get_word(char *in,char *out,int *idx,int max);
extern  void   get_string(char *in,char *out,int *idx,int max);
extern  signed long int word_to_int(char *data);

extern  int     string_2_number(char *str);
extern  int     list_mac(char *filename);
extern  int     export_header(void);
extern  void    output_command_data_file(void);
extern  void    change_ext(char *target,char *filename,char *ext);

extern  int export_ultraedit_langscript(void);
extern  int update_ultraedit_langscript(void);
extern	int Config_NotPad_Xml();


extern  void    output_message(char flag,int line_number,char *message);

#endif//__mchead_h

