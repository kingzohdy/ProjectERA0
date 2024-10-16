/*
	parselogicexp.cpp:
	added by kent 2004.7.19, for parsing logic expression.
	use two stack to parse one logic expression, one store logic operator, one store expssion.
	the way to do it: break a long logic expression to many "IF false GOTO FALSE_EXIT , ture GOTO TRUE_EXIT"
	the way referenced 清华大学出版社《编译原理》167 page:
	E->E1 or E2   {backpath (E1.false, E2.codebegin);
				  E.codebegin := E1.codebegin;
				  E.true := merge(E1.true, E2.true)
				  E.false := E2.false}

    E->E1 and E2  {backpath (E1.true, E2.codebegin);
                  E.codebegin := E1.codebegin;
		          E.true := E1.true;
				  E.false := merge(E1.false, E2.false);}
    
	E->not E1     {E.true := E1.false;
	              E.codebegin := E1.codebegin;
				  E.false := E1.true;}

    E.true is ture link, E.false is false link.
	backpath(p,t) fill the p link GOTO addr with t;
	merge(p1,p2) merge p1 link and p2 link , then return link head addr. It is Implemented by function
	link_exp.

*/

#include "mchead.h"
#include "parselogicexp.h"
#include <string.h>

/*the stack store ptr to avail more flexible*/
typedef struct MY_STACK_STRUCT
{
	void **s;
	int num;
	int top;	
}MY_STACK;

typedef struct LOGIC_EXPRESSION_STRUCT
{
	long int codebegin;
	long int true_addr; //true link
	long int false_addr; //false link
	unsigned char   logic_exp_encode[MAX_BUFLINE];
	long int    encode_len;
}LOGIC_EXPRESSION;

#define OP_LOGIC_LEFT                   0x28
#define OP_LOGIC_RIGHT                  0x29
#define OP_LOGIC_NOT                    0x21
#define OP_LOGIC_OR                     0x7c
#define OP_LOGIC_AND                    0x26


extern int   encode_code_offset(unsigned char *buf, unsigned long code_offset);
extern int   get_source_number(char *source, int *source_index, signed long int *number, unsigned char *number_id);
extern int   get_source_op(char *source, int *source_index, unsigned char *op);
extern void  skip_space(char *data,int *index);
extern unsigned char *   target_code;
extern unsigned long int target_offset;
extern int               system_if_index;

void backpatch(LOGIC_EXPRESSION *pe, bool flag, int codebegin);
int merge_expression(int op);

MY_STACK my_op_stack;
MY_STACK my_exp_stack;

int get_pri(int op)
{
	int ret;
	
	switch(op) 
	{
	case OP_LOGIC_LEFT:
		ret = 0;
		break;	
	case OP_LOGIC_OR:
		ret = 1;
		break;
	case OP_LOGIC_AND:
		ret = 2;
		break;
	case OP_LOGIC_NOT:
		ret = 3;
		break;
	default:
		ret = -1;
	}
	return ret;
}

int init_stack(MY_STACK *st, int num)
{
	if ((st->s = (void **)malloc(num * sizeof(void *))) == NULL)
		return -1;
	st->num = num;
	st->top = 0;
	return 1;
}

int free_stack(MY_STACK *st)
{
	if (st->s != NULL)
	{
		free(st->s);
		st->s = NULL;
	}
	st->num = 0;
	st->top = 0;
	return 1;
}

int push_stack(MY_STACK *st, void *pdata)
{
	if (st->top >= (st->num - 1))
		return -1;
    st->s[st->top] = pdata;
	st->top++;
	return st->top;
}

int pop_stack(MY_STACK *st, void **p)
{
	if (st->top == 0)
		return -1;
	st->top--;
	*p = st->s[st->top];
	return 1;
}

/*return val -1 all ptr is NULL
              0 left_idx is min
              1 right_idx is min
              2 not_idx is min
              3 and_idx is min
              4 or_idx is min
*/
int min_idx(char *logic_op_idx[5], char **pcur_op_idx)
{
	int i, j;
	
	*pcur_op_idx = NULL;
	j = 0;
	
	for (i=0; i<5; i++)
	{
		if (logic_op_idx[i] != NULL)
		{
			*pcur_op_idx = logic_op_idx[i];
			j = i;
			break;
		}
	}
	if (*pcur_op_idx == NULL) 
        return -1;     
	
	for (i=0; i<5; i++)
	{
		if ((logic_op_idx[i] != NULL) && (logic_op_idx[i] < *pcur_op_idx))
		{
			*pcur_op_idx = logic_op_idx[i];
			j = i;
		}
	}
	
	return j;
}

