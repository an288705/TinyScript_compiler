#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#define MAX_SYMBOL_TABLE_SIZE 500

instruction* IR;

void expression(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count);
void statement(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count);
void block(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count, int proci);

void print_st(symbol* symbol_table, int* st_ptr)
{
    for(int i = 0; i<(*st_ptr);i++)
    {
        printf("row %d: %d %s %d %d %d\n",i,symbol_table[i].kind,symbol_table[i].name,symbol_table[i].val,symbol_table[i].level,symbol_table[i].addr);
    }
}

int search_table(char name[], int level, symbol* symbol_table, int* st_ptr)
{
    for(int i = (*st_ptr); i>=0;i--)
    {
        //search from the end to prioritize local scope. add a check to avoid accessing symbols of higher level
        if (strcmp(name,symbol_table[i].name)==0 && symbol_table[i].level<=level)
        {
            return i;
        }
    }

    return -1; //return -1 if not found
}

void factor(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count)
{
    if(atoi(token)==identsym)
    {
        char ident[12]; //ident cant be longer than 11 char
        int i; //index of symbol table
        //first scan will return ident name. scan twice
		strcpy(ident,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;

        i = search_table(ident,level,symbol_table,st_ptr);
        if(i==-1)
        {
            printf("Error : undeclared variable or constant in equation\n");
            exit(-1);
        }
        if (symbol_table[i].kind==1) //ident is a const
        {
            (*cci)++;
            IR[(*cci)].opcode = 1;
            strcpy(IR[(*cci)].op,"LIT");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = symbol_table[i].val;
        }
        else //ident is a var
        {
            (*cci)++;
            IR[(*cci)].opcode = 3;
            strcpy(IR[(*cci)].op,"LOD");
            IR[(*cci)].l = level - symbol_table[i].level; //the L is the distance from the current level to the level of initialization
            IR[(*cci)].m = symbol_table[i].addr;
        }

		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
    } 
    else if(atoi(token)==numbersym)
    {
        char num[6]; //nums cannot exceed 5 digits

		strcpy(num,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        (*cci)++;
        IR[(*cci)].opcode = 1;
        strcpy(IR[(*cci)].op,"LIT");
        IR[(*cci)].l = 0;
        IR[(*cci)].m = atoi(num);
		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
    }
    else if(atoi(token)==lparentsym)
    {
		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        if(atoi(token)!=rparentsym)
        {       
            printf("Error : right parenthesis must follow left parenthesis\n");
            exit(-1);
        }
		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
    }
    else if (atoi(token)==callsym)
    {
        statement(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
    }
    else
    {
        printf("Error : arithmetic equations must contain operands, parentheses, numbers, or symbols\n");
        exit(-1);
    }
    
}

void term(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count)
{
    factor(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
    while(atoi(token)==multsym || atoi(token)==slashsym || atoi(token)==modsym)
    {
        if(atoi(token)==multsym)
        {
			strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            factor(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 4;
        }
        else if (atoi(token)==slashsym)
        {
			strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            factor(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 5;
        }
        else
        {
			strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            factor(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 7;
        }

    }
    
}

void expression(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count)
{
    if (atoi(token)==minussym)
    {
		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        term(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        (*cci)++;
        IR[(*cci)].opcode = 2;
        strcpy(IR[(*cci)].op,"OPR");
        IR[(*cci)].l = 0;
        IR[(*cci)].m = 1;

        while(atoi(token)==plussym || atoi(token)==minussym)
        {
            if(atoi(token)==plussym)
            {
				strcpy(token,token_list[(*tl_ptr)].token); //get token
				(*tl_ptr)++;
                term(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
                (*cci)++;
                IR[(*cci)].opcode = 2;
                strcpy(IR[(*cci)].op,"OPR");
                IR[(*cci)].l = 0;
                IR[(*cci)].m = 2;
            }
            else
            {
				strcpy(token,token_list[(*tl_ptr)].token); //get token
				(*tl_ptr)++;
                term(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
                (*cci)++;
                IR[(*cci)].opcode = 2;
                strcpy(IR[(*cci)].op,"OPR");
                IR[(*cci)].l = 0;
                IR[(*cci)].m = 3;
            }
        }
    }
    else
    {
        if(atoi(token)==plussym)
        {
			strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
        }
        term(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        while(atoi(token)==plussym || atoi(token)==minussym)
        {
            if(atoi(token)==plussym)
            {
				strcpy(token,token_list[(*tl_ptr)].token); //get token
				(*tl_ptr)++;
                term(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
                (*cci)++;
                IR[(*cci)].opcode = 2;
                strcpy(IR[(*cci)].op,"OPR");
                IR[(*cci)].l = 0;
                IR[(*cci)].m = 2;
            }
            else
            {
				strcpy(token,token_list[(*tl_ptr)].token); //get token
				(*tl_ptr)++;
                term(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
                (*cci)++;
                IR[(*cci)].opcode = 2;
                strcpy(IR[(*cci)].op,"OPR");
                IR[(*cci)].l = 0;
                IR[(*cci)].m = 3;
            }
        }
    }
}

void condition(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count)
{
    if(atoi(token)==oddsym)
    {
		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        (*cci)++;
        IR[(*cci)].opcode = 2;
        strcpy(IR[(*cci)].op,"OPR");
        IR[(*cci)].l = 0;
        IR[(*cci)].m = 6;
    }
    else
    {
        expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        if(atoi(token)==leqsym)
        {
            strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 11;
        }
        else if(atoi(token)==geqsym)
        {
            strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 13;
        }
        else if (atoi(token)==neqsym)
        {
            strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 9;
        }
        else if (atoi(token)==eqsym)
        {
            strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 8;
        }
        else if (atoi(token)==lessym)
        {
            strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 10;
        }
        else if (atoi(token)==gtrsym)
        {
            strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 12;
        }
        else
        {
            printf("Error : condition must contain comparison operator\n");
            exit(-1);
        }
    }  
}

void statement(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count)
{
    if(atoi(token)==identsym)
    {
        char ident[12]; //ident cant be longer than 11 char
        int i; //index of symbol table

		strcpy(ident,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        i = search_table(ident,level,symbol_table,st_ptr);
        if(i==-1)
        {
            printf("Error : undeclared variable or constant in equation\n");
            exit(-1);
        }
        if (symbol_table[i].kind!=2)
        {
            printf("Error : only variable values may be altered\n");
            exit(-1);
        }
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        if(atoi(token)!=becomessym)
        {
            printf("Error : assignment statements must use :=\n");
            exit(-1);
        }
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        (*cci)++;

        IR[(*cci)].opcode = 4;
        strcpy(IR[(*cci)].op,"STO");
        IR[(*cci)].l = level - symbol_table[i].level; //the L is the distance from the current level to the level of initialization
        IR[(*cci)].m = symbol_table[i].addr;
    }
    else if(atoi(token)==callsym)
    {
        char ident[12];
        int i = 0;
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        if(atoi(token)!=identsym)
        {  
            printf("Error : const, var, procedure, call, and read keywords must be followed by identifier\n");
            exit(-1);
        }
		strcpy(ident,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;

        i = search_table(ident,level,symbol_table,st_ptr);
        if(i==-1)
        {
            printf("Error : undeclared procedure for call\n");
            exit(-1);
        }
        
		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;

        if (atoi(token)==lparentsym)
        {
            //get the token. the ident name will be passed into factor
            strcpy(token,token_list[(*tl_ptr)].token); //get token
		    (*tl_ptr)++;

            if(symbol_table[i].param==0)
            {
                printf("Error : parameters must be declared\n");
                exit(-1);
            }

            expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);

            if (atoi(token)!=rparentsym)
            {                   
                printf("Error : right parenthesis must follow left parenthesis\n");
                exit(-1);
            }
            strcpy(token,token_list[(*tl_ptr)].token); //get token
		    (*tl_ptr)++;
        }
        else
        {
            (*cci)++;
            IR[(*cci)].opcode = 1;
            strcpy(IR[(*cci)].op,"LIT");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 0; 
        }

        (*cci)++;
        IR[(*cci)].opcode = 5;
        strcpy(IR[(*cci)].op,"CAL");
        IR[(*cci)].l = level - symbol_table[i].level;
        IR[(*cci)].m = symbol_table[i].val; //use a dummy val for call
        
    }
    else if(atoi(token)==returnsym)
    {
        if(level==0)
        {
            printf("Error : cannot return from main\n");
            exit(-1);
        }
        
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        if(atoi(token)==lparentsym)
        {
            strcpy(token,token_list[(*tl_ptr)].token); //get token
		    (*tl_ptr)++;
            expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0; //change
            IR[(*cci)].m = 0;

            if(atoi(token)!=rparentsym)
            {                
                printf("Error : right parenthesis must follow left parenthesis\n");
                exit(-1);
            }
            
            strcpy(token,token_list[(*tl_ptr)].token); //get token
		    (*tl_ptr)++;
        }
        else
        {
            (*cci)++;
            IR[(*cci)].opcode = 1;
            strcpy(IR[(*cci)].op,"LIT");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 0; 
            
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0; 
            IR[(*cci)].m = 0;
        }
    } 
    else if(atoi(token)==beginsym)
    {
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        statement(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        
        //when statement works perfectly according to practice IR, we should be at semicolon
        while (atoi(token)==semicolonsym)
        {
            strcpy(token,token_list[(*tl_ptr)].token); //get token
			(*tl_ptr)++;
            statement(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        }
        if(atoi(token)!=endsym)
        {
            printf("Error : begin must be followed by end\n");
            exit(-1);
        }
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
    }
    else if(atoi(token)==ifsym)
    {
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        condition(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        
        //give jpc dummy val of its IR location
        //after statement call, update jpc to real cci
        (*cci)++;
        int jpc_cci = (*cci);
        IR[(*cci)].opcode = 8;
        strcpy(IR[(*cci)].op,"JPC");
        IR[(*cci)].l = 0;
        IR[(*cci)].m = jpc_cci;

        if(atoi(token)!=thensym)
        {
            printf("Error : if must be followed by then\n");
            exit(-1);
        }
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        statement(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        
        if(atoi(token)==elsesym)
        {
            //add 1 to go to the intended line of IR
            IR[jpc_cci].m = (*cci)+1; 

            strcpy(token,token_list[(*tl_ptr)].token); //get token
            (*tl_ptr)++;

            //give jmp dummy val of its IR location
            //after statement call, update jmp to real cci
            (*cci)++;
            int jmp_cci = (*cci);
            IR[(*cci)].opcode = 7;
            strcpy(IR[(*cci)].op,"JMP");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = jmp_cci;

            statement(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
            
            //add 1 to go to the intended line of IR
            IR[jmp_cci].m = (*cci)+1;
        }
        else
        {
            //add 1 to go to the intended line of IR
            IR[jpc_cci].m = (*cci)+1; 
        }
    }  
    else if(atoi(token)==whilesym)
    {
        //save the cci to loop back to it
        //add 1 to go to the intended line of IR
        int loopi = (*cci)+1; 

        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        condition(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        if(atoi(token)!=dosym)
        {
            printf("Error : while must be followed by do\n");
            exit(-1);
        }
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        
        //give jpc dummy val of its IR cci 
        //after statement call, update jpc to real cci
        (*cci)++;
        int jpc_cci = (*cci);
        IR[(*cci)].opcode = 8;
        strcpy(IR[(*cci)].op,"JPC");
        IR[(*cci)].l = 0;
        IR[(*cci)].m = jpc_cci;

        statement(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        (*cci)++;
        IR[(*cci)].opcode = 7;
        strcpy(IR[(*cci)].op,"JMP");
        IR[(*cci)].l = 0;
        IR[(*cci)].m = loopi;

        //add 1 to go to the intended line of IR
        IR[jpc_cci].m = (*cci)+1;
    }
    else if(atoi(token)==readsym)
    {
        char ident[12]; //ident cant be longer than 11 char
        int i; //symbol table index

        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        if (atoi(token)!=identsym)
        {

            printf("Error : const, var, procedure, call, and read keywords must be followed by identifier\n");
            exit(-1);
        }
        
		strcpy(ident,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        i = search_table(ident,level,symbol_table,st_ptr);
        if(i==-1)
        {
            printf("Error : undeclared variable or constant in equation\n");
            exit(-1);
        }
        if (symbol_table[i].kind!=2) 
        {
            printf("Error : only variable values may be altered\n");
            exit(-1);
        }
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        (*cci)++;
        IR[(*cci)].opcode = 9;
        strcpy(IR[(*cci)].op,"SYS");
        IR[(*cci)].l = 0;
        IR[(*cci)].m = 2;

        (*cci)++;
        IR[(*cci)].opcode = 4;
        strcpy(IR[(*cci)].op,"STO");
        IR[(*cci)].l = level - symbol_table[i].level; //the L is the distance from the current level to the level of initialization
        IR[(*cci)].m = symbol_table[i].addr;
    }
    else if(atoi(token)==writesym)
    {
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        expression(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
        (*cci)++;
        IR[(*cci)].opcode = 9;
        strcpy(IR[(*cci)].op,"SYS");
        IR[(*cci)].l = 0;
        IR[(*cci)].m = 1;
    }  
    
}

void procdecl(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count)
{
    while(atoi(token)==procsym)
    {
        char ident[12];
        strcpy(token,token_list[(*tl_ptr)].token); //get token
	    (*tl_ptr)++;

        if(atoi(token)!=identsym)
        {
            
            printf("Error : const, var, procedure, call, and read keywords must be followed by identifier\n");
            exit(-1);
        }

        strcpy(ident,token_list[(*tl_ptr)].token); //get token
        (*tl_ptr)++;

        int check = search_table(ident,level,symbol_table,st_ptr);
        if(check!=-1 && symbol_table[check].level==level)
        {
            printf("Error : competing symbol declarations at the same level\n");
            exit(-1);
        }

        int proci = (*st_ptr);
        (*procedure_count)++;
        symbol_table[(*st_ptr)].kind = 3;
        strcpy(symbol_table[(*st_ptr)].name,ident);
        symbol_table[(*st_ptr)].val = (*procedure_count);
        symbol_table[(*st_ptr)].level = level;
        symbol_table[(*st_ptr)].addr = -1; //this will change after creation of inc statement
        symbol_table[(*st_ptr)].param = 0;
        (*st_ptr)++;
        //print_st(symbol_table,st_ptr);

        strcpy(token,token_list[(*tl_ptr)].token); //get token
        (*tl_ptr)++;

        if (atoi(token)==lparentsym)
        {
            char param[12]; //this is the parameter of the procedure

            strcpy(token,token_list[(*tl_ptr)].token); //get token
            (*tl_ptr)++;

            if (atoi(token)!=identsym)
            {
                
                printf("Error : parameters may only be specified by an identifier\n");
                exit(-1);
            }

            strcpy(param,token_list[(*tl_ptr)].token); //get token
		    (*tl_ptr)++;

            //enter into symbol table
            symbol_table[(*st_ptr)].kind = 2;
            strcpy(symbol_table[(*st_ptr)].name,param);
            symbol_table[(*st_ptr)].val = 0;
            symbol_table[(*st_ptr)].level = level+1;
            symbol_table[(*st_ptr)].addr = 3; //the argument of a procedure get the address 3
            symbol_table[(*st_ptr)].param = 0;
            (*st_ptr)++;

            /*now that there is a parameter, update the symbol table parameter to 1*/
            symbol_table[proci].param = 1; 
            
            strcpy(token,token_list[(*tl_ptr)].token); //get token
            (*tl_ptr)++;

            if (atoi(token)!=rparentsym)
            {              
                printf("Error : right parenthesis must follow left parenthesis\n");
                exit(-1);
            }
            strcpy(token,token_list[(*tl_ptr)].token); //get token
            (*tl_ptr)++;
            if (atoi(token)!=semicolonsym)
            {
                printf("Error : symbol declarations must be followed by a semicolon\n");
                exit(-1);
            }
            strcpy(token,token_list[(*tl_ptr)].token); //get token
            (*tl_ptr)++;

            block(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level+1,procedure_count,proci);
        }
        else
        {
            if (atoi(token)!=semicolonsym)
            {
                printf("Error : symbol declarations must be followed by a semicolon\n");
                exit(-1);
            }

            strcpy(token,token_list[(*tl_ptr)].token); //get token
            (*tl_ptr)++;

            block(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level+1,procedure_count,proci);
        }

        if (IR[(*cci)].opcode!=2 && IR[(*cci)].m!=0)
        {
            //if the previous instruction is not an OPR and is not a const, create instructions
            (*cci)++;
            IR[(*cci)].opcode = 1;
            strcpy(IR[(*cci)].op,"LIT");
            IR[(*cci)].l = 0;
            IR[(*cci)].m = 0;
            
            (*cci)++;
            IR[(*cci)].opcode = 2;
            strcpy(IR[(*cci)].op,"OPR");
            IR[(*cci)].l = 0; 
            IR[(*cci)].m = 0;
        }
        
        if (atoi(token)!=semicolonsym)
        {
            printf("Error : symbol declarations must be followed by a semicolon\n");
            exit(-1);
        }

        strcpy(token,token_list[(*tl_ptr)].token); //get token
        (*tl_ptr)++;
    }
}

void vardecl(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count, int* m)
{
    char ident[12];

    strcpy(token,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    if(atoi(token)!=identsym)
    {

        printf("Error : const, var, procedure, call, and read keywords must be followed by identifier\n");
        exit(-1);
    }

	strcpy(ident,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;

    int check = search_table(ident,level,symbol_table,st_ptr);
    if(check!=-1 && symbol_table[check].level==level)
    {
        printf("Error : competing symbol declarations at the same level\n");
        exit(-1);
    }

    //enter into symbol table
    (*m)++;
    symbol_table[(*st_ptr)].kind = 2;
    strcpy(symbol_table[(*st_ptr)].name,ident);
    symbol_table[(*st_ptr)].val = 0;
    symbol_table[(*st_ptr)].level = level; 
    symbol_table[(*st_ptr)].addr = (*m); 
    symbol_table[(*st_ptr)].param = 0;
    (*st_ptr)++;

    //print_st(symbol_table,st_ptr);

    strcpy(token,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    while(atoi(token)==commasym)
    {
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        if(atoi(token)!=identsym)
        {
            printf("Error : const, var, procedure, call, and read keywords must be followed by identifier\n");
            exit(-1);
        }

		strcpy(ident,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;

        int check = search_table(ident,level,symbol_table,st_ptr);
        if(check!=-1 && symbol_table[check].level==level)
        {
            printf("Error : competing symbol declarations at the same level\n");
            exit(-1);
        }

        //enter into symbol table
        (*m)++;
        symbol_table[(*st_ptr)].kind = 2;
        strcpy(symbol_table[(*st_ptr)].name,ident);
        symbol_table[(*st_ptr)].val = 0;
        symbol_table[(*st_ptr)].level = level;
        symbol_table[(*st_ptr)].addr = (*m); 
        symbol_table[(*st_ptr)].param = 0;
        (*st_ptr)++;
        //print_st(symbol_table,st_ptr);

		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
    }
    
    if(atoi(token)!=semicolonsym)
    {
        printf("Error : symbol declarations must be followed by a semicolon\n");
        exit(-1);
    }
    strcpy(token,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    
}

void constdecl(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count)
{   
    char ident[12]; //names cannot exceed 11 chars
    char num[6]; //nums cannot exceed 5 digits

    strcpy(token,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    if(atoi(token)!=identsym)
    {
        printf("Error : const, var, procedure, call, and read keywords must be followed by identifier\n");
        exit(-1);
    }
	strcpy(ident,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    
    int check = search_table(ident,level,symbol_table,st_ptr);
    if(check!=-1 && symbol_table[check].level==level)
    {
        printf("Error : competing symbol declarations at the same level\n");
        exit(-1);
    }

    strcpy(token,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    if(atoi(token)!=eqsym)
    {
        printf("Error : constants must be assigned with =\n");
        exit(-1);
    }
    strcpy(token,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    if(atoi(token)!=numbersym)
    {
        printf("Error : constants must be assigned an integer value\n");
        exit(-1);
    }
	strcpy(num,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;

	strcpy(token,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    
    //enter into symbol table
    symbol_table[(*st_ptr)].kind = 1;
    strcpy(symbol_table[(*st_ptr)].name,ident);
    symbol_table[(*st_ptr)].val = atoi(num);
    symbol_table[(*st_ptr)].level = 0; 
    symbol_table[(*st_ptr)].addr = 0; 
    symbol_table[(*st_ptr)].param = 0;
    (*st_ptr)++;
    //print_st(symbol_table,st_ptr);

    while(atoi(token)==commasym)
    {
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        if(atoi(token)!=identsym)
        {
            printf("Error : const, var, procedure, call, and read keywords must be followed by identifier\n");
            exit(-1);
        }
		strcpy(ident,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        
        int check = search_table(ident,level,symbol_table,st_ptr);
        if(check!=-1 && symbol_table[check].level==level)
        {
            printf("Error : competing symbol declarations at the same level\n");
            exit(-1);
        }

        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        if(atoi(token)!=eqsym)
        {
            printf("Error : constants must be assigned with =\n");
            exit(-1);
        }
        strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;
        if(atoi(token)!=numbersym)
        {
            printf("Error : constants must be assigned an integer value\n");
            exit(-1);
        }
		strcpy(num,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;

		strcpy(token,token_list[(*tl_ptr)].token); //get token
		(*tl_ptr)++;

        //enter into symbol table
        symbol_table[(*st_ptr)].kind = 1;
        strcpy(symbol_table[(*st_ptr)].name,ident);
        symbol_table[(*st_ptr)].val = atoi(num);
        symbol_table[(*st_ptr)].level = 0; 
        symbol_table[(*st_ptr)].addr = 0; 
        symbol_table[(*st_ptr)].param = 0;
        (*st_ptr)++;
        //print_st(symbol_table,st_ptr);
    }
    
    if(atoi(token)!=semicolonsym)
    {
        printf("Error : symbol declarations must be followed by a semicolon\n");
        exit(-1);
    }
	strcpy(token,token_list[(*tl_ptr)].token); //get token
	(*tl_ptr)++;
    
}

void block(tok* token_list, int* tl_ptr, char token[], instruction* IR, int* cci, symbol* symbol_table, int* st_ptr, int level, int* procedure_count, int proci)
{
    
    //using int to create a new m every block call is important. it guarantees uniqueness of m
    int m = 3+symbol_table[proci].param; //m is the AR size. init size is 4, so m = 3 for 0 index + the parameters of the procedure
    int l;

    if(atoi(token)==constsym)
    {
        constdecl(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
    }
    if(atoi(token)==varsym)
    {
        vardecl(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count,&m);
    }
    if(atoi(token)==procsym)
    {
        procdecl(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count);
    }

    (*cci)++;
    IR[(*cci)].opcode = 6;
    strcpy(IR[(*cci)].op,"INC");
    IR[(*cci)].l = 0;
    IR[(*cci)].m = m+1;

    //after creating the inc command, update the procedure st addr to it
    symbol_table[proci].addr = (*cci);

    statement(token_list,tl_ptr,token,IR,cci,symbol_table,st_ptr,level,procedure_count); 
    
}

instruction *parse(tok* token_list, int print)
{
    int tl_ptr = 0;
    char token[10] = {'\0'};
    struct instruction IR[1001];
    int cci = -1; //current IR index
    struct symbol symbol_table[MAX_SYMBOL_TABLE_SIZE]; //malloc(MAX_SYMBOL_TABLE_SIZE*sizeof(symbol));
    int st_ptr = 0;
    int procedure_count = 1; //for each procedure, there is a corresponding jmp call. the count tells CAL where to go

    /*before parsing, start with a jmp command for main. then look through the token list and initialize 
    jmp commands for each procedure. give jmp dummy val of its IR location and change later*/
    cci++;
    IR[cci].opcode = 7;
    strcpy(IR[cci].op,"JMP");
    IR[cci].l = 0;
    IR[cci].m = cci;

    //now add to symbol table
    symbol_table[st_ptr].kind = 3;
    strcpy(symbol_table[st_ptr].name,"main");
    symbol_table[st_ptr].val = 0;
    symbol_table[st_ptr].level = 0;
    symbol_table[st_ptr].addr = 0; //this will change according to begin-end statement
    symbol_table[st_ptr].param = 0;
    st_ptr++;

    int a = 0;
    
    while(strcmp(token_list[a].token,"STOP")!=0)
    {
        if (strcmp(token_list[a].token,"30")==0)
        {
            //give jmp dummy val of its IR location
            //after statement call, update jmp to real cci
            cci++;
            IR[cci].opcode = 7;
            strcpy(IR[cci].op,"JMP");
            IR[cci].l = 0;
            IR[cci].m = cci;
        }

        a++;   
    }

    //begin parsing in gramatical order. initial level is 0
    strcpy(token,token_list[tl_ptr].token); //get token
    tl_ptr++;
    block(token_list,&tl_ptr,token,IR,&cci,symbol_table,&st_ptr,0,&procedure_count, 0); //proci of main is 0

    //now switch the dummy vals to the intended val
    int j = 0;
    int k = 0;

    for(j = 0; j<st_ptr;j++)
    {
        if (symbol_table[j].kind==3)
        {
            IR[k].m = symbol_table[j].addr;
            k++;
        }
    }

    //switch dummy val on "CAL" (test later)
    for(int line = 0;line<=cci;line++)
    {
        if(IR[line].opcode==5) //5 is CAL
        {
            //CAL gets the location the desired jump index is jumping to
            int jumpi = IR[line].m-1;
            IR[line].m = IR[jumpi].m; 
        }
    }

    if(atoi(token)!=periodsym)
    {
        //if all goes well, statement should've already gotten the periodsym
        printf("Error : program must end with period\n");
        exit(-1);
    }
    cci++;
    IR[cci].opcode = 9;
    strcpy(IR[cci].op,"SYS");
    IR[cci].l = 0;
    IR[cci].m = 3;
    
    if(print==1)
    {
        printf("Generated Assembly:\n");
        printf("Line\tOP\tL\tM\n");
        for(int i = 0; i<=cci;i++)
        {
            printf("%d\t%s\t%d\t%d\n",i,(IR+i)->op,(IR+i)->l,(IR+i)->m);
        }
        printf("\n\n");
    }

    //return pointer
    instruction* copy = IR;

    //free the token list since we dont need it anymore
    free(token_list);

    return copy;
}
