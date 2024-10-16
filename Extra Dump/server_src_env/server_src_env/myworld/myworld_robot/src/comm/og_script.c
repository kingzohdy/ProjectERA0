/*
**  script.cpp
**  script file functions.
**
**  Jim, 2004.2.9.
**      -- Created.
**
**  Jim, 2004.5.9.
**      -- Update header for reduce header size.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "og_script.h"

#ifdef WIN32 /* WIN32 */
#define DIR_SEPARATE_CHAR '\\'
#else /* !WIN32 */
#define getch   getchar
#define strcmpi strcasecmp
#define strupr(a) (a)
#define DIR_SEPARATE_CHAR '/'
#endif /* WIN32 */

extern void* get_script_location(IN char *fname);
extern void *get_script_location_byind(ScriptIndex *pstIndex);

static long g_debug_uin[8]={0};
static ScriptKernel *pkernel = NULL;    /* script interpreter kernel */

static int is_expr_true(long num1, unsigned char oper, long num2);
static long get_script_long(void);
static unsigned long get_script_cmd_code();
static int callstack_push();
static int callstack_pop();
static int fn_dummy_mac_decoder(unsigned long code);
static signed long int get_proc_address(ScriptHeader *pheader, int proc_no);
//static int compare_proc_no(const ScriptProcAddr *addr1, const ScriptProcAddr *addr2);

int init_script(IN void *var_ptr, IN int size)
{
    if(!var_ptr)
        return -1;
    if(size < (int)sizeof(ScriptKernel))
        return -2;
    memset(var_ptr, 0, sizeof(ScriptKernel));

    pkernel = (ScriptKernel*)var_ptr;
    pkernel->decoder = fn_dummy_mac_decoder;
       
    return sizeof(ScriptKernel);
}

void free_script(void)
{
    pkernel = NULL;
}

int get_total_oldproc(signed long *oldproc_addr)
{
    int     i;
    int     total;
    
    total=0;
    for(i=0;i<MAX_PROC;i++)
    {
        if(oldproc_addr[i]>=0)
            total++;
    }
    return(total);
}

int load_script(IN char *fname, IN void *code_ptr, IN unsigned int size)
{
    FILE *fp = NULL;
    char filename[256];

    ScriptFileHeader fhdr;
	char *ptr;

	//added by kent, 2004.7.27, for version compatibility
	ScriptOldFileHeader oldfhdr;
	signed long oldproc_addr[MAX_PROC];
	ScriptProcAddr procaddr;
	int i;
	int idx;

    ScriptHeader hd, *phd;
 
    if(! pkernel)
        return -1;
    if(! code_ptr)
        return -2;
    
    /* open file for read 
    */
    //sprintf((char *)filename, "%s%c%s.mac", SCRIPT_PATH, 
    //   DIR_SEPARATE_CHAR, fname);
    sprintf((char *)filename, "%s", fname);
    if(NULL == (fp = fopen((const char *)filename,"rb")))
    {
        //printf("ERROR : error open file %s(load_script)\n",filename);
        goto _error_exit;
    }

    /* read file header & proc_addr(s)
    */

   fread(&fhdr, 1, sizeof(fhdr) - sizeof(fhdr.proc_addr[0]), fp);

	if (fhdr.version >= 0x00060001)
	{
	    hd.max_proc = fhdr.max_proc;
		hd.code_offset = sizeof(hd) + (hd.max_proc - 1) * sizeof(hd.proc_addr[0]);
		hd.code_size = fhdr.data_size;
		memcpy(code_ptr, &hd, sizeof(hd));
		phd = (ScriptHeader *)code_ptr;

/*
		if (phd->proc_addr - phd + sizeof(phd->proc_addr[0]) * phd->max_proc > size )
			goto _error_exit;
*/
			
		fread(phd->proc_addr, 1, sizeof(phd->proc_addr[0]) * phd->max_proc, fp);


		/* read script encoded data
		*/
		ptr = (char*)phd + phd->code_offset;
		fseek(fp, fhdr.head_size, SEEK_SET);

		if (phd->code_offset + phd->code_size > size)
			goto _error_exit;
			
		fread(ptr, 1, phd->code_size, fp);
	}
	else
	{
		rewind(fp);
		fread(&oldfhdr, 1, sizeof(oldfhdr) - sizeof(oldfhdr.proc_addr[0]), fp);
		fread(oldproc_addr, 1, sizeof(oldproc_addr), fp);
		hd.max_proc = get_total_oldproc(oldproc_addr);
		hd.code_offset = sizeof(hd) + (hd.max_proc - 1) * sizeof(hd.proc_addr[0]);
		hd.code_size = oldfhdr.data_size;
		memcpy(code_ptr, &hd, sizeof(hd));
		phd = (ScriptHeader *)code_ptr;
		idx = 0;
		for(i=0; i<MAX_PROC; i++)
		{
			if(oldproc_addr[i] >= 0)
			{
				procaddr.index = i;
				procaddr.addr = oldproc_addr[i];
				memcpy(&(phd->proc_addr[idx]), &procaddr, sizeof(phd->proc_addr[idx]));
				idx++;
			}
		}

		ptr = (char*)phd + phd->code_offset;
		fseek(fp, oldfhdr.head_size, SEEK_SET);

		if (phd->code_offset + phd->code_size > size)
			goto _error_exit;
		
		fread(ptr, 1, phd->code_size, fp);
	}
   

    /* read ok, close file
    */
    if(fp) fclose(fp);

    /* return used memory size(in bytes)
    */
    return ptr + phd->code_size - (char*)code_ptr;
    
_error_exit:
    if(fp) fclose(fp);
    return -1;
}


