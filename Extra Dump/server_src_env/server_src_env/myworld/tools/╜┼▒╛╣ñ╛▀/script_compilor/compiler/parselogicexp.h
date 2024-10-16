#define MAX_BUFLINE  1024
#define OP_LOGIC_LEFT                   0x28
#define OP_LOGIC_RIGHT                  0x29
#define OP_LOGIC_NOT                    0x21
#define OP_LOGIC_OR                     0x7c
#define OP_LOGIC_AND                    0x26

long int parselogicexp(char *pexp, int *index1);
