#ifndef WHERE_H
#define WHERE_H
#include "Command.h"
#include "Table.h"

typedef struct where_clause{
    int logic;
    int field1;
    int op1;
    char* cmp_str1;
    int field2;
    int op2;
    char* cmp_str2;
}Where_t;

int check_where(Table_t *table ,Where_t *wh , int idx);
int get_op(char* op);
Where_t* new_Where();
Where_t* parse_Where(Command_t *cmd);
int check_logic(User_t *user,int field,int op , char *str);
#endif