int exec_script(IN char *fname, IN ScriptIndex *pstIndex, IN int proc_no, OUT char *pszDebug, IN void *pstTestEnv)
{
    int pass;
    int num,num1,num2;
    long jump_offset;
    unsigned char oper;
    int ret;
    char str[256];
    unsigned long command_code;
    signed long addr;

    unsigned char *plocation;
    ScriptHeader *phdr;
    unsigned char *pencoded_data;
    TestEnv *pstEnv;
    char szOper[8];
	
    if (!pstTestEnv)
	pstEnv	=	NULL;
    else
	pstEnv	=	(TestEnv*)pstTestEnv;

    if(!pkernel)
        return -1;

    if (fname[0] == 0)
	return -1;	

    plocation = NULL;
    if (pstIndex && 0 == strcasecmp(pstIndex->szName, fname))	
    {
	  plocation = 	(unsigned char*)get_script_location_byind(pstIndex);
    }

    if (NULL == plocation)
    {
	    if (pstIndex)
	    {
           	pstIndex->szName[0] = 0;
	    }
			
	    plocation = (unsigned char*)get_script_location(fname);
	    if(NULL == plocation)
	        return -1;
    }

    phdr = (ScriptHeader*)plocation;


    //Jim, changed [5/9/2004]
    //{{{
    addr = get_proc_address(phdr, proc_no);
    if(0 > addr)
        return -1;

    /* old version
    if(proc_no < 0 || proc_no >= (int)phdr->max_proc)
        return -1;
    if(phdr->proc_addr[proc_no] < 0)
        return -1;
    */
    //}}}

    if(!callstack_push())
        return GAME_OVER;

    pencoded_data = (unsigned char *)phdr + phdr->code_offset;

    pkernel->curr_code_segment = pencoded_data;
    pkernel->curr_hdr = phdr;

    //Jim, changed [5/9/2004]
    //{{{
    //pkernel->decode_ptr = pencoded_data + phdr->proc_addr[proc_no];
    pkernel->decode_ptr = pencoded_data + addr;
    //}}}

    pass = 0;
    num = num1 = num2 = 0;
    ret = GAME_CONTINUE;
    memset(str, 0x00, 255);
    while(pass==0)
    {
        command_code = get_script_cmd_code();
        switch(command_code)
        {
        case CMD_PROC:
            num = get_script_number(NULL);
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 4, "PROC    %d", num);
#endif//MC_DEBUG
            break;

        case CMD_ENDPROC:
            pass = 1;
 
#ifdef  MC_DEBUG
            debug_log(pszDebug, -4, 0, "ENDPROC");
#endif//MC_DEBUG
            break;

        case CMD_IF:
            jump_offset = get_script_long();
            num1 = get_script_number(&num);
            oper = get_script_char();
            num2 = get_script_number(NULL);
            if(!is_expr_true(num1, oper, num2))
            {
                pkernel->decode_ptr = pkernel->curr_code_segment + jump_offset;
            }
#ifdef  MC_DEBUG
			switch (oper)
			{
			case opEQ:
				strcpy(szOper, "==");
				break;
			case opLT:
				strcpy(szOper, "<");
				break;
			case opGT:
				strcpy(szOper, ">");
				break;
			case opLE:
				strcpy(szOper, "<=");
				break;
			case opGE:
				strcpy(szOper, ">=");
				break;
			case opNE:
				strcpy(szOper, "!=");
				break;
			default:
				strcpy(szOper, "?");
			}
            debug_log(pszDebug, 0, 0, "IF [%d] %d(%d) %s %d",jump_offset,num,num1,szOper,num2);
#endif//MC_DEBUG
            break;

        case CMD_GOTO: /* GOTO label*/
            jump_offset = get_script_long();
            pkernel->decode_ptr = pkernel->curr_code_segment + jump_offset;
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "GOTO [%d]", jump_offset);
#endif//MC_DEBUG
            break;

        case CMD_CALL:  /* CALL #proc_no */
	     num = get_script_number(NULL);	
		
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "CALL {%d}",num);
#endif//MC_DEBUG
	      
	      exec_script(fname, pstIndex, num, pszDebug, pstTestEnv);	
		
            break;

        case CMD_RETURN:
            pass = 1;
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "RETURN");
#endif//MC_DEBUG
            break;

        case CMD_VAR:
            num = get_script_number(NULL);
            num2 = get_script_number(NULL);
            if( num >= 0 && num < SCRIPT_VARIABLE_NUM )
            {
                 pkernel->callstack[pkernel->callstack_top-1].variable[num] = num2;		
            }
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "VAR %d %d",num,num2);
#endif//MC_DEBUG
            break;

        case CMD_ADD:
            num = get_script_number(NULL);
            num2 = get_script_number(NULL);
            if( num >= 0 && num < SCRIPT_VARIABLE_NUM )
            {
                 pkernel->callstack[pkernel->callstack_top-1].variable[num] += num2;	
            }
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "ADD %d(%d) %d",num,pkernel->callstack[pkernel->callstack_top-1].variable[num],num2);
#endif//MC_DEBUG
            break;

        case CMD_SUB:
            num = get_script_number(NULL);
            num2 = get_script_number(NULL);
            if( num >= 0 && num < SCRIPT_VARIABLE_NUM )
            {
                 pkernel->callstack[pkernel->callstack_top-1].variable[num] -= num2;		
	     }
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "SUB %d %d",num,num2);
#endif//MC_DEBUG
            break;

        case CMD_MUL:
            num = get_script_number(NULL);
            num2 = get_script_number(NULL);
            if( num>=0 && num < SCRIPT_VARIABLE_NUM )
            {
                 pkernel->callstack[pkernel->callstack_top-1].variable[num] *= num2;		
            }
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "MUL %d %d",num,num2);
#endif//MC_DEBUG
            break;

        case CMD_DEV:
            num = get_script_number(NULL);
            num2 = get_script_number(NULL);
            if( num >= 0 && num < SCRIPT_VARIABLE_NUM )
            {
                if(num2)
                {
			pkernel->callstack[pkernel->callstack_top-1].variable[num] /= num2;			
                }
            }
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "DEV %d %d",num,num2);
#endif//MC_DEBUG
            break;
        case CMD_MOD:
            {
                num = get_script_number(NULL);
                num2 = get_script_number(NULL);
                if(num >=0 && num < SCRIPT_VARIABLE_NUM && num2 != 0)
                {
                    pkernel->callstack[pkernel->callstack_top-1].variable[num] %= num2;			
                }
            }
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "MOD %d %d",num,num2);
#endif//MC_DEBUG
            break;

        case CMD_RAND:
            num = get_script_number(NULL);
            num2 = get_script_number(NULL);
            if( num >= 0 && num < SCRIPT_VARIABLE_NUM )
            {
                if(num2 > 0 )
                {
					if (pstEnv && pstEnv->iRandCount > 0 && pstEnv->iRandCount <= 50)
					{
						pkernel->callstack[pkernel->callstack_top-1].variable[num] = pstEnv->aiRandSet[rand() % pstEnv->iRandCount];
						
					}
					else
					{
						pkernel->callstack[pkernel->callstack_top-1].variable[num] = rand() % num2;
					}
                }
            }
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "RAND %d %d",num,num2);
#endif//MC_DEBUG
            break;

        case CMD_EXEC:
                strcpy( (char *)str,(const char *)get_script_string() );
                num = get_script_number(NULL);

		  if (str[0] == 0)
			return -1;
		
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "EXEC {%s %d}", str, num);
#endif//MC_DEBUG
	      
	      exec_script(str, NULL, num, pszDebug, pstTestEnv);

            break;

        case CMD_EXIT:
            ret = GAME_OVER;
            pass = 1;
