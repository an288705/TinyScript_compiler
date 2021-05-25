#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "compiler.h"

int base(int stack[], int level, int BP)
{
    int base = BP;
    int counter = level;

    while (counter > 0)
    {
        base = stack[base];
        counter--;
    }

    return base;
}

void execute(instruction *IR, int print)
{
    //initialize components
    int PC = 0;
    int BP = 0;
    int SP = -1;
    int halt = 1;
    int stack[50] = {0};
    int markers[50] = {0};
    int max_level = 0;

    //SWITCH STATEMENT FOR OP CODE

    /*Opcode instruction
    1   lit 0 m
    2 	opr 0 m
    3 	lod l m
    4 	sto l m
    5 	cal l m
    6 	inc 0 m
    7 	jmp 0 m
    8 	jpc 0 m
    9 	sys 0 m*/
    if(print==1)
    {
    printf("\t\tPC\tBP\tSP\tstack\nInitial values: %d\t%d\t%d\n", PC, BP, SP);
    }
    while (halt == 1)
    { //while loop until program halts
        switch (IR[PC].opcode)
        {
        case 1: //opcode 1: lit 0 m
            if(print==1)
            {
            printf("%d ",PC);
            printf("LIT\t%d ",IR[PC].l);
            printf("%d",IR[PC].m);
            SP++;
            }

            stack[SP] = IR[PC].m; //push literal to top of stack
            PC++; //increment PC
            break;
        case 2: //opcode 2: opr 0 m. 
        //This opcode has multiple operations. What the operation does is written in the print statement
            if (IR[PC].m == 0)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }
                PC++; //increment PC

                stack[BP-1] = stack[SP];
                SP = BP - 1;
                markers[SP+1] = 0;
                BP = stack[SP+2]; //BP is now equal to the dynamic link
                PC = stack[SP+3]; //PC is now equal to the return address
                max_level--; //return to one level lower
            }
            else if (IR[PC].m == 1)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                stack[SP] = stack[SP] * -1;
            }
            else if (IR[PC].m == 2)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                SP--;
                stack[SP] = stack[SP] + stack[SP + 1];
            }
            else if (IR[PC].m == 3)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }
                PC++; //increment PC
                SP--;
                stack[SP] = stack[SP] - stack[SP + 1];
            }
            else if (IR[PC].m == 4)
            {
                if(print==1)
            {
                printf("%d ",PC);  
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }
                PC++; //increment PC
                SP--;
                stack[SP] = stack[SP] * stack[SP + 1];
            }
            else if (IR[PC].m == 5)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                SP--;
                stack[SP] = stack[SP] / stack[SP + 1];
            }
            else if (IR[PC].m == 6)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                stack[SP] = stack[SP] % 2;
            }
            else if (IR[PC].m == 7)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                SP--;
                stack[SP] = stack[SP] % stack[SP + 1];
            }
            else if (IR[PC].m == 8)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                SP--;

                if (stack[SP]==stack[SP+1])
                {
                    stack[SP] = 1;
                }
                else
                {
                    stack[SP] = 0;
                }
            }
            else if (IR[PC].m == 9)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                SP--;

                if (stack[SP]!=stack[SP+1])
                {
                    stack[SP] = 1;
                }
                else
                {
                    stack[SP] = 0;
                }
            }
            else if (IR[PC].m == 10)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                SP--;

                if (stack[SP]<stack[SP+1])
                {
                    stack[SP] = 1;
                }
                else
                {
                    stack[SP] = 0;
                }
            }
            else if (IR[PC].m == 11)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                SP--;

                if (stack[SP]<=stack[SP+1])
                {
                    stack[SP] = 1;
                }
                else
                {
                    stack[SP] = 0;
                }
            }
            else if (IR[PC].m == 12)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   

                PC++; //increment PC
                SP--;

                if (stack[SP]>stack[SP+1])
                {
                    stack[SP] = 1;
                }
                else
                {
                    stack[SP] = 0;
                }
            }
            else if (IR[PC].m == 13)
            {
                if(print==1)
            {
                printf("%d ",PC);
                printf("OPR\t%d ",IR[PC].l);
                printf("%d",IR[PC].m);
            }   
                PC++; //increment PC
                SP--;

                if (stack[SP]>=stack[SP+1])
                {
                    stack[SP] = 1;
                }
                else
                {
                    stack[SP] = 0;
                }
            }
            break;
        case 3: //opcode 3: lod l 
            if(print==1)
            {
            printf("%d ",PC);
            printf("LOD\t%d ",IR[PC].l);
            printf("%d",IR[PC].m);
            }
            SP++;

            /*use the base function to get the index of the level you want, then offset by m*/
            stack[SP] = stack[base(stack, IR[PC].l, BP) + IR[PC].m];
            PC++; //increment PC
            break;
        case 4: //opcode 4: sto l m
            if(print==1)
            {
            printf("%d ",PC);
            printf("STO\t%d ",IR[PC].l);
            printf("%d",IR[PC].m);
            }
            /*use the base function to get the index of the level you want, then offset by m*/
            stack[base(stack, IR[PC].l, BP) + IR[PC].m] = stack[SP];

            PC++; //increment PC
            SP--;
            break;
        case 5: //opcode 5: cal l 
            if(print==1)
            {
            printf("%d ",PC);
            printf("CAL\t%d ",IR[PC].l);
            printf("%d",IR[PC].m);
            }
            stack[SP + 1] = base(stack, IR[PC].l, BP); //this val is the index of static link
            markers[SP+1] = 1;
            stack[SP + 2] = BP;                    //this val is the index of dynamic link
            stack[SP + 3] = PC + 1;                //this val is the return address
            stack[SP + 4] = stack[SP];             //this val is the parameter

            PC = IR[PC].m; //increment PC
            BP = SP+1;
            max_level++;
            break;
        case 6: //opcode 6: inc 0 
            if(print==1)
            {
            printf("%d ",PC);
            printf("INC\t%d ",IR[PC].l);
            printf("%d",IR[PC].m);
            }
                
            SP = SP + IR[PC].m; //increment the stack by IR[PC].m indexes

            PC++;
            break;
        case 7: //opcode 7: jmp 0 m
            if(print==1)
            {
            printf("%d ",PC);
            printf("JMP\t%d ",IR[PC].l);
            printf("%d",IR[PC].m);
            }
            PC = IR[PC].m; //jump to IR[PC].m
            break;
        case 8: //opcode 8: jpc 0 m
            if(print==1)
            {
            printf("%d ",PC);
            printf("JPC\t%d ",IR[PC].l);
            printf("%d",IR[PC].m);
            }
            //only jump if the top of stack is 0
            if (stack[SP] == 0)
            {
                PC = IR[PC].m;
            }
            else
            {
                PC++;
            }

            SP--;
            break;
        case 9: //opcode 9: sys 0 m
            if (IR[PC].m == 1)
            {
                //pop the stack
                if(print==1)
                {
                printf("Top of Stack Value: %d\n", stack[SP]);
                }
                SP--;
            }
            else if (IR[PC].m == 2)
            {
                //enter an integer to top of stack
                SP++;
                {
                printf("Please Enter an Integer:");
                }
                scanf("%d", &stack[SP]);
            }
            else if (IR[PC].m == 3)
            {
                //halt the program
                halt = 0;
            }
            if(print==1)
            {
            printf("%d ",PC);
            printf("SYS\t%d ",IR[PC].l);
            printf("%d",IR[PC].m);
            }
            
            PC++; //increment PC
            break;
        default:
            printf("Error!  incorrect op code");
            exit(-1);
            break;
        }
        
        /*before reiterating while loop, print the status of the registers*/
        if(print==1)
        {
            printf("\t%d",PC);
            printf("\t%d",BP);
            printf("\t%d\t",SP);
            

            for(int i=0;i<=SP;i++)
            {
                if(markers[i]==1)
                {
                    printf("| %d ",stack[i]);
                }
                else
                {
                    printf("%d ",stack[i]);
                }
            }

            printf("\n");
        }
    }

}