/*
  将中缀表达式转换为后缀表达式的算法思想：
　　·当读到数字直接送至输出队列中
　　·当读到运算符t时，
　　　　　a.将栈中所有优先级高于或等于t的运算符弹出，送到输出队列中； 
　　　　　b.t进栈
　　·读到左括号时总是将它压入栈中
　　·读到右括号时，将靠近栈顶的第一个左括号上面的运算符全部依次弹出，送至输出队列后，再丢弃左括号。
  中缀表达式全部读完后，若栈中仍有运算符，将其送到输出队列中。

  此函数完成将栈中所有优先级高于或等于t的运算符弹出，再取表达式stack中的操作数进行运算
*/
int process_logic_op(int op_in_expression)
{
	int ret;
	int *p;
	int *p1;
	
	ret = pop_stack(&my_op_stack, (void **)&p);
	while ((ret != -1) && (get_pri(*p) >= get_pri(op_in_expression))) 
	{
		if (merge_expression(*p) < 0)
		{
			free(p);
			return -1;
		}
		free(p);
		ret = pop_stack(&my_op_stack, (void **)&p);
	}
	if (ret != -1)
	{
		p1 = (int *)malloc(sizeof(int));
		if (p1 == NULL)
		{
			strcpy(print_rec, "system error, can't malloc memory!");
			output_message(OUTPUT_ERROR,system_line_number, print_rec);
			return -1;
		}
		*p1 = *p;
		if (push_stack(&my_op_stack, p1) < 0)
		{
			output_message(OUTPUT_ERROR,system_line_number,"stack overflow!");
			free(p1);
			return -1;
		}
	}
	
	p1 = (int *)malloc(sizeof(int));
	if (p1 == NULL)
	{
		strcpy(print_rec, "system error, can't malloc memory!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		return -1;
	}
	*p1 = op_in_expression;
	if (push_stack(&my_op_stack, p1) < 0)
	{
		output_message(OUTPUT_ERROR,system_line_number,"stack overflow!");
		free(p1);
		return -1;
	}
		
	return 1;
}

int encode_if_code(LOGIC_EXPRESSION *p, int number1, unsigned char number_id1, 
				   unsigned char op, int number2, unsigned char number_id2)
{
	int     command_skip_len;
	signed long int offset_address;
	
	p->codebegin = 0;
	p->encode_len = 0;
	command_skip_len = 0;

	p->logic_exp_encode[p->encode_len++] = (unsigned char)(CMD_IF & 0xff);
	p->logic_exp_encode[p->encode_len++] = (unsigned char)((CMD_IF & 0xff00) >> 8);
	
	offset_address = p->encode_len;
	p->logic_exp_encode[p->encode_len++] = (unsigned char)command_skip_len;

	p->false_addr = p->encode_len;
	p->encode_len += encode_code_offset(p->logic_exp_encode + p->encode_len, 0);
    command_skip_len = command_skip_len + 4;

	p->logic_exp_encode[p->encode_len++] = number_id1;
    *(signed long int *)&p->logic_exp_encode[p->encode_len] = (signed long int)number1;
    p->encode_len += 4;
	command_skip_len += 5;

	p->logic_exp_encode[p->encode_len++] = (unsigned char)op;
	command_skip_len++;

	p->logic_exp_encode[p->encode_len++] = number_id2;
    *(signed long int *)&p->logic_exp_encode[p->encode_len] = (signed long int)number2;
    p->encode_len += 4;
	command_skip_len += 5;

	p->logic_exp_encode[offset_address]=(unsigned char)command_skip_len;

	/*Insert GOTO*/
	p->logic_exp_encode[p->encode_len++] = (unsigned char)(CMD_GOTO & 0xff);
	p->logic_exp_encode[p->encode_len++] = (unsigned char)((CMD_GOTO & 0xff00) >> 8);
	p->logic_exp_encode[p->encode_len++] = 4;
	p->true_addr = p->encode_len;
	p->encode_len += encode_code_offset(p->logic_exp_encode + p->encode_len, 0);

	return 1;
}

int push_logic_exp(char *pexp)
{
	signed long int number1,number2;
	int index = 0;
	unsigned char   number_id1;
    unsigned char   number_id2;
	unsigned char   op;
	LOGIC_EXPRESSION *p;
	
	if(TTN_OK != get_source_number(pexp, &index, &number1, &number_id1))
    {
        strcpy(print_rec,"IF parameter 1 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
		return -1;
    }
    number_id1 = mc_information.variable; //force variable
	
    if(TTN_OK != get_source_op(pexp, &index, &op))
    {
        strcpy(print_rec, "IF parameter 2 op must be one of (<,=,>,<=,!=,>=).");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
		op = OP_ID_EQ;
    }
    
    if(TTN_OK != get_source_number(pexp, &index, &number2, &number_id2))
    {
        strcpy(print_rec,"IF parameter 3 must be int .");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
	    return -1;
    }

	/*check the exp , if there are more symbols, error*/
	skip_space(pexp, &index);
	if (pexp[index] != 0)
	{
		sprintf(print_rec, "Logic expression error, %s have some problem!", pexp);
		output_message(OUTPUT_ERROR,system_line_number,print_rec);
		return -1;
	}

	p = (LOGIC_EXPRESSION *)malloc(sizeof(LOGIC_EXPRESSION));
	if (p == NULL)
	{
		strcpy(print_rec, "system error, can't malloc memory!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		return -1;
	}

    encode_if_code(p, number1, number_id1, op, number2, number_id2);
	if (push_stack(&my_exp_stack, p) < 0)
	{
		output_message(OUTPUT_ERROR,system_line_number,"stack overflow!");
		free(p);
		return -1;
	}
	
	return 1;
}

void backpatch(LOGIC_EXPRESSION *pe, bool flag, int codebegin)
{
	int tmp;
	int tmp1;

	if (flag == false)
	{
		tmp = pe->false_addr;
		while(tmp != 0)
		{
			tmp1 = *(int *)(pe->logic_exp_encode + tmp);
			encode_code_offset(pe->logic_exp_encode + tmp, codebegin);
			tmp = tmp1;
		}
	}
	else
	{
		tmp = pe->true_addr;
		while(tmp != 0)
		{
			tmp1 = *(int *)(pe->logic_exp_encode + tmp);
			encode_code_offset(pe->logic_exp_encode + tmp, codebegin);
			tmp = tmp1;
		}
	}

	return;
}

int link_exp(LOGIC_EXPRESSION *pe1, LOGIC_EXPRESSION *pe2, bool flag)
{
	int ret = -1;
	int tmp;
	int tmp1;
	
	if (flag == true)
	{
		/*get pe2 ture link tail*/
		tmp = pe2->true_addr;
		while(tmp != 0)
		{
			tmp1 = tmp;
			tmp = *(int *)(pe2->logic_exp_encode + tmp);
		}

		encode_code_offset(pe2->logic_exp_encode + tmp1, pe1->true_addr);
		
		ret = pe2->true_addr;
	}
	else
	{
		/*get pe1 false link tail*/
		tmp = pe2->false_addr;
		while(tmp != 0)
		{
			tmp1 = tmp;
			tmp = *(int *)(pe2->logic_exp_encode + tmp);
		}
		
		encode_code_offset(pe2->logic_exp_encode + tmp1, pe1->false_addr);
		
		ret = pe2->false_addr;
	}

	return ret;
}

int merge_exp_encode(LOGIC_EXPRESSION *pe1, LOGIC_EXPRESSION *pe2)
{
	int i;

	if ((pe1->encode_len + pe2->encode_len) > MAX_BUFLINE)
	{
		strcpy(print_rec, "encode buf overflow, expression is too long!");
        output_message(OUTPUT_ERROR,system_line_number,print_rec);
		return -1;
	}
	
	for (i = 0; i < pe2->encode_len; i++)
	{
		pe1->logic_exp_encode[pe1->encode_len++] = 
			pe2->logic_exp_encode[pe2->codebegin + i];
	}

	return 1;
}

/*update expression addr from relative to absolute*/
void update_pe_addr(LOGIC_EXPRESSION *pe, int offset)
{
	int tmp;
	int i;

	for (i = 0; i < pe->encode_len; i++)
	{
		/*find offset and goto position, 25 is (IF + GOTO) code len*/
		if ((i%25 == 3) || (i%25 == 21))
		{
			tmp = *(int *)(pe->logic_exp_encode + pe->codebegin + i);
			
			/*if it is not true or false out position, update it*/
			if (tmp != 0) 
			{
				encode_code_offset(pe->logic_exp_encode + pe->codebegin + i, tmp + offset);
			}
		}
	}
	
	for (i = 0; i < pe->encode_len; i++)
	{
		pe->logic_exp_encode[pe->codebegin + offset + pe->encode_len - i -1] = 
			pe->logic_exp_encode[pe->codebegin + pe->encode_len - i -1];
	}
	pe->codebegin += offset;
	pe->true_addr += offset;
	pe->false_addr += offset;

	return;
}

LOGIC_EXPRESSION * merge_or_expression(void)
{
	LOGIC_EXPRESSION *pe1;
	LOGIC_EXPRESSION *pe2;

	pe1 = pe2 = NULL;
	
	if (pop_stack(&my_exp_stack, (void **)&pe2) < 0)
	{
		strcpy(print_rec, "logic expression error,maybe OR have no parament!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		return NULL;
	}
	if (pop_stack(&my_exp_stack, (void **)&pe1) < 0)
	{
		strcpy(print_rec, "logic expression error,maybe OR have no two parament!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		if (pe1 != NULL)
			free(pe1);
		return NULL;
	}

	/*now pe2 will be after pe1, so we update pe2 addr*/
	update_pe_addr(pe2, pe1->encode_len);
	
	backpatch(pe1, false, pe2->codebegin);
	pe1->true_addr = link_exp(pe1, pe2, true);
	pe1->false_addr = pe2->false_addr;
	if (merge_exp_encode(pe1, pe2) < 0)
	{
		if (pe1 != NULL)
			free(pe1);
		if (pe2 != NULL)
			free(pe2);

		return NULL;
	}
	
	if (pe2 != NULL)
		free(pe2);

	return pe1;
}

LOGIC_EXPRESSION * merge_and_expression(void)
{
	LOGIC_EXPRESSION *pe1;
	LOGIC_EXPRESSION *pe2;
	
	pe1 = pe2 = NULL;
	
	if (pop_stack(&my_exp_stack, (void **)&pe2) < 0)
	{
		strcpy(print_rec, "logic expression error,maybe AND have no parament!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		return NULL;
	}
	if (pop_stack(&my_exp_stack, (void **)&pe1) < 0)
	{
		strcpy(print_rec, "logic expression error,maybe AND have no two parament!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		if (pe1 != NULL)
			free(pe1);
		return NULL;
	}
	
	/*now pe2 will be after pe1, so we update pe2 addr*/
	update_pe_addr(pe2, pe1->encode_len);
	
	backpatch(pe1, true, pe2->codebegin);
	pe1->false_addr = link_exp(pe1, pe2, false);
	pe1->true_addr = pe2->true_addr;
	if (merge_exp_encode(pe1, pe2) < 0)
	{
		if (pe1 != NULL)
			free(pe1);
		if (pe2 != NULL)
			free(pe2);
		
		return NULL;
	}
	
	if (pe2 != NULL)
		free(pe2);
	
	return pe1;
}

LOGIC_EXPRESSION * merge_not_expression(void)
{
	LOGIC_EXPRESSION *pe;
	int tmp;

	if (pop_stack(&my_exp_stack, (void **)&pe) < 0)
	{
		strcpy(print_rec, "logic expression error,maybe NOT have no parament!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		return NULL;
	}

	tmp = pe->true_addr;
	pe->true_addr = pe->false_addr;
	pe->false_addr = tmp;

	return pe;	
}

int merge_expression(int op)
{
	LOGIC_EXPRESSION *pe;

	pe = NULL;
		
	switch(op) 
	{
	case OP_LOGIC_OR:
		pe = merge_or_expression();
		if (pe == NULL)
			return -1;
		break;
	case OP_LOGIC_AND:
		pe = merge_and_expression();
		if (pe == NULL)
			return -1;
		break;
	case OP_LOGIC_NOT:
		pe = merge_not_expression();
		if (pe == NULL)
			return -1;
		break;
	default:
	  	strcpy(print_rec, "Logic expression error!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		return -1;
	}
	
	if (push_stack(&my_exp_stack, pe) < 0)
	{
		strcpy(print_rec, "stack overflow, maybe logic expression to long!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		if (pe != NULL)
			free(pe);
		return -1;
	}
	
	return 1;
}

void exit_parselogicexp(void)
{
	int ret;
	int *p;
	LOGIC_EXPRESSION *p1;
	
	ret = pop_stack(&my_op_stack, (void **)&p);
	while (ret != -1)
	{
		free(p);
		ret = pop_stack(&my_op_stack, (void **)&p);
	}
	free_stack(&my_op_stack);


	ret = pop_stack(&my_exp_stack, (void **)&p1);
	while (ret != -1)
	{
		free(p1);
		ret = pop_stack(&my_exp_stack, (void **)&p1);
	}
	free_stack(&my_exp_stack);
	
	return;
}

void optimizegoto(long int goto_optimize_start, long int goto_optimize_end)
{
	long int i;
	long int goto_addr;

	for(i = goto_optimize_start; i < goto_optimize_end; i++)
	{
		if ((i - goto_optimize_start)%25 == 21)
		{
			goto_addr = *(int *)(target_code + i);
			if (goto_addr == (i + 4))
			{
				target_code[i - 19] = 22;
			}
		}
	}

	return;
}

long int addinto_target(LOGIC_EXPRESSION *pe)
{
	int  i;
	int  tmp;
	int  tmp1;
	long int goto_optimize_start;
	long int goto_optimize_end;

	for (i = 0; i < pe->encode_len; i++)
	{
		/*find offset and goto position, 25 is (IF + GOTO) code len*/
		if ((i%25 == 3) || (i%25 == 21))
		{
			tmp = *(int *)(pe->logic_exp_encode + pe->codebegin + i);
			
			/*if it is not true or false out position, update it*/
			if (tmp != 0) 
			{
				encode_code_offset(pe->logic_exp_encode + pe->codebegin + i, tmp + target_offset);
			}
		}
	}
	pe->true_addr += target_offset;
	pe->false_addr += target_offset;

	goto_optimize_start = target_offset;

	for (i = 0; i < pe->encode_len; i++)
	{
		target_code[target_offset++] = pe->logic_exp_encode[i]; 
	}

	/*now target_offset is true out position, update true link*/
	tmp = pe->true_addr;
	while(tmp != 0)
	{
		tmp1 = *(int *)(target_code + tmp);
		encode_code_offset(target_code + tmp, target_offset);
		tmp = tmp1;
	}

	goto_optimize_end = target_offset;
	optimizegoto(goto_optimize_start, goto_optimize_end);

	return pe->false_addr;
}

int mystrupr(char *pbuf)
{
	char *p;

	for(p = pbuf; *p != 0; p++)
	{
		if ((*p >= 'a') && (*p <= 'z'))
			*p = *p + 'A' - 'a';
	}
	return 1;
}

long int parselogicexp(char *pexp, int *index1)
{
	int  index;
	char line_buffer[MAX_BUFLINE];
	char temp[MAX_BUFLINE];
	char *logic_op_idx[5];
	char *cur_op_idx;
	char *pbuf;
	char ch;
	int  len;
	int  flag_idx;
	int  *p;
	long int  ret;
	LOGIC_EXPRESSION *pe;

	if (init_stack(&my_op_stack, MAX_BUFLINE) < 0)
	{
		strcpy(print_rec, "system error, can't malloc memory!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		return -1;
	}
	if (init_stack(&my_exp_stack, MAX_BUFLINE) < 0)
	{
		strcpy(print_rec, "system error, can't malloc memory!");
        output_message(OUTPUT_ERROR,system_line_number, print_rec);
		return -1;
	}

	memset(line_buffer, 0x00, MAX_BUFLINE);
	strcpy(line_buffer, pexp + *index1);
	//strupr(line_buffer);
	/*modified by kent, for Linux have no function strupr*/
	mystrupr(line_buffer);

	pbuf = line_buffer;
	len = strlen(line_buffer);
	logic_op_idx[0] = strchr(line_buffer, '(');
	logic_op_idx[1] = strchr(line_buffer, ')');
	logic_op_idx[2] = strstr(line_buffer, "NOT");
	logic_op_idx[3] = strstr(line_buffer, "AND");
	logic_op_idx[4] = strstr(line_buffer, "OR");
	cur_op_idx = NULL;

	while(pbuf != &line_buffer[len])
	{
		flag_idx = min_idx(logic_op_idx, &cur_op_idx);
		if (flag_idx != -1)
		{
			memcpy(temp, pbuf, cur_op_idx - pbuf);
			temp[cur_op_idx - pbuf] = 0;
						
			index = 0;
			skip_space(temp, &index);
			ch = temp[index];
			if (ch=='@' || ch=='#' || (ch>='0' && ch<='9'))
			{
			    if (push_logic_exp(temp) < 0)
				{
					exit_parselogicexp();
					return -1;
				}
			}

			pbuf = cur_op_idx;
			switch(flag_idx)
			{
			case 0:
				p = (int *)malloc(sizeof(int));
				if (p == NULL)
				{
					strcpy(print_rec, "system error, can't malloc memory!");
					output_message(OUTPUT_ERROR,system_line_number, print_rec);
					exit_parselogicexp();
					return -1;
				}
				*p = OP_LOGIC_LEFT;
				if (push_stack(&my_op_stack, p) < 0)
				{
					strcpy(print_rec, "stack overflow, maybe logic expression to long!");
					output_message(OUTPUT_ERROR,system_line_number, print_rec);
					free(p);
					exit_parselogicexp();
					return -1;
				}

				pbuf += 1;
				logic_op_idx[0] = strchr(pbuf, '(');
				break;
			case 1:
				ret = pop_stack(&my_op_stack, (void **)&p);
				while ((ret != -1) && (*p != OP_LOGIC_LEFT))
				{
					if (merge_expression(*p) < 0)
					{
						free(p);
						exit_parselogicexp();
						return -1;
					}
					
					free(p);

					ret = pop_stack(&my_op_stack, (void **)&p);
				}
				if (ret == -1)
				{
					strcpy(print_rec, ") have no pair (");
					output_message(OUTPUT_ERROR,system_line_number, print_rec);
					exit_parselogicexp();
					return -1;
				}
				
				pbuf += 1;
				logic_op_idx[1] = strchr(pbuf, ')');
				break;
			case 2:
				if (process_logic_op(OP_LOGIC_NOT) < 0)
				{
					exit_parselogicexp();
					return -1;
				}
			
				pbuf += 3;
				logic_op_idx[2] = strstr(pbuf, "NOT");
				break;
			case 3:
				if (process_logic_op(OP_LOGIC_AND) < 0)
				{
					exit_parselogicexp();
					return -1;
				}
				
				pbuf += 3;
				logic_op_idx[3] = strstr(pbuf, "AND");
				break;
			case 4:
				if (process_logic_op(OP_LOGIC_OR) < 0)
				{
					exit_parselogicexp();
					return -1;
				}

				pbuf += 2;
				logic_op_idx[4] = strstr(pbuf, "OR");
				break;
			default:
				;
			}
		}
		else  // there has no more logic operator
			break;
	}
	
	index = 0;
	skip_space(pbuf, &index);
	ch = pbuf[index];
	if (ch=='@' || ch=='#' || (ch>='0' && ch<='9'))
	{
		if (push_logic_exp(pbuf) < 0)
		{
			exit_parselogicexp();
			return -1;
		}
	}

	ret = pop_stack(&my_op_stack, (void **)&p);
	while (ret != -1)
	{
		if (merge_expression(*p) < 0)
		{
			free(p);
			exit_parselogicexp();
			return -1;
		}

		free(p);

		ret = pop_stack(&my_op_stack, (void **)&p);
	}

	if (my_exp_stack.top != 1)
	{
		strcpy(print_rec, "logic expression error!");
		output_message(OUTPUT_ERROR,system_line_number, print_rec);
		exit_parselogicexp();
		return -1;
	}
	else
	{
		if (pop_stack(&my_exp_stack, (void **)&pe) < 0)
		{
			strcpy(print_rec, "logic expression stack error!");
			output_message(OUTPUT_ERROR,system_line_number, print_rec);
			exit_parselogicexp();
			return -1;
		}

		ret = addinto_target(pe);
	
		free(pe);
	}

	free_stack(&my_op_stack);
	free_stack(&my_exp_stack);
	return ret;
}