#ifdef  MC_DEBUG
            debug_log(pszDebug, 0, 0, "EXIT");
#endif//MC_DEBUG
            break;

        case CMD_ELSE:
        case CMD_ELSEIF:
        case CMD_ENDIF:
        case CMD_WHILE:
        case CMD_ENDWHILE:
        case CMD_SWITCH:
        case CMD_CASE:
        case CMD_ENDCASE:
        case CMD_ENDSWITCH:
        case CMD_BREAK:
            break;

            // USER COMMAND NOW =============================================
        default:
            debug_log(pszDebug, 0, 0, "USER CMD %d", command_code);	
            ret = pkernel->decoder(command_code);
            if(ret != GAME_CONTINUE)
                pass=1;
            break;
        }
    }

    callstack_pop();
	
    return(ret);
}


int check_script_proc(IN char *fname, IN ScriptIndex *pstIndex, IN int proc_no)
{
    signed long addr;
    unsigned char *plocation;
    ScriptHeader *phdr;
	
    if(!pkernel)
        return -1;

    if (fname[0] == 0)
	return -1;	

    plocation = NULL;
    if (pstIndex && 0 == strcasecmp(pstIndex->szName, fname))	
    {
	  plocation = 	(unsigned char*)get_script_location_byind(pstIndex);
    }

    if (NULL == plocation)
    {
          pstIndex->szName[0] = 0;
			
	    plocation = (unsigned char*)get_script_location(fname);
	    if(NULL == plocation)
	        return -1;
    }

    phdr = (ScriptHeader*)plocation;

    addr = get_proc_address(phdr, proc_no);

    if(0 > addr)
        return -1;

    return 0;	
}

