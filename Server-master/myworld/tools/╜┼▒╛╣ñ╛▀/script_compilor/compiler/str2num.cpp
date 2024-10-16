/*
**		STR2NUM.C
**		calculate the result of a dec string.
**
**
**		ZJian, 1998.11.12.	
**			Created.
**		ZJian, 1999.6.1.		
**			Add hex(0x??) surpport and char('?') surpport.
**		ZJian, 2001.1.16.
**			Add to library.
*/
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "mchead.h"

// #define TEST_THIS

#define MAXNUM          512
#define ISOP            1
#define ISNUM           2

#define DEC_NUM         1
#define HEX_NUM         2
#define CHAR_NUM        3
#define NOT_NUM         4

typedef struct tagELETYPE
{
	int value;           // value
	int type;            // 1 for op, 2 for number
} ELETYPE, *LPELETYPE ;


typedef struct tagQSTYPE    //queue stack type
{
	ELETYPE  stack[MAXNUM];
	int      top;
} QSTYPE, *LPQSTYPE ;


static	void        init_qs(QSTYPE *s);
static	int         push_qs(QSTYPE *s,ELETYPE x);
static	ELETYPE  *  pop_qs(QSTYPE *s);
static	ELETYPE  *  get_top_qs(QSTYPE s);
static	int         empty_qs(QSTYPE s);
static	char        proceed(char op1,char op2);
static	int         post_fix(char *source, QSTYPE *target, QSTYPE *s);
static	int         cal_result(QSTYPE *s);

extern	int     string_2_number(char *str);

/******************************************************************************/
/*        Function:       Initialize queue stack                              */
/******************************************************************************/
void    init_qs(QSTYPE *s)
{
	s->top=-1;
}

/******************************************************************************/
/*        Function:       Push element to queue stack                         */
/******************************************************************************/
int     push_qs(QSTYPE *s,ELETYPE x)
{
	if(s->top >= MAXNUM-1 )
		return(0);
	else
	{
		s->top++;
		memcpy(&s->stack[s->top],&x,sizeof(ELETYPE));
		return(1);
	}
}

/******************************************************************************/
/*        Function:       Pop element from queue stack                        */
/******************************************************************************/
ELETYPE * pop_qs(QSTYPE *s)
{
	static ELETYPE x;
	
	if(s->top < 0)
		return(NULL);
	else
	{
		s->top--;
		memcpy(&x,&s->stack[s->top+1],sizeof(ELETYPE));
		return(&x);
	}
}

/******************************************************************************/
/*        Function:       Get top element of queue stack                      */
/******************************************************************************/
ELETYPE * get_top_qs(QSTYPE s)
{
	static  ELETYPE x;
	
	if(s.top<0)
		return(NULL);
	else
	{
		memcpy(&x,&s.stack[s.top],sizeof(ELETYPE));
		return(&x);
	}
}

