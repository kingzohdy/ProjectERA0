#ifndef OG_SCRIPT1_H
#define OG_SCRIPT1_H

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif	/* WIN32 */

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif

/*
** game flow control values
*/
#define GAME_CONTINUE       0
#define GAME_OVER           2

#define MC_DEBUG
#define SCRIPT_VARIABLE_NUM     1048
#define SCRIPT_CALLSTACK_SIZE   8
#define MAX_PROC 			2048



/* system script command
*/
#define CMD_PROC            0x00
#define CMD_ENDPROC         0x01
#define CMD_IF              0x02
#define CMD_ELSE            0x03
#define CMD_ELSEIF          0x04
#define CMD_ENDIF           0x05
#define CMD_WHILE           0x06
#define CMD_BREAK           0x07
#define CMD_ENDWHILE        0x08
#define CMD_SWITCH          0x09
#define CMD_CASE            0x0a
#define CMD_ENDCASE         0x0b
#define CMD_ENDSWITCH       0x0c
#define CMD_GOTO            0x0d
#define CMD_CALL            0x0e
#define CMD_RETURN          0x0f
#define CMD_EXEC            0x10
#define CMD_VAR             0x11
#define CMD_ADD             0x12
#define CMD_SUB             0x13
#define CMD_MUL             0x14
#define CMD_DEV             0x15
#define CMD_RAND            0x16
#define CMD_EXIT            0x17
#define CMD_MOD             0x18

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*fnScriptDecoder)(unsigned long code);

enum opType
{
	opEQ = 0x3d,   // "=="
	opLT = 0x3c,   // "<"
	opGT = 0x3e,   // ">"
	opLE = 0xf3,   // "<="
	opGE = 0xf2,   // ">="
	opNE = 0xf7    // "!="
};

enum
{   
	LEAD_VARIABLE       =   '@',
	LEAD_DIRECTNUMBER   =   '#'
};

#pragma pack(1)

typedef struct  
{
    unsigned short  index;
    signed long     addr;
} ScriptProcAddr;


typedef struct
{
    unsigned long copyright; 
    unsigned long id;
    unsigned long version;
    unsigned long head_size;
    
    unsigned long data_size;
    unsigned long key_check;
    unsigned long max_proc;

    //Jim, changed [5/9/2004]
    //signed long proc_addr[1];
    ScriptProcAddr  proc_addr[1];

} ScriptFileHeader;

/*added by kent, 2004.7.27, for the version compatibility*/
typedef struct
{
    unsigned long copyright; 
    unsigned long id;
    unsigned long version;
    unsigned long head_size;
    
    unsigned long data_size;
    unsigned long key_check;
    unsigned long max_proc;
	
    signed long proc_addr[1];
	
} ScriptOldFileHeader;

typedef struct
{
    unsigned long code_offset;  /* code offset from start of location   */
    unsigned long code_size;    /* code size in bytes                   */
    unsigned long max_proc;     /* max proc in this script segment      */

    //Jim, changed [5/9/2004]
    //signed long proc_addr[1];   /* proc address(s) in this script seg.  */
    ScriptProcAddr proc_addr[1];    /* proc address(s) in this script seg.  */    

} ScriptHeader;


typedef struct  
{
    ScriptHeader *  hdr_ptr;    /* script header ptr    */
    unsigned char *code_segment_ptr;   /* current code segment start */
    unsigned char*  decode_ptr; /* script decode ptr    */
    	
    long variable[SCRIPT_VARIABLE_NUM]; /* script variables */
} ScriptCallStack;


typedef struct  
{
    int indent_size;    /* indent size in bytes */
    char info[1024];    /* debug text */
} ScriptDebugger;


typedef struct
{
    fnScriptDecoder decoder;            /* user decoder function ptr */
    ScriptDebugger debugger;            /* script debugger */
    
    ScriptHeader *curr_hdr;             /* current header */
    unsigned char *curr_code_segment;   /* current code segment start */
    unsigned char *decode_ptr;          /* current decode ptr */
    unsigned char *param_ptr;           /* current cmd params ptr */

    ScriptCallStack callstack[SCRIPT_CALLSTACK_SIZE];   /* call stack data  */
    int callstack_top;                  /* top of callstack */
 } ScriptKernel;

typedef struct 
{	
	int iRandCount;	
	long aiRandSet[50];	
	time_t tTune;	
	char cSuccFlag;		//标志位，通过GM设置效果成功率、命中率等概率为100
} TestEnv;

typedef struct tagScriptIndex
{
	char szName[32];
	char cScriptType;            
	int  iIndex;                   
}ScriptIndex;

#pragma pack()

int init_script(IN void *var_ptr, IN int size);
void set_script_decoder(IN fnScriptDecoder func);
void free_script(void);

int load_script(IN char* fname, IN void* code_ptr, IN unsigned int size);
int exec_script(IN char *fname, IN ScriptIndex *pstIndex, IN int proc_no, OUT char *pszDebug, IN void *pstTestEnv);
int check_script_proc(IN char *fname, IN ScriptIndex *pstIndex, IN int proc_no);

int get_script_number(INOUT int *var_idx);
char get_script_char(void);
char* get_script_string();
int get_script_variable(IN int idx, OUT int *val);
int set_script_variable(IN int idx, IN int val);

void debug_log(char *pszDebug, int before_update, int after_update, char *fmt, ...);

int FindDebugUin(long lUin);
int PlayerDebugScript(long lUin, int iType);

#ifdef __cplusplus
}
#endif

#endif /* OG_SCRIPT1_H */