void set_script_decoder(IN fnScriptDecoder func)
{
    if(!pkernel) return;
    if(func) pkernel->decoder = func;
}

int get_script_number(INOUT int *var_idx)
{
   long ret, number;
    unsigned char *p;
    
    ret = 0;
    p = pkernel->param_ptr;
    switch(*p++)    /* leadcode */
    {
    case LEAD_VARIABLE:
        number = *(signed long *)p;
        p += sizeof(signed long);
        if( number >= 0 && number < SCRIPT_VARIABLE_NUM )
        {
            ret =  pkernel->callstack[pkernel->callstack_top-1].variable[number];
        }
        if(var_idx) *var_idx = number;
        break;

    case LEAD_DIRECTNUMBER:
        number = *(signed long *)p;
        p += sizeof(signed long);
        ret = number;
        if(var_idx) *var_idx = -1;
        break;
    }
    pkernel->param_ptr = p;
    return ret;
}


char*  get_script_string(void)
{
    static char temp[256];
    unsigned char *p, len;

    p = pkernel->param_ptr;
    len = *p++;
    memset(temp, 0x00, 256);
    strncpy(temp, (char*)p, len);
    p += len;
    pkernel->param_ptr = p;

    return (char*)temp;
}


char get_script_char(void)
{
    char ch;
    ch = *pkernel->param_ptr++;
    return  ch;
}


