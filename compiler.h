enum token_type{modsym = 1, identsym, numbersym, plussym, minussym,multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym,readsym , elsesym, returnsym}; 

typedef struct tok
{
	char *token;
} tok;

typedef struct instruction
{
	int opcode;
	char op[4];
	int l;
	int m;
	
} instruction;

typedef struct symbol
{
	int kind;
	char name[12];
	int val;
	int level;
	int addr;
	int param;
} symbol;


tok* lex_analyze(char *inputfile, int print);
void execute(instruction *code, int print);
instruction *parse(tok* token_list, int print);