/******************************************************************************/
/*        Function:       If the queue stack is empty ?                       */
/******************************************************************************/
int     empty_qs(QSTYPE s)
{
	if(s.top<0)
		return(1);
	else
		return(0);
}
/******************************************************************************/
/*        Function:       proceed the pre turn of the operate char            */
/*        Input:          op1,op2         --> the operate chars to proceed    */
/*        Output:         char                                                */
/*                                '>'     --> op1 turn > op2 turn             */
/*                                '='     --> op1 turn = op2 turn             */
/*                                '<'     --> op1 turn < op2 turn             */
/******************************************************************************/
char    proceed(char op1,char op2)
{
	char    cc1[7]={ '+', '-', '*', '/', '(', ')', '#' };
	char    cc2[7]={ '+', '-', '*', '/', '(', ')', '#' };
	int     index1,index2;
	int     i;
	char turn[7][7]={
		/* vertic is op1, horiz is op2, the turn-char is the compare result */
		/* '>'     the op1's turn is heigher than op2                       */
		/* '<'     the op1's turn is lower than op2                         */
		/* '='     the op1's turn is equal to op2                           */
		/* '$'     the turn is error                                        */
		/*2\1*//*[+]  [-]  [*]  [/]  [(]  [)]  [#]*/
		/*[+]*/{ '>', '>', '<', '<', '<', '>', '>' },
		/*[-]*/{ '>', '>', '<', '<', '<', '>', '>' },
		/*[*]*/{ '>', '>', '>', '>', '<', '>', '>' },
		/*[/]*/{ '>', '>', '>', '>', '<', '>', '>' },
		/*[(]*/{ '<', '<', '<', '<', '<', '=', '$' },
		/*[)]*/{ '>', '>', '>', '>', '$', '>', '>' },
		/*[#]*/{ '<', '<', '<', '<', '<', '$', '=' }
	};
	
	index1=-1;
	index2=-1;
	for(i=0;i<7;i++)
	{
		if(op1==cc1[i])
			index1=i;
		if(op2==cc2[i])
			index2=i;
	}
	if((index1==-1)||(index2==-1))
		return('$');
	return(turn[index1][index2]);
}
/******************************************************************************/
/*        Function:       Postfix the middle-end sentence                     */
/******************************************************************************/
int post_fix( char *source, QSTYPE *target, QSTYPE *s )
{
	int     value;
	int     vflag;
	int     nflag;
	char    c,c1,c2;
	char    op;
	ELETYPE x,x1,x2;
	QSTYPE  *q;
	int     pass,index,sub_pass;
	char    buffer[1024];
	int     len;
	int     cflag;
	int     nbase;
    int     ret;
	
    ret = 0;

	//(1)process the source string to eletype and push it to queue stack target
    //
	init_qs(target);
	op=' ';
	value=0;
	vflag=0;
	nflag=0;
	nbase=10;
	cflag=0;

	memset(buffer,0x00,1024);
	strcpy((char *)buffer,(const char *)source);
	strcat((char *)buffer,";;;;;;");
	len=(int)strlen((const char *)buffer);
	pass=0;
	index=0;
	while(pass==0)
	{
		c=buffer[index++];
		if(c==';')
			break;
		else if(c=='\'')                     // char
		{
			c1=buffer[index];
			c2=buffer[index+1];
			if(c2=='\'')
			{
				x.value=c1;
				x.type=ISNUM;
				push_qs(target,x);
				index+=2;
			}
			else
			{
				sprintf(print_rec,"Invalid char in number(%s).", source);
				output_message(OUTPUT_ERROR,system_line_number,print_rec);
				pass=1;
                ret=-1;
			}
		}
		else if(c >='0' && c<='9')           // number
		{
			nbase=10;
			if((c=='0')&& ((buffer[index]=='x')||(buffer[index]=='X')) )
			{/* is hex number */
				nbase=16;
				c=buffer[++index];
			}
			else
			{/* is dec number */
				index--;
			}

            /* start process number's value */
			value = 0;
			sub_pass=0;
			while(sub_pass==0)
			{
				switch(c)
				{
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					value=value*nbase + c - '0';
					index++;
					break;
				case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
					if(nbase==16)
					{
						value=value*nbase + c -'A' + 10;
						index++;
					}
					else
                    {
                        sprintf(print_rec,"Invalid char in number(%s).", source);
                        output_message(OUTPUT_ERROR,system_line_number,print_rec);
                        sub_pass=1;
                        pass = 1;
                        ret = -1;
                    }
					break;
				case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
					if(nbase==16)
					{
						value=value*nbase + c -'a' + 10;
						index++;
					}
					else
                    {
                        sprintf(print_rec,"Invalid char in number(%s).", source);
                        output_message(OUTPUT_ERROR,system_line_number,print_rec);
						sub_pass=1;
                        pass = 1;
                        ret = -1;
                    }
					break;
                case '+': case '-': case '*': case '/': case '(': case ')':
                    sub_pass = 1;
                    break;
                case ';':
                    sub_pass = 1;
                    break;
				default:
                    {
                        sprintf(print_rec,"Invalid char in number(%s).", source);
                        output_message(OUTPUT_ERROR,system_line_number,print_rec);
						sub_pass=1;
                        pass = 1;
                        ret = -1;
                    }
					break;
				}
				c=buffer[index];
			}
            if(ret == -1)
            {
                return  -1;
            }
			x.value=value;
			x.type=ISNUM;
			push_qs(target,x);
		}
		else if( (c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='(')||(c==')')) // op
		{
			x.value=(int)c;
			x.type=ISOP;
			push_qs(target,x);
		}
		else         // others
		{
            sprintf(print_rec,"Invalid char in number(%s).", source);
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
			pass=1;
            ret = -1;
		}
		if(index>len)
			break;
	}

	// (2) pop the element of queue stack target to queue stack s
    //
	init_qs(s);
	x.value='#';
	x.type=ISOP;
	push_qs(s,x);
	while(!empty_qs(*target))
    {
        push_qs(s,*pop_qs(target));
    }

	//(3) postfix the queue stack s to queue stack target
    //
	q=(QSTYPE *)malloc(sizeof(QSTYPE));
	if(q==NULL)
	{
        sprintf(print_rec,"Memory alloc error when process number string(%s).", source);
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
		return -2;
	}
	init_qs(target);
	init_qs(q);
	
	x.value='#';
	x.type=ISOP;
	push_qs(q,x);
	
	memcpy(&x1,get_top_qs(*q),sizeof(ELETYPE));
	memcpy(&x2,pop_qs(s),sizeof(ELETYPE));
	pass=0;
	while(pass==0)
	{
		if(x2.type==ISNUM)
		{
			push_qs(target,x2);
			memcpy(&x2,pop_qs(s),sizeof(ELETYPE));
		}
		else if(x2.type==ISOP)
		{
			c1=(char)x1.value;
			c2=(char)x2.value;
			
			if(proceed(c1,c2)=='<')
			{
				/* push x2 to queue stack q                     */
				push_qs(q,x2);

				/* change x1 to now top of queue stack q        */
				memcpy(&x1,get_top_qs(*q),sizeof(ELETYPE));

				/* get next element of queue stack s to x2      */
				memcpy(&x2,pop_qs(s),sizeof(ELETYPE));

			} /* op of the top of queue stack q  < read op      */

			else if(proceed(c1,c2)=='>')
			{
				/* pop out the element of q to target           */
				push_qs(target,*pop_qs(q));

				/* change x1 to now top of queue stack q        */
				memcpy(&x1,get_top_qs(*q),sizeof(ELETYPE));

			} /* op of the top of queue stack q > read op       */

			else if( (proceed(c1,c2)=='=')&&(c1=='(')&&(c2==')'))
			{
				/* kill the element of q (OP '(')               */
				pop_qs(q);

				/* change x1 to now top of queue stack q        */
				memcpy(&x1,get_top_qs(*q),sizeof(ELETYPE));

				/* get next element of queue stack s to x2      */
				memcpy(&x2,pop_qs(s),sizeof(ELETYPE));
			} /* kill the couple () of q                        */

			else if( (proceed(c1,c2)=='=')&&(c1=='#')&&(c2=='#'))
			{
				pass=1;
			} /* process finished                               */
		}
		else
		{
            sprintf(print_rec,"Invalid element of queue stack when process (%s).", source);
            output_message(OUTPUT_ERROR,system_line_number,print_rec);
			if(q!=NULL)
			{
				free(q);
				q=NULL;
			}
			return -3;
		}
	}

	//(4)convert the turn of queue stack target
	init_qs(s);
	while(!empty_qs(*target))
    {
        push_qs(s,*pop_qs(target));
    }

	init_qs(q);
	while(!empty_qs(*s))
    {
        push_qs(q,*pop_qs(s));
    }

	init_qs(target);
	while(!empty_qs(*q))
    {
        push_qs(target,*pop_qs(q));
    }

	//(5)free memory
	if(q != NULL)
		free(q);
	return  0;
}
/******************************************************************************/
/******************************************************************************/
int         cal_result(QSTYPE *s)
{
	ELETYPE x,x1,x2,r;
	QSTYPE  *t;
	
	t=(QSTYPE *)malloc(sizeof(QSTYPE));
	if(t==NULL)
		return(0);
	init_qs(t);
	
	while(!empty_qs(*s))
	{
		memcpy(&x,pop_qs(s),sizeof(ELETYPE));
		if(x.type==ISNUM)
		{
			push_qs(t,x);
		}
		else
		{
			switch(x.value)
			{
			case '+':
				x1.value=0;
				x1.type=ISNUM;
				x2.value=0;
				x2.type=ISNUM;
				if(!empty_qs(*t))
					memcpy(&x2,pop_qs(t),sizeof(ELETYPE));
				if(!empty_qs(*t))
					memcpy(&x1,pop_qs(t),sizeof(ELETYPE));
				r.value=x1.value + x2.value;
				r.type=ISNUM;
				push_qs(t,r);
				break;
			case '-':
				x1.value=0;
				x1.type=ISNUM;
				x2.value=0;
				x2.type=ISNUM;
				if(!empty_qs(*t))
					memcpy(&x2,pop_qs(t),sizeof(ELETYPE));
				if(!empty_qs(*t))
					memcpy(&x1,pop_qs(t),sizeof(ELETYPE));
				r.value=x1.value - x2.value;
				r.type=ISNUM;
				push_qs(t,r);
				break;
			case '*':
				x1.value=1;
				x1.type=ISNUM;
				x2.value=0;
				x2.type=ISNUM;
				if(!empty_qs(*t))
					memcpy(&x2,pop_qs(t),sizeof(ELETYPE));
				if(!empty_qs(*t))
					memcpy(&x1,pop_qs(t),sizeof(ELETYPE));
				r.value=x1.value * x2.value;
				r.type=ISNUM;
				push_qs(t,r);
				break;
			case '/':
				x1.value=1;
				x1.type=ISNUM;
				x2.value=0;
				x2.type=ISNUM;
				if(!empty_qs(*t))
					memcpy(&x2,pop_qs(t),sizeof(ELETYPE));
				if(!empty_qs(*t))
					memcpy(&x1,pop_qs(t),sizeof(ELETYPE));
				if(x2.value!=0)
					r.value=x1.value / x2.value;
				else
				{
					printf("Error: zero devide error\n");
					if(t!=NULL)
					{
						free(t);
						t=NULL;
					}
					return(0);
				}
				r.type=ISNUM;
				push_qs(t,r);
				break;
			}
		}
	}
	r.value=0;
	r.type=ISNUM;
	if(!empty_qs(*t))
		memcpy(&r,pop_qs(t),sizeof(ELETYPE));
	if(t!=NULL)
		free(t);
	return(r.value);
}
/******************************************************************************/
int     string_2_number(char *str)
{
	QSTYPE  ss;
	QSTYPE  tt;
	int     rr, result;
	
    rr = 0;
	result = post_fix(str,&tt,&ss);
    if(0 == result)
    {
        rr=cal_result(&tt);
    }
	return(rr);
}
/******************************************************************************/
/******************************************************************************/
#ifdef  TEST_THIS
static	void    disp_qs(QSTYPE *s)
{
	ELETYPE x;
	QSTYPE *d;
	
	
	d=(QSTYPE *)malloc(sizeof(QSTYPE));
	if(d==NULL)
		return;
	printf("disp_qs ------------------------------------------\n");
	memcpy(d,s,sizeof(QSTYPE));
	while(!empty_qs(*d))
	{
		memcpy(&x,pop_qs(d),sizeof(ELETYPE));
		switch(x.type)
		{
		case ISOP:
			printf("OP :%c\n",x.value);
			break;
		case ISNUM:
			printf("NUM:%d\n",x.value);
			break;
		}
	}
	if(d!=NULL)
		free(d);
}

void    main(int argc,char *argv[])
{
	char    source[256];
	
	if(argc!=2)
		return;
	strcpy(source,argv[1]);
	printf("%s = %d\n",source,string2number(source));
}
#endif  // TEST_THIS


