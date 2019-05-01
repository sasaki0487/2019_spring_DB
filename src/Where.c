#include <stdlib.h>
#include <string.h>
#include "Where.h"
#include "Command.h"
#include "Table.h"

const char *opr[6] = {"=","!=",">","<",">=","<="};

int get_op(char* op){
    for(int i = 0 ; i < 6 ; i++){
        if(!strcmp(op,opr[i])){
            return i;
        }
    }
    return -1;
}

Where_t* new_Where(){
    Where_t *cmd = (Where_t*)malloc(sizeof(Where_t));
    memset(cmd, 0, sizeof(Where_t));

    cmd->logic = 0;
    cmd->field1 = 0;
    cmd->op1 = 0;
    cmd->cmp_str1 = NULL;
    cmd->field2 = 0;
    cmd->op2 = 0;
    cmd->cmp_str2 = NULL;

    return cmd;
}

Where_t* parse_Where(Command_t *cmd){
    Where_t *wh = new_Where();
    int lo_idx = 0;
    char *op_pos;
    for(int i = 1 + cmd->where_flag ; i < cmd->args_len ; i++){
        if(!strncmp(cmd->args[i],"or",2)){
            wh->logic = 1;
            lo_idx = i;
            break;
        }
        if(!strncmp(cmd->args[i],"and",3)){
            wh->logic = 2;
            lo_idx = i;
            break;
        }
    }
    if(wh->logic == 0){
        if((cmd->args_len - cmd->where_flag == 2) || (cmd->cmd_args.sel_args.offset_pos - cmd->where_flag == 2) || (cmd->cmd_args.sel_args.limit_pos - cmd->where_flag == 2)){

            wh->field1 = get_field(cmd->args[cmd->where_flag+1]);
            for(int i = 5 ; i >= 0 ; i--){
                op_pos = strstr(cmd->args[cmd->where_flag+1],opr[i]);
                if(op_pos){
                    wh->op1 = i;
                    wh->cmp_str1 = op_pos + strlen(opr[i]);
                    break;
                }
            }
        }
        else{
            wh->field1 = get_field(cmd->args[cmd->where_flag+1]);
            wh->op1 = get_op(cmd->args[cmd->where_flag+2]);
            wh->cmp_str1 = cmd->args[cmd->where_flag+3];
        }
    }
    else{
        if(lo_idx - cmd->where_flag == 2){
            wh->field1 = get_field(cmd->args[cmd->where_flag+1]);
            for(int i = 5 ; i >= 0 ; i--){
                op_pos = strstr(cmd->args[cmd->where_flag+1],opr[i]);
                if(op_pos){
                    wh->op1 = i;
                    wh->cmp_str1 = op_pos + strlen(opr[i]);
                    break;
                }
            }
        }
        else{
            wh->field1 = get_field(cmd->args[cmd->where_flag+1]);
            wh->op1 = get_op(cmd->args[cmd->where_flag+2]);
            wh->cmp_str1 = cmd->args[cmd->where_flag+3];
        }
        if((cmd->args_len - lo_idx == 2) || (cmd->cmd_args.sel_args.offset_pos - lo_idx == 2) || (cmd->cmd_args.sel_args.limit_pos - lo_idx == 2)){
            wh->field2 = get_field(cmd->args[lo_idx+1]);
            for(int i = 5 ; i >= 0 ; i--){
                op_pos = strstr(cmd->args[lo_idx+1],opr[i]);
                if(op_pos){
                    wh->op2 = i;
                    wh->cmp_str2 = op_pos + strlen(opr[i]);
                    break;
                }
            }
        }
        else{
            wh->field2 = get_field(cmd->args[lo_idx+1]);
            wh->op2 = get_op(cmd->args[lo_idx+2]);
            wh->cmp_str2 = cmd->args[lo_idx+3];
        }
    }
    return wh;
}

int check_logic(User_t *user,int field ,int op ,char *str){
    if(field == 1){
        double num = atof(str);
        if(op == 0) return user->id == num;
        else if(op == 1) return user->id != num;
        else if(op == 2) return user->id > num;
        else if(op == 3) return user->id < num;
        else if(op == 4) return user->id >= num;
        else if(op == 5) return user->id <= num;
    }
    else if(field == 2){
        if(op == 0) return !strcmp(user->name,str);
        else if(op == 1) return strcmp(user->name,str);
    }
    else if(field == 3){
        if(op == 0) return !strcmp(user->email,str);
        else if(op == 1) return strcmp(user->email,str);
    }
    else if(field == 4){
        double num = atof(str);
        if(op == 0) return user->age == num;
        else if(op == 1) return user->age != num;
        else if(op == 2) return user->age > num;
        else if(op == 3) return user->age < num;
        else if(op == 4) return user->age >= num;
        else if(op == 5) return user->age <= num;
    }
    return 0;
}

int check_where(Table_t *table , Where_t *wh , int idx){
    User_t *user = get_User(table,idx);
    int result1,result2;
    if(wh->logic == 0){
        return check_logic(user,wh->field1,wh->op1,wh->cmp_str1);
    }
    else{
        result1 = check_logic(user,wh->field1,wh->op1,wh->cmp_str1);
        result2 = check_logic(user,wh->field2,wh->op2,wh->cmp_str2);
        if(wh->logic == 1) return result1 | result2;
        else if(wh->logic == 2) return result1 & result2;
    }
    return 1;
}
