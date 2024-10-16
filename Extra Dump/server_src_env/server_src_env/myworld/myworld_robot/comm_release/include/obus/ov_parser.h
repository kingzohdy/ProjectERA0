#ifndef OV_PARSER_H
#define OV_PARSER_H

#include "scew.h"

#ifdef __cplusplus
extern "C"
{
#endif

void parser_convert_string(unsigned char* pszStr);
void parser_convert_element(scew_element* pstElement);
int parser_set_encoding(scew_parser* pstParser, const char* pszEncoding);
int parser_set_default_encoding(scew_parser* pstParser);

void tree_set_encoding(scew_tree* pstTree, const char* pszEncoding);
void tree_set_default_encoding(scew_tree* pstTree);
void tree_set_standalone(scew_tree* pstTree, int standalone);

scew_parser* parser_create(void);
#define parser_free		scew_parser_free

#define parser_load_file	scew_parser_load_file
#define parser_load_fp		scew_parser_load_file_fp
#define parser_load_buffer	scew_parser_load_buffer

scew_tree* parser_tree(scew_parser* pstParser);

unsigned int tree_save_file(scew_tree* pstTree, const char* pszFile);


#ifdef __cplusplus
}
#endif

#endif /* OV_PARSER_H */