int get_script_variable(IN int idx, OUT int *val)
{
    if(idx >= 0 && idx < SCRIPT_VARIABLE_NUM)
    {
        if(val) *val = pkernel->callstack[pkernel->callstack_top-1].variable[idx];
        return 0;
    }
    return -1;
}


int set_script_variable(IN int idx, IN int val)
{
    if(idx >= 0 && idx < SCRIPT_VARIABLE_NUM)
    {
        pkernel->callstack[pkernel->callstack_top-1].variable[idx] = val;
        return 0;
    }
    return -1;
}


static int is_expr_true(long num1, unsigned char oper, long num2)
{
    switch(oper)
    {
    case opEQ: return (num1 == num2);
    case opLT: return (num1 <  num2);
    case opGT: return (num1 >  num2);
    case opLE: return (num1 <= num2);
    case opGE: return (num1 >= num2);
    case opNE: return (num1 != num2);
    default:   return 0;
    }
}


static long get_script_long(void)
{
    long long_value;
    unsigned char *p;

    p = pkernel->param_ptr;
    long_value = p[0] + p[1] * 0x100 + p[2] * 0x10000 + p[3] * 0x1000000;
    p += 4;
    pkernel->param_ptr = p;

    return long_value;
}


static unsigned long get_script_cmd_code()
{
    unsigned long cmd_code;
    unsigned char *p;
    int skip;

    p = pkernel->decode_ptr;
    cmd_code = p[0] + p[1] * 0x100;
    p += 2;

    skip = *p++;
    pkernel->decode_ptr = p + skip;

    pkernel->param_ptr = p;
    
    return cmd_code;
}


static int fn_dummy_mac_decoder(unsigned long code)
{
    unsigned short my_code;
    my_code = (unsigned short)code;
    debug_log(NULL, 0, 0, "USERCMD(0x%0x)",code);
    return GAME_CONTINUE;
}


static int callstack_push()
{
    int idx = pkernel->callstack_top;
    int is_success = 0;

    if(idx < SCRIPT_CALLSTACK_SIZE)
    {
        pkernel->callstack[idx].decode_ptr = pkernel->decode_ptr;
        pkernel->callstack[idx].hdr_ptr = pkernel->curr_hdr;
	 pkernel->callstack[idx].code_segment_ptr = pkernel->curr_code_segment;
	 pkernel->callstack_top ++;

        is_success = 1;
    }

    return is_success;
}


static int callstack_pop()
{
    int idx = pkernel->callstack_top;
    int is_success = 0;

    if(idx > 0)
    {
        idx = --pkernel->callstack_top;
	  pkernel->curr_code_segment = pkernel->callstack[idx].code_segment_ptr;
	  pkernel->curr_hdr = pkernel->callstack[idx].hdr_ptr;
	  pkernel->decode_ptr = pkernel->callstack[idx].decode_ptr;
        
        is_success = 1;
    }

    return is_success;
}


