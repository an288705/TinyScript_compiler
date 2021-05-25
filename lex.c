#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

void int_to_str(int n, char s[])
{
    int i = 0;
    char buffer[3]; //buffer for the buffer

    /*read int in reverse order*/
    do 
    {      
        buffer[i] = n % 10 + '0';  
        i++;
    } while ((n /= 10) > 0);
    buffer[i] = '\0';
    
    int a = 0;

    /*now reverse the buffer*/
    for(a;a<i;a++)
    {
        s[a] = buffer[i-1-a];
    }

    s[a] = '\0';
} 

tok* lex_analyze(char *inputfile, int print)
{

    enum token_type token; 
    char c;
    char token_list[1000] = {'\0'}; //saves the token list. initialize to '\0'
    char buffer[3]; //buffer for int_to_str(). size = 2+'\0' = 3
    char lexeme[12]; //saves the lexeme
    int lexptr = 0; //pointer for the lexeme
    int offset = 0; //flag to stop program from getting next word
    int err_one = 0; //flag for error 1
    int err_three = 0; //flag for error 3
    int err_four = 0; //flag for error 4
    int ipfptr = 0;

    if(print==1)
    {
    printf("Lexeme Table:\n");
    printf("\tlexeme\ttoken type\n");
    }

    while (inputfile[ipfptr]!='\0')
    {
        if(offset==0)
        {
            c = inputfile[ipfptr++];;
        }

        /*clear all flags and buffers*/
        offset = 0;
        err_one = 0;
        err_three = 0;
        err_four = 0;
        lexptr = 0;
        strcpy(lexeme,"\0");
        strcpy(buffer,"\0");
        
        if(ispunct(c)!=0)
        {
            if(c=='%')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = modsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");}
            else if(c=='+')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = plussym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c=='-')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = minussym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c=='*')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = multsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c=='=')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = eqsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c=='(')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = lparentsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c==')')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = rparentsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c==',')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = commasym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c==';')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = semicolonsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c=='.')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                token = periodsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if(c=='/')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string

                /*get next char*/
                c = inputfile[ipfptr++];;

                if(c=='*')
                {
                    c = inputfile[ipfptr++];;

                    /*iterate through ifp until * and / is found*/
                    while(inputfile[ipfptr]!='\0')
                    {
                        if(c=='*')
                        {
                            c = inputfile[ipfptr++];;
                            if(c=='/')
                            {
                                break;
                            }
                        }

                        c = inputfile[ipfptr++];;
                    }
                    
                }
                else
                {
                    /*turn on offset since we don't know what next char is*/
                    token = slashsym;
                    offset = 1;
                    if(print==1)
                {
                    printf("\t%s\t%d\n",lexeme,token);
                }
                    /*now save to token list*/
                    int_to_str(token,buffer);
                    strcat(token_list,buffer);
                    strcat(token_list," ");
                }
    
            }
            else if(c=='<')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string

                /*get next char*/
                c = inputfile[ipfptr++];;
                lexptr++;

                if(c=='>')
                {
                    lexeme[lexptr] = c;
                    lexeme[lexptr+1] = '\0'; //end string
                    token = neqsym;
                    if(print==1)
                {
                    printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
                }
                else if (c=='=')
                {
                    lexeme[lexptr] = c;
                    lexeme[lexptr+1] = '\0'; //end string
                    token = leqsym;
                    if(print==1)
                {
                    printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
                }
                else
                {
                    /*turn on offset since we don't know what next char is*/
                    token = lessym;
                    offset = 1;
                    if(print==1)
                {
                    printf("\t%s\t%d\n",lexeme,token);
                }
                    /*now save to token list*/
                    int_to_str(token,buffer);
                    strcat(token_list,buffer);
                    strcat(token_list," ");
                }  
            }
            else if(c=='>')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                /*get next char*/
                c = inputfile[ipfptr++];;
                lexptr++;

                if(c=='=')
                {
                    lexeme[lexptr] = c;
                    lexeme[lexptr+1] = '\0'; //end string
                    token = geqsym;
                    if(print==1)
                {
                    printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
                }
                else
                {
                    /*turn on offset since we don't know what next char is*/
                    token = gtrsym;
                    offset = 1;
                    if(print==1)
                {
                    printf("\t%s\t%d\n",lexeme,token);
                }
                    /*now save to token list*/
                    int_to_str(token,buffer);
                    strcat(token_list,buffer);
                    strcat(token_list," ");
                }  
            }
            else if(c==':')
            {
                lexeme[lexptr] = c;
                lexeme[lexptr+1] = '\0'; //end string
                /*get next char*/
                c = inputfile[ipfptr++];;
                lexptr++;

                if(c=='=')
                {
                    lexeme[lexptr] = c;
                    lexeme[lexptr+1] = '\0'; //end string
                    token = becomessym;
                    if(print==1)
                {
                    printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
                }
                else
                {
                    printf("Error : Invalid Symbol\n");
                    exit(-1);
                }  
            }
            else
            {
                printf("Error : Invalid Symbol\n");
                exit(-1);
            }
        }
        else if(isalpha(c)!=0)
        {
            lexeme[lexptr] = c;
            lexeme[lexptr+1] = '\0'; //end string

            while (isalnum(c)!=0)
            {
                /*read in new string and append to lexeme until limit exceeded. ONLY READ
                ALPHANUMERIC CHARACTERS INTO LEXEME*/
                c = inputfile[ipfptr++];;
                lexptr++;

                if (lexptr>=11)
                {
                    err_one = 1;
                }         

                if(lexptr<11 && isalnum(c)!=0)
                {
                    lexeme[lexptr] = c;
                    lexeme[lexptr+1] = '\0'; //end string
                }
            }

            /*turn on offset since the while loop stops at a non-alphanumeric char*/
            offset = 1; 

            if(err_one==1)
            {
                printf("Error : Identifier names cannot exceed 11 characters\n");
                exit(-1);
            }
            else if (strcmp(lexeme,"odd")==0)
            {
                token = oddsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"begin")==0)
            {
                token = beginsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"end")==0)
            {
                token = endsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"if")==0)
            {
                token = ifsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"then")==0)
            {
                token = thensym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"while")==0)
            {
                token = whilesym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"do")==0)
            {
                token = dosym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"call")==0)
            {
                token = callsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"const")==0)
            {
                token = constsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"var")==0)
            {
                token = varsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");}
            else if (strcmp(lexeme,"procedure")==0)
            {
                token = procsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"write")==0)
            {
                token = writesym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"read")==0)
            {
                token = readsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"else")==0)
            {
                token = elsesym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else if (strcmp(lexeme,"return")==0)
            {
                token = returnsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
            }
            else
            {
                token = identsym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list. for identsym, save token and identifier*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
                strcat(token_list,lexeme);
                strcat(token_list," ");
            }
        }
        else if(isdigit(c)!=0)
        {
            lexeme[lexptr] = c;
            lexeme[lexptr+1] = '\0'; //end string

            while (isalnum(c)!=0)
            {
                /*read in new string and append to lexeme until limit exceeded. ONLY READ
                ALPHANUMERIC CHARACTERS INTO LEXEME. If error 5 is reached, turn on flag*/
                c = inputfile[ipfptr++];;
                lexptr++;

                /*if all the characters have been digits and there are more than 5 digits 
                (zero indexed), then turn on error 3 flag*/
                if(err_four!=1 && lexptr>=5)
                {
                    err_three = 1;
                }

                /*if character is in alphabet, turn on error 4 flag*/
                if(isalpha(c)!=0)
                {
                    err_four = 1;
                }

                if(lexptr<5 && isalnum(c)!=0)
                {
                    lexeme[lexptr] = c;
                    lexeme[lexptr+1] = '\0'; //end string
                }
            }

            /*turn on offset since the while loop stops at a non-alphanumeric char*/
            offset = 1;

            if(err_three==1)
            {
                printf("Error : Numbers cannot exceed 5 digits\n");
                exit(-1);
            }
            else if (err_four==1)
            {
                printf("Error : Identifiers cannot begin with a digit\n");
                exit(-1);
            }
            else
            {
                token = numbersym;
                if(print==1)
                {
                printf("\t%s\t%d\n",lexeme,token);
                }
                /*now save to token list. for numbersym, save token and number*/
                int_to_str(token,buffer);
                strcat(token_list,buffer);
                strcat(token_list," ");
                strcat(token_list,lexeme);
                strcat(token_list," ");
            }
        }
    }

    if(print==1)
    {
    printf("\n");
    printf("Token List:\n");
    printf("%s\n\n\n",token_list);
    }
    //return pointer
    int tlptr = 0;
    tok* list = malloc(5001 * sizeof(tok));
    int i = 0;

    while(token_list[tlptr]!='\0')
    {
        int j = 0;
        char s[12] = {'\0'};

        while(isspace(token_list[tlptr])==0)
        {
            s[j] = token_list[tlptr];
            tlptr++;
            j++;
        }

        list[i].token = strdup(s);
        i++;
        tlptr++;
    }

    list[i].token = strdup("STOP"); //flag for stoppin in parser.c

    return list;

}