void debug_log(char *pszDebug, int before_update, int after_update, char *fmt, ...)
{
    char str[1024];
    char tmp[1024];
    va_list args;

    pkernel->debugger.indent_size += before_update;
    if(pkernel->debugger.indent_size > 512)
        pkernel->debugger.indent_size = 512;
    if(pkernel->debugger.indent_size < 0)
        pkernel->debugger.indent_size = 0;

    memset(str, 0, 1024);
    memset(str, ' ', pkernel->debugger.indent_size);


    va_start(args, fmt);
    vsprintf( &str[pkernel->debugger.indent_size], 
        //1024 - pkernel->debugger.indent_size, 
        fmt, args );
    va_end(args);

//    printf("%s\n", str);
    if (pszDebug != NULL && strlen(pszDebug) < 4096-1024-1)
    {
    	vsprintf(str, fmt, args);
       	snprintf(tmp, 1024, "%s\n", str);
       	strcat(pszDebug, tmp);
    }

    pkernel->debugger.indent_size += after_update;
    if(pkernel->debugger.indent_size > 512)
        pkernel->debugger.indent_size = 512;
    if(pkernel->debugger.indent_size < 0)
        pkernel->debugger.indent_size = 0;
}

/*
static signed long get_proc_address(ScriptHeader *pheader, int proc_no)
{
    ScriptProcAddr key, *node_ptr;

    if(!pheader || proc_no<0)
        return 0;

    key.index = proc_no;
    node_ptr = (ScriptProcAddr*)bsearch(&key, pheader->proc_addr, pheader->max_proc, sizeof(key),
        (int (*)(const void*, const void*))compare_proc_no);
    if(NULL == node_ptr)
        return -1;

    return node_ptr->addr;
}*/

static ScriptProcAddr *bsearch_proc (const ScriptProcAddr *key, const void *base, int nmemb)
{
	int l, u, idx;
	ScriptProcAddr *p;
	int comparison;
	
	if (0 >= nmemb) return NULL;
	l = 0;
	u = nmemb-1;
	
	while (l <= u)
	{
		idx = (l + u) / 2;
		p = (ScriptProcAddr *) (((const char *) base) + (idx * sizeof(ScriptProcAddr)));
		comparison = (int)key->index - (int)p->index;
		
		if (comparison == 0)
		{
			return p;
		}		
		else if (comparison < 0)
		{
			u = idx - 1;
		}
		else
		{
			l = idx + 1;
		}		
	}
	
	return NULL;
}


static signed long get_proc_address(ScriptHeader *pheader, int proc_no)
{
    ScriptProcAddr key, *node_ptr;

    if(!pheader || proc_no<0)
        return 0;

    key.index = proc_no;
    node_ptr = bsearch_proc(&key, pheader->proc_addr, pheader->max_proc);
    if(NULL == node_ptr)
        return -1;

    return node_ptr->addr;
}

/*
static int compare_proc_no(const ScriptProcAddr *addr1, const ScriptProcAddr *addr2)
{
    return (int)addr1->index - (int)addr2->index;
}*/

int FindDebugUin(long lUin)
{
	int i;

	for (i = 0; i < 8; i++)
	{
		if (lUin == g_debug_uin[i])
			return i;
	}

	return -1;
}

int FindDebugFree()
{
	int i;

	for (i = 0; i < 8; i++)
	{
		if (g_debug_uin[i] <= 0)
			return i;
	}

	memmove(&g_debug_uin[0], &g_debug_uin[1], 7*sizeof(g_debug_uin[0]));
	return 7;
}

int PlayerDebugScript(long lUin, int iType)
{
	int iPos;

	if (iType == 0)
	{
		iPos = FindDebugUin(lUin);
		
		if (iPos < 0)
			return 0;

		g_debug_uin[iPos] = 0;
	}
	else if (iType == 1)
	{
		if (FindDebugUin(lUin) > 0)
			return 0;
		
		iPos = FindDebugFree();

		if (iPos < 0)
			return -1;

		g_debug_uin[iPos] = lUin;
	}
	else
		return -1;
	
	return 0;
}

