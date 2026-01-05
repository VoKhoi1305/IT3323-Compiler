/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

// Reader

#define IO_ERROR 0
#define IO_SUCCESS 1

int lineNo, colNo;
int currentChar;

int readChar(void) {
  currentChar = getc(stdin);
  colNo ++;
  if (currentChar == '\n') {
    lineNo ++;
    colNo = 0;
  }
  return currentChar;
}

// Instructions

#define TRUE 1
#define FALSE 0
#define DC_VALUE 0

typedef int WORD;

enum OpCode {
  OP_LA,   // Load Address:    t := t + 1; s[t] := base(p) + q;
  OP_LV,   // Load Value:      t := t + 1; s[t] := s[base(p) + q];
  OP_LC,   // load Constant    t := t + 1; s[t] := q;
  OP_LI,   // Load Indirect    s[t] := s[s[t]];
  OP_INT,  // Increment t      t := t + q;
  OP_DCT,  // Decrement t      t := t - q;
  OP_J,    // Jump             pc := q;
  OP_FJ,   // False Jump       if s[t] = 0 then pc := q; t := t - 1;
  OP_HL,   // Halt             Halt
  OP_ST,   // Store            s[s[t-1]] := s[t]; t := t -2;
  OP_CALL, // Call             s[t+2] := b; s[t+3] := pc; s[t+4]:= base(p); b:=t+1; pc:=q;
  OP_EP,   // Exit Procedure   t := b - 1;  pc := s[b+2];  b := s[b+1];
  OP_EF,   // Exit Function    t := b;  pc := s[b+2];  b := s[b+1];
  OP_RC,   // Read Char        read one character into s[s[t]];  t := t - 1;
  OP_RI,   // Read Integer     read integer to s[s[t]];  t := t-1;
  OP_WRC,  // Write Char       write one character from s[t];  t := t-1;
  OP_WRI,  // Write Int        write integer from s[t];  t := t-1;
  OP_WLN,  // WriteLN          CR/LF
  OP_AD,   // Add              t := t-1;  s[t] := s[t] + s[t+1];
  OP_SB,   // Substract        t := t-1;  s[t] := s[t] - s[t+1];
  OP_ML,   // Multiple         t := t-1;  s[t] := s[t] * s[t+1];
  OP_DV,   // Divide           t := t-1;  s[t] := s[t] / s[t+1];
  OP_NEG,  // Negative         s[t] := - s[t];
  OP_CV,   // Copy Top         s[t+1] := s[t]; t := t + 1;
  OP_EQ,   // Equal            t := t - 1;  if s[t] = s[t+1] then s[t] := 1 else s[t] := 0;
  OP_NE,   // Not Equal        t := t - 1;  if s[t] != s[t+1] then s[t] := 1 else s[t] := 0;
  OP_GT,   // Greater          t := t - 1;  if s[t] > s[t+1] then s[t] := 1 else s[t] := 0;
  OP_LT,   // Less             t := t - 1;  if s[t] < s[t+1] then s[t] := 1 else s[t] := 0;
  OP_GE,   // Greater or Equal t := t - 1;  if s[t] >= s[t+1] then s[t] := 1 else s[t] := 0;
  OP_LE,   // Less or Equal    t := t - 1;  if s[t] >= s[t+1] then s[t] := 1 else s[t] := 0;

  OP_BP    // Break point. Just for debugging
};

struct Instruction_ {
  enum OpCode op;
  WORD p;
  WORD q;
};

typedef struct Instruction_ Instruction;
typedef int CodeAddress;

struct CodeBlock_ {
  Instruction* code;
  int codeSize;
  int maxSize;
};

typedef struct CodeBlock_ CodeBlock;


#define MAX_BLOCK 50

CodeBlock* createCodeBlock(int maxSize) {
  CodeBlock* codeBlock = (CodeBlock*) malloc(sizeof(CodeBlock));

  codeBlock->code = (Instruction*) malloc(maxSize * sizeof(Instruction));
  codeBlock->codeSize = 0;
  codeBlock->maxSize = maxSize;
  return codeBlock;
}

void freeCodeBlock(CodeBlock* codeBlock) {
  free(codeBlock->code);
  free(codeBlock);
}

int emitCode(CodeBlock* codeBlock, enum OpCode op, WORD p, WORD q) {
  Instruction* bottom = codeBlock->code + codeBlock->codeSize;

  if (codeBlock->codeSize >= codeBlock->maxSize) return 0;

  bottom->op = op;
  bottom->p = p;
  bottom->q = q;
  codeBlock->codeSize ++;
  return 1;
}

int emitLA(CodeBlock* codeBlock, WORD p, WORD q) { return emitCode(codeBlock, OP_LA, p, q); }
int emitLV(CodeBlock* codeBlock, WORD p, WORD q) { return emitCode(codeBlock, OP_LV, p, q); }
int emitLC(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_LC, DC_VALUE, q); }
int emitLI(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_LI, DC_VALUE, DC_VALUE); }
int emitINT(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_INT, DC_VALUE, q); }
int emitDCT(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_DCT, DC_VALUE, q); }
int emitJ(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_J, DC_VALUE, q); }
int emitFJ(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_FJ, DC_VALUE, q); }
int emitHL(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_HL, DC_VALUE, DC_VALUE); }
int emitST(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_ST, DC_VALUE, DC_VALUE); }
int emitCALL(CodeBlock* codeBlock, WORD p, WORD q) { return emitCode(codeBlock, OP_CALL, p, q); }
int emitEP(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_EP, DC_VALUE, DC_VALUE); }
int emitEF(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_EF, DC_VALUE, DC_VALUE); }
int emitRC(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_RC, DC_VALUE, DC_VALUE); }
int emitRI(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_RI, DC_VALUE, DC_VALUE); }
int emitWRC(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_WRC, DC_VALUE, DC_VALUE); }
int emitWRI(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_WRI, DC_VALUE, DC_VALUE); }
int emitWLN(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_WLN, DC_VALUE, DC_VALUE); }
int emitAD(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_AD, DC_VALUE, DC_VALUE); }
int emitSB(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_SB, DC_VALUE, DC_VALUE); }
int emitML(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_ML, DC_VALUE, DC_VALUE); }
int emitDV(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_DV, DC_VALUE, DC_VALUE); }
int emitNEG(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_NEG, DC_VALUE, DC_VALUE); }
int emitCV(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_CV, DC_VALUE, DC_VALUE); }
int emitEQ(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_EQ, DC_VALUE, DC_VALUE); }
int emitNE(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_NE, DC_VALUE, DC_VALUE); }
int emitGT(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_GT, DC_VALUE, DC_VALUE); }
int emitLT(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_LT, DC_VALUE, DC_VALUE); }
int emitGE(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_GE, DC_VALUE, DC_VALUE); }
int emitLE(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_LE, DC_VALUE, DC_VALUE); }

int emitBP(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_BP, DC_VALUE, DC_VALUE); }


void printInstruction(Instruction* inst) {
  switch (inst->op) {
  case OP_LA: printf("LA %d,%d", inst->p, inst->q); break;
  case OP_LV: printf("LV %d,%d", inst->p, inst->q); break;
  case OP_LC: printf("LC %d", inst->q); break;
  case OP_LI: printf("LI"); break;
  case OP_INT: printf("INT %d", inst->q); break;
  case OP_DCT: printf("DCT %d", inst->q); break;
  case OP_J: printf("J %d", inst->q); break;
  case OP_FJ: printf("FJ %d", inst->q); break;
  case OP_HL: printf("HL"); break;
  case OP_ST: printf("ST"); break;
  case OP_CALL: printf("CALL %d,%d", inst->p, inst->q); break;
  case OP_EP: printf("EP"); break;
  case OP_EF: printf("EF"); break;
  case OP_RC: printf("RC"); break;
  case OP_RI: printf("RI"); break;
  case OP_WRC: printf("WRC"); break;
  case OP_WRI: printf("WRI"); break;
  case OP_WLN: printf("WLN"); break;
  case OP_AD: printf("AD"); break;
  case OP_SB: printf("SB"); break;
  case OP_ML: printf("ML"); break;
  case OP_DV: printf("DV"); break;
  case OP_NEG: printf("NEG"); break;
  case OP_CV: printf("CV"); break;
  case OP_EQ: printf("EQ"); break;
  case OP_NE: printf("NE"); break;
  case OP_GT: printf("GT"); break;
  case OP_LT: printf("LT"); break;
  case OP_GE: printf("GE"); break;
  case OP_LE: printf("LE"); break;

  case OP_BP: printf("BP"); break;
  default: break;
  }
}

void printCodeBlock(CodeBlock* codeBlock) {
  Instruction* pc = codeBlock->code;
  int i;
  for (i = 0 ; i < codeBlock->codeSize; i ++) {
    printf("%d:  ",i);
    printInstruction(pc);
    printf("\n");
    pc ++;
  }
}


void loadCode(CodeBlock* codeBlock, FILE* f) {
  Instruction* code = codeBlock->code;
  int n;

  codeBlock->codeSize = 0;
  while (!feof(f)) {
    n = fread(code, sizeof(Instruction), MAX_BLOCK, f);
    code += n;
    codeBlock->codeSize += n;
  }
}


void saveCode(CodeBlock* codeBlock, FILE* f) {
  fwrite(codeBlock->code, sizeof(Instruction), codeBlock->codeSize, f);
}


// Charcode
typedef enum {
  CHAR_SPACE,
  CHAR_LETTER,
  CHAR_DIGIT,
  CHAR_PLUS,
  CHAR_MINUS,
  CHAR_TIMES,
  CHAR_SLASH,
  CHAR_LT,
  CHAR_GT,
  CHAR_EXCLAIMATION,
  CHAR_EQ,
  CHAR_COMMA,
  CHAR_PERIOD,
  CHAR_COLON,
  CHAR_SEMICOLON,
  CHAR_SINGLEQUOTE,
  CHAR_LPAR,
  CHAR_RPAR,
  CHAR_UNKNOWN
} CharCode;

CharCode charCodes[256] = {
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_SPACE, CHAR_EXCLAIMATION, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_SINGLEQUOTE,
  CHAR_LPAR, CHAR_RPAR, CHAR_TIMES, CHAR_PLUS, CHAR_COMMA, CHAR_MINUS, CHAR_PERIOD, CHAR_SLASH,
  CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT,
  CHAR_DIGIT, CHAR_DIGIT, CHAR_COLON, CHAR_SEMICOLON, CHAR_LT, CHAR_EQ, CHAR_GT, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN
};


// Token

#define MAX_IDENT_LEN 15
#define KEYWORDS_COUNT 20

typedef enum {
  TK_NONE, TK_IDENT, TK_NUMBER, TK_CHAR, TK_EOF,

  KW_PROGRAM, KW_CONST, KW_TYPE, KW_VAR,
  KW_INTEGER, KW_CHAR, KW_ARRAY, KW_OF, 
  KW_FUNCTION, KW_PROCEDURE,
  KW_BEGIN, KW_END, KW_CALL,
  KW_IF, KW_THEN, KW_ELSE,
  KW_WHILE, KW_DO, KW_FOR, KW_TO,

  SB_SEMICOLON, SB_COLON, SB_PERIOD, SB_COMMA,
  SB_ASSIGN, SB_EQ, SB_NEQ, SB_LT, SB_LE, SB_GT, SB_GE,
  SB_PLUS, SB_MINUS, SB_TIMES, SB_SLASH,
  SB_LPAR, SB_RPAR, SB_LSEL, SB_RSEL
} TokenType; 

typedef struct {
  char string[MAX_IDENT_LEN + 1];
  int lineNo, colNo;
  TokenType tokenType;
  union {
    int intValue;
    char charValue;
  };
} Token;

struct {
  char string[MAX_IDENT_LEN + 1];
  TokenType tokenType;
} keywords[KEYWORDS_COUNT] = {
  {"PROGRAM", KW_PROGRAM},
  {"CONST", KW_CONST},
  {"TYPE", KW_TYPE},
  {"VAR", KW_VAR},
  {"INTEGER", KW_INTEGER},
  {"CHAR", KW_CHAR},
  {"ARRAY", KW_ARRAY},
  {"OF", KW_OF},
  {"FUNCTION", KW_FUNCTION},
  {"PROCEDURE", KW_PROCEDURE},
  {"BEGIN", KW_BEGIN},
  {"END", KW_END},
  {"CALL", KW_CALL},
  {"IF", KW_IF},
  {"THEN", KW_THEN},
  {"ELSE", KW_ELSE},
  {"WHILE", KW_WHILE},
  {"DO", KW_DO},
  {"FOR", KW_FOR},
  {"TO", KW_TO}
};

int keywordEq(char *kw, char *string) {
  while ((*kw != '\0') && (*string != '\0')) {
    if (*kw != toupper(*string)) break;
    kw ++; string ++;
  }
  return ((*kw == '\0') && (*string == '\0'));
}

TokenType checkKeyword(char *string) {
  int i;
  for (i = 0; i < KEYWORDS_COUNT; i++)
    if (keywordEq(keywords[i].string, string)) 
      return keywords[i].tokenType;
  return TK_NONE;
}

Token* makeToken(TokenType tokenType, int lineNo, int colNo) {
  Token *token = (Token*)malloc(sizeof(Token));
  token->tokenType = tokenType;
  token->lineNo = lineNo;
  token->colNo = colNo;
  return token;
}

char *tokenToString(TokenType tokenType) {
  switch (tokenType) {
  case TK_NONE: return "None";
  case TK_IDENT: return "an identification";
  case TK_NUMBER: return "a number";
  case TK_CHAR: return "a constant char";
  case TK_EOF: return "end of file";

  case KW_PROGRAM: return "keyword PROGRAM";
  case KW_CONST: return "keyword CONST";
  case KW_TYPE: return "keyword TYPE";
  case KW_VAR: return "keyword VAR";
  case KW_INTEGER: return "keyword INTEGER";
  case KW_CHAR: return "keyword CHAR";
  case KW_ARRAY: return "keyword ARRAY";
  case KW_OF: return "keyword OF";
  case KW_FUNCTION: return "keyword FUNCTION";
  case KW_PROCEDURE: return "keyword PROCEDURE";
  case KW_BEGIN: return "keyword BEGIN";
  case KW_END: return "keyword END";
  case KW_CALL: return "keyword CALL";
  case KW_IF: return "keyword IF";
  case KW_THEN: return "keyword THEN";
  case KW_ELSE: return "keyword ELSE";
  case KW_WHILE: return "keyword WHILE";
  case KW_DO: return "keyword DO";
  case KW_FOR: return "keyword FOR";
  case KW_TO: return "keyword TO";

  case SB_SEMICOLON: return "\';\'";
  case SB_COLON: return "\':\'";
  case SB_PERIOD: return "\'.\'";
  case SB_COMMA: return "\',\'";
  case SB_ASSIGN: return "\':=\'";
  case SB_EQ: return "\'=\'";
  case SB_NEQ: return "\'!=\'";
  case SB_LT: return "\'<\'";
  case SB_LE: return "\'<=\'";
  case SB_GT: return "\'>\'";
  case SB_GE: return "\'>=\'";
  case SB_PLUS: return "\'+\'";
  case SB_MINUS: return "\'-\'";
  case SB_TIMES: return "\'*\'";
  case SB_SLASH: return "\'/\'";
  case SB_LPAR: return "\'(\'";
  case SB_RPAR: return "\')\'";
  case SB_LSEL: return "\'(.\'";
  case SB_RSEL: return "\'.)\'";
  default: return "";
  }
}

// Error

typedef enum {
  ERR_END_OF_COMMENT,
  ERR_IDENT_TOO_LONG,
  ERR_NUMBER_TOO_LONG,
  ERR_INVALID_CONSTANT_CHAR,
  ERR_INVALID_SYMBOL,
  ERR_INVALID_IDENT,
  ERR_INVALID_CONSTANT,
  ERR_INVALID_TYPE,
  ERR_INVALID_BASICTYPE,
  ERR_INVALID_VARIABLE,
  ERR_INVALID_FUNCTION,
  ERR_INVALID_PROCEDURE,
  ERR_INVALID_PARAMETER,
  ERR_INVALID_STATEMENT,
  ERR_INVALID_COMPARATOR,
  ERR_INVALID_EXPRESSION,
  ERR_INVALID_TERM,
  ERR_INVALID_FACTOR,
  ERR_INVALID_LVALUE,
  ERR_INVALID_ARGUMENTS,
  ERR_UNDECLARED_IDENT,
  ERR_UNDECLARED_CONSTANT,
  ERR_UNDECLARED_INT_CONSTANT,
  ERR_UNDECLARED_TYPE,
  ERR_UNDECLARED_VARIABLE,
  ERR_UNDECLARED_FUNCTION,
  ERR_UNDECLARED_PROCEDURE,
  ERR_DUPLICATE_IDENT,
  ERR_TYPE_INCONSISTENCY,
  ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY
} ErrorCode;

#define NUM_OF_ERRORS 30

struct ErrorMessage {
  ErrorCode errorCode;
  char *message;
};

struct ErrorMessage errors[NUM_OF_ERRORS] = {
  {ERR_END_OF_COMMENT, "End of comment expected."},
  {ERR_IDENT_TOO_LONG, "Identifier too long."},
  {ERR_NUMBER_TOO_LONG, "Number too long."},
  {ERR_INVALID_CONSTANT_CHAR, "Invalid char constant."},
  {ERR_INVALID_SYMBOL, "Invalid symbol."},
  {ERR_INVALID_IDENT, "An identifier expected."},
  {ERR_INVALID_CONSTANT, "A constant expected."},
  {ERR_INVALID_TYPE, "A type expected."},
  {ERR_INVALID_BASICTYPE, "A basic type expected."},
  {ERR_INVALID_VARIABLE, "A variable expected."},
  {ERR_INVALID_FUNCTION, "A function identifier expected."},
  {ERR_INVALID_PROCEDURE, "A procedure identifier expected."},
  {ERR_INVALID_PARAMETER, "A parameter expected."},
  {ERR_INVALID_STATEMENT, "Invalid statement."},
  {ERR_INVALID_COMPARATOR, "A comparator expected."},
  {ERR_INVALID_EXPRESSION, "Invalid expression."},
  {ERR_INVALID_TERM, "Invalid term."},
  {ERR_INVALID_FACTOR, "Invalid factor."},
  {ERR_INVALID_LVALUE, "Invalid lvalue in assignment."},
  {ERR_INVALID_ARGUMENTS, "Wrong arguments."},
  {ERR_UNDECLARED_IDENT, "Undeclared identifier."},
  {ERR_UNDECLARED_CONSTANT, "Undeclared constant."},
  {ERR_UNDECLARED_INT_CONSTANT, "Undeclared integer constant."},
  {ERR_UNDECLARED_TYPE, "Undeclared type."},
  {ERR_UNDECLARED_VARIABLE, "Undeclared variable."},
  {ERR_UNDECLARED_FUNCTION, "Undeclared function."},
  {ERR_UNDECLARED_PROCEDURE, "Undeclared procedure."},
  {ERR_DUPLICATE_IDENT, "Duplicate identifier."},
  {ERR_TYPE_INCONSISTENCY, "Type inconsistency"},
  {ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, "The number of arguments and the number of parameters are inconsistent."}
};

void error(ErrorCode err, int lineNo, int colNo) {
  int i;
  for (i = 0 ; i < NUM_OF_ERRORS; i ++) 
    if (errors[i].errorCode == err) {
      printf("%d-%d:%s\n", lineNo, colNo, errors[i].message);
      exit(0);
    }
}

void missingToken(TokenType tokenType, int lineNo, int colNo) {
  printf("%d-%d:Missing %s\n", lineNo, colNo, tokenToString(tokenType));
  exit(0);
}

void assert(char *msg) {
  printf("%s\n", msg);
}

// symtab

#define RESERVED_WORDS 4
#define INT_SIZE 1
#define CHAR_SIZE 1

enum TypeClass {
  TP_INT,
  TP_CHAR,
  TP_ARRAY
};

enum ObjectKind {
  OBJ_CONSTANT,
  OBJ_VARIABLE,
  OBJ_TYPE,
  OBJ_FUNCTION,
  OBJ_PROCEDURE,
  OBJ_PARAMETER,
  OBJ_PROGRAM
};

enum ParamKind {
  PARAM_VALUE,
  PARAM_REFERENCE
};

struct Type_ {
  enum TypeClass typeClass;
  int arraySize;
  struct Type_ *elementType;
};

typedef struct Type_ Type;
typedef struct Type_ BasicType;


struct ConstantValue_ {
  enum TypeClass type;
  union {
    int intValue;
    char charValue;
  };
};

typedef struct ConstantValue_ ConstantValue;

struct Scope_;
struct ObjectNode_;
struct Object_;

struct ConstantAttributes_ {
  ConstantValue* value;
};

struct VariableAttributes_ {
  Type *type;
  struct Scope_ *scope;

  int localOffset;        // offset of the local variable calculated from the base of the stack frame
};

struct TypeAttributes_ {
  Type *actualType;
};

struct ProcedureAttributes_ {
  struct ObjectNode_ *paramList;
  struct Scope_* scope;

  int paramCount;
  CodeAddress codeAddress;
};

struct FunctionAttributes_ {
  struct ObjectNode_ *paramList;
  Type* returnType;
  struct Scope_ *scope;

  int paramCount;
  CodeAddress codeAddress;
};

struct ProgramAttributes_ {
  struct Scope_ *scope;
  CodeAddress codeAddress;
};

struct ParameterAttributes_ {
  enum ParamKind kind;
  Type* type;
  struct Scope_ *scope;

  int localOffset;
};

typedef struct ConstantAttributes_ ConstantAttributes;
typedef struct TypeAttributes_ TypeAttributes;
typedef struct VariableAttributes_ VariableAttributes;
typedef struct FunctionAttributes_ FunctionAttributes;
typedef struct ProcedureAttributes_ ProcedureAttributes;
typedef struct ProgramAttributes_ ProgramAttributes;
typedef struct ParameterAttributes_ ParameterAttributes;

struct Object_ {
  char name[MAX_IDENT_LEN];
  enum ObjectKind kind;
  union {
    ConstantAttributes* constAttrs;
    VariableAttributes* varAttrs;
    TypeAttributes* typeAttrs;
    FunctionAttributes* funcAttrs;
    ProcedureAttributes* procAttrs;
    ProgramAttributes* progAttrs;
    ParameterAttributes* paramAttrs;
  };
};

typedef struct Object_ Object;

struct ObjectNode_ {
  Object *object;
  struct ObjectNode_ *next;
};

typedef struct ObjectNode_ ObjectNode;

struct Scope_ {
  ObjectNode *objList;
  Object *owner;
  struct Scope_ *outer;
  int frameSize;
};

typedef struct Scope_ Scope;

struct SymTab_ {
  Object* program;
  Scope* currentScope;
  ObjectNode *globalObjectList;
};

typedef struct SymTab_ SymTab;

Scope* createScope(Object* owner);

Object* createProgramObject(char *programName);
Object* createConstantObject(char *name);
Object* createTypeObject(char *name);
Object* createVariableObject(char *name);
Object* createFunctionObject(char *name);
Object* createProcedureObject(char *name);
Object* createParameterObject(char *name, enum ParamKind kind);

Object* findObject(ObjectNode *objList, char *name);

void initSymTab(void);
void cleanSymTab(void);
void enterBlock(Scope* scope);
void exitBlock(void);
void declareObject(Object* obj);

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;
Object* writeiProcedure;
Object* writecProcedure;
Object* writelnProcedure;
Object* readiFunction;
Object* readcFunction;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  Type* resultType = (Type*) malloc(sizeof(Type));
  resultType->typeClass = type->typeClass;
  if (type->typeClass == TP_ARRAY) {
    resultType->arraySize = type->arraySize;
    resultType->elementType = duplicateType(type->elementType);
  }
  return resultType;
}

int compareType(Type* type1, Type* type2) {
  if (type1->typeClass == type2->typeClass) {
    if (type1->typeClass == TP_ARRAY) {
      if (type1->arraySize == type2->arraySize)
	return compareType(type1->elementType, type2->elementType);
      else return 0;
    } else return 1;
  } else return 0;
}

void freeType(Type* type) {
  switch (type->typeClass) {
  case TP_INT:
  case TP_CHAR:
    free(type);
    break;
  case TP_ARRAY:
    freeType(type->elementType);
    freeType(type);
    break;
  }
}

int sizeOfType(Type* type) {
  switch (type->typeClass) {
  case TP_INT:
    return INT_SIZE;
  case TP_CHAR:
    return CHAR_SIZE;
  case TP_ARRAY:
    return (type->arraySize * sizeOfType(type->elementType));
  }
  return 0;
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  ConstantValue* value = (ConstantValue*) malloc(sizeof(ConstantValue));
  value->type = TP_INT;
  value->intValue = i;
  return value;
}

ConstantValue* makeCharConstant(char ch) {
  ConstantValue* value = (ConstantValue*) malloc(sizeof(ConstantValue));
  value->type = TP_CHAR;
  value->charValue = ch;
  return value;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  ConstantValue* value = (ConstantValue*) malloc(sizeof(ConstantValue));
  value->type = v->type;
  if (v->type == TP_INT) 
    value->intValue = v->intValue;
  else
    value->charValue = v->charValue;
  return value;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = NULL;
  scope->frameSize = RESERVED_WORDS;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program);
  program->progAttrs->codeAddress = DC_VALUE;
  symtab->program = program;

  return program;
}

Object* createConstantObject(char *name) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_CONSTANT;
  obj->constAttrs = (ConstantAttributes*) malloc(sizeof(ConstantAttributes));
  return obj;
}

Object* createTypeObject(char *name) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_TYPE;
  obj->typeAttrs = (TypeAttributes*) malloc(sizeof(TypeAttributes));
  return obj;
}

Object* createVariableObject(char *name) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_VARIABLE;
  obj->varAttrs = (VariableAttributes*) malloc(sizeof(VariableAttributes));
  obj->varAttrs->type = NULL;
  obj->varAttrs->scope = NULL;
  obj->varAttrs->localOffset = 0;
  return obj;
}

Object* createFunctionObject(char *name) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_FUNCTION;
  obj->funcAttrs = (FunctionAttributes*) malloc(sizeof(FunctionAttributes));
  obj->funcAttrs->returnType = NULL;
  obj->funcAttrs->paramList = NULL;
  obj->funcAttrs->paramCount = 0;
  obj->funcAttrs->codeAddress = DC_VALUE;
  obj->funcAttrs->scope = createScope(obj);
  return obj;
}

Object* createProcedureObject(char *name) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PROCEDURE;
  obj->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));
  obj->procAttrs->paramList = NULL;
  obj->procAttrs->paramCount = 0;
  obj->procAttrs->codeAddress = DC_VALUE;
  obj->procAttrs->scope = createScope(obj);
  return obj;
}

Object* createParameterObject(char *name, enum ParamKind kind) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PARAMETER;
  obj->paramAttrs = (ParameterAttributes*) malloc(sizeof(ParameterAttributes));
  obj->paramAttrs->kind = kind;
  obj->paramAttrs->type = NULL;
  obj->paramAttrs->scope = NULL;
  obj->paramAttrs->localOffset = 0;
  return obj;
}

void freeObject(Object* obj) {
  switch (obj->kind) {
  case OBJ_CONSTANT:
    free(obj->constAttrs->value);
    free(obj->constAttrs);
    break;
  case OBJ_TYPE:
    free(obj->typeAttrs->actualType);
    free(obj->typeAttrs);
    break;
  case OBJ_VARIABLE:
    free(obj->varAttrs->type);
    free(obj->varAttrs);
    break;
  case OBJ_FUNCTION:
    freeReferenceList(obj->funcAttrs->paramList);
    freeType(obj->funcAttrs->returnType);
    freeScope(obj->funcAttrs->scope);
    free(obj->funcAttrs);
    break;
  case OBJ_PROCEDURE:
    freeReferenceList(obj->procAttrs->paramList);
    freeScope(obj->procAttrs->scope);
    free(obj->procAttrs);
    break;
  case OBJ_PROGRAM:
    freeScope(obj->progAttrs->scope);
    free(obj->progAttrs);
    break;
  case OBJ_PARAMETER:
    freeType(obj->paramAttrs->type);
    free(obj->paramAttrs);
  }
  free(obj);
}

void freeScope(Scope* scope) {
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  ObjectNode* list = objList;

  while (list != NULL) {
    ObjectNode* node = list;
    list = list->next;
    freeObject(node->object);
    free(node);
  }
}

void freeReferenceList(ObjectNode *objList) {
  ObjectNode* list = objList;

  while (list != NULL) {
    ObjectNode* node = list;
    list = list->next;
    free(node);
  }
}

void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

int ident_cmp(char *name1, char *name2) {
  char *i = name1;
  char *j = name2; 
  while ((*i != '\0') && (*j != '\0') && (tolower(*i) == tolower(*j))) {
    i++; j++;
  }
  if ((*i == '\0') && (*i == '\0')) return 1;
  else return 0;
}

Object* findObject(ObjectNode *objList, char *name) {
  while (objList != NULL) {
    if (ident_cmp(objList->object->name, name) == 1) 
      return objList->object;
    else objList = objList->next;
  }
  return NULL;
}

void declareObject(Object* obj) {
  Object* owner;

  if (symtab->currentScope == NULL)  //  globalObject
    addObject(&(symtab->globalObjectList), obj);
  else {
    switch (obj->kind) {
    case OBJ_VARIABLE:
      obj->varAttrs->scope = symtab->currentScope;
      obj->varAttrs->localOffset = symtab->currentScope->frameSize;
      symtab->currentScope->frameSize += sizeOfType(obj->varAttrs->type);
      break;
    case OBJ_PARAMETER:
      obj->paramAttrs->scope = symtab->currentScope;
      obj->paramAttrs->localOffset = symtab->currentScope->frameSize;
      symtab->currentScope->frameSize ++;
      owner = symtab->currentScope->owner;
      switch (owner->kind) {
      case OBJ_FUNCTION:
      	addObject(&(owner->funcAttrs->paramList), obj);
       	owner->funcAttrs->paramCount ++;
        break;
      case OBJ_PROCEDURE:
      	addObject(&(owner->procAttrs->paramList), obj);
       	owner->procAttrs->paramCount ++;
        break;
      default:
       	break;
      }
      break;
    case OBJ_FUNCTION:
      obj->funcAttrs->scope->outer = symtab->currentScope;
      break;
    case OBJ_PROCEDURE:
      obj->procAttrs->scope->outer = symtab->currentScope;
      break;
    default: break;
    }
    addObject(&(symtab->currentScope->objList), obj);
  }  
}


/******************* others ******************************/


void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void initSymTab(void) {
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  symtab->program = NULL;
  symtab->currentScope = NULL;
  
  readcFunction = createFunctionObject("READC");
  declareObject(readcFunction);
  readcFunction->funcAttrs->returnType = makeCharType();

  readiFunction = createFunctionObject("READI");
  declareObject(readiFunction);
  readiFunction->funcAttrs->returnType = makeIntType();


  writeiProcedure = createProcedureObject("WRITEI");
  declareObject(writeiProcedure);
  enterBlock(writeiProcedure->procAttrs->scope);
    param = createParameterObject("i", PARAM_VALUE);
    param->paramAttrs->type = makeIntType();
    declareObject(param);
  exitBlock();

  writecProcedure = createProcedureObject("WRITEC");
  declareObject(writecProcedure);
  enterBlock(writecProcedure->procAttrs->scope);
    param = createParameterObject("ch", PARAM_VALUE);
    param->paramAttrs->type = makeCharType();
    declareObject(param);
  exitBlock();
  writelnProcedure = createProcedureObject("WRITELN");
  declareObject(writelnProcedure);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

// scanner
Token *currentToken;
Token *lookAhead;


int state;
int ln, cn, len;
TokenType tkType;
char str[MAX_IDENT_LEN];
char c;
/***************************************************************/
Token* getToken(void) 
{
  Token *token;
  switch(state)
    {
    case 0:
      if (currentChar == EOF)
	state = 1;
      else
  	switch (charCodes[currentChar]) 
	  {
	  case CHAR_SPACE: 
	    state = 2;
	    break;
	  case CHAR_LETTER: 
	    state = 3;
	    break;
	  case CHAR_DIGIT: 
	    state = 7; 
	    break;
	  case CHAR_PLUS:
	    state = 9;
	    break;
	  case CHAR_MINUS:
	    state = 10;
	    break;
	  case CHAR_TIMES:
	    state = 11;
	    break;
	  case CHAR_SLASH:
	    state = 12;
	    break;
	  case CHAR_LT:
	    state = 13;
	    break;
	  case CHAR_GT:
	    state = 16; 
	    break;
	  case CHAR_EQ:
	    state = 19; 
	    break;
	  case CHAR_EXCLAIMATION:
	    state = 20;
	    break;
	  case CHAR_COMMA:
	    state = 23; 
	    break;
	  case CHAR_PERIOD:
	    state = 24; 
	    break;
	  case CHAR_SEMICOLON:
	    state = 27;
	    break;
	  case CHAR_COLON:
	    state = 28;
	    break;
	  case CHAR_SINGLEQUOTE: 
	    state = 31;
	    break;
	  case CHAR_LPAR:
	    state = 35;
	    break;
	  case CHAR_RPAR:
	    state = 42;
	    break;
	  default: 
	    state = 43;
	  }
      return getToken();
    case 1:
      return makeToken(TK_EOF, lineNo, colNo);
    case 2:
      // skip blanks
      while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE))
	readChar();
      if (currentChar == EOF)
	state = 1;
      else state = 0;
      return getToken();
    case 3:
      // Recognize Identifiers and keywords
      ln = lineNo;
      cn = colNo;
      len = 0;
      while ((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT)) {
	str[len++] = currentChar;
	readChar();
      }
      str[len] = '\0';
      if (len > 15)
	error(ERR_IDENT_TOO_LONG, ln, cn);
      else {
	state = 4;
	return getToken();
      }
    case 4:
      tkType = checkKeyword(str);
      if (tkType == TK_NONE) state = 5;
      else state = 6;
      return getToken();
    case 5:
      // identifier
      token = makeToken(TK_IDENT, ln, cn);
      strcpy(token->string, str);
      return token;
    case 6:
      // keywords
      token = makeToken(tkType, ln, cn);
      strcpy(token->string,str);
      return token;
    case 7:
      ln = lineNo;
      cn = colNo;
      len = 0;
      while (charCodes[currentChar] == CHAR_DIGIT) {
	str[len++] = currentChar;
	readChar();
      }
      str[len] = '\0';
      state = 8;
      return getToken();
    case 8:
      if (len > 10)
	error(ERR_NUMBER_TOO_LONG, ln, cn);
      else {
	if (atoll(str) > INT_MAX)
	  error(ERR_NUMBER_TOO_LONG, ln, cn);
	else {
	  token = makeToken(TK_NUMBER, ln, cn);
	  strcpy(token->string, str);
	  token->intValue = atoi(str);
	  return token;
	}
      }
    case 9:
      readChar();
      return makeToken(SB_PLUS, lineNo, colNo-1);
    case 10:
      readChar();
      return makeToken(SB_MINUS, lineNo, colNo-1);
    case 11:
      readChar();
      return makeToken(SB_TIMES, lineNo, colNo-1);
    case 12:
      readChar();
      return makeToken(SB_SLASH, lineNo, colNo-1);
    case 13:
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) state = 14;
      else state = 15; 
      return getToken();
    case 14:
      readChar();
      return makeToken(SB_LE, lineNo, colNo-2);
    case 15:
      return makeToken(SB_LT, lineNo, colNo-1);
    case 16:
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) state = 17;
      else state = 18; 
      return getToken();
    case 17:
      readChar();
      return makeToken(SB_GE, lineNo, colNo-2);
    case 18:
      return makeToken(SB_GT, lineNo, colNo-1);
    case 19:
      readChar();
      return makeToken(SB_EQ, lineNo, colNo-1);
    case 20:
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) state = 21;
      else state = 22; 
      return getToken();
    case 21:
      readChar();
      return makeToken(SB_NEQ, lineNo, colNo-2);
    case 22:
      error(ERR_INVALID_SYMBOL, lineNo, colNo-1);
    case 23:
      readChar();
      return makeToken(SB_COMMA, lineNo, colNo-1);
    case 24: 
      readChar();
      if (charCodes[currentChar] == CHAR_RPAR) state = 25;
      else state = 26; 
      return getToken();
    case 25:
      readChar();
      return makeToken(SB_RSEL, lineNo, colNo-2);
    case 26:
      return makeToken(SB_PERIOD, lineNo, colNo-1);
    case 27:
      readChar();
      return makeToken(SB_SEMICOLON, lineNo, colNo-1);
    case 28:
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) state = 29;
      else state = 30; 
      return getToken();
    case 29:
      readChar();
      return makeToken(SB_ASSIGN, lineNo, colNo-2);
    case 30:
      return makeToken(SB_COLON, lineNo, colNo-1);
    case 31:
      readChar();
      if (currentChar == EOF) 
	state=34;
      else 
	if(isprint(currentChar))
	  state =32;
	else state = 34;
      return getToken();
    case 32:
      str[0] = currentChar;
      str[1] = '\0';
      readChar();
      if (charCodes[currentChar] == CHAR_SINGLEQUOTE) 
	state=33; 
      else 
	state =34;
      return getToken();
    case 33:
      token = makeToken(TK_CHAR, lineNo, colNo-3);
      strcpy(token->string, str);
      token->charValue = str[0];
      readChar();
      return token;
    case 34:
      error(ERR_INVALID_CONSTANT_CHAR, lineNo, colNo-2);
    case 35: // tokens begin with lpar, skip comments
      ln = lineNo;
      cn = colNo;
      readChar();
      switch (charCodes[currentChar]) 
	{
	case CHAR_PERIOD:
	  state = 36;
	  break;
	case CHAR_TIMES:
	  state = 37;
	  break;
	default:
	  state = 41;
	}
      return getToken();
    case 36:
      readChar();
      return makeToken(SB_LSEL, lineNo, colNo - 2);
    case 37:
      readChar();
      while ((currentChar != EOF) && (charCodes[currentChar] != CHAR_TIMES))
	readChar();
      if (currentChar == EOF)
	state = 40;
      else
	state = 38;
      return getToken();
    case 38:
      readChar();
      if (currentChar == EOF)
	state = 40;
      else if (charCodes[currentChar] == CHAR_RPAR)
	state = 39;
      else
	state = 37;
      return getToken();
    case 39:
      readChar();
      state = 0;
      return getToken();
    case 40:
      error(ERR_END_OF_COMMENT, lineNo, colNo);
    case 41:
      return makeToken(SB_LPAR, ln, cn);
    case 42:
      readChar();
      return makeToken(SB_RPAR, lineNo, colNo);
    case 43:
      error(ERR_INVALID_SYMBOL, lineNo, colNo);
    }
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

/******************************************************************/

Token* getValidToken(void) {

  state=0;
  Token *token = getToken();
  
  while (token->tokenType == TK_NONE) {
    free(token);
    token = getToken();
  }
  // printToken(token);
  return token;
}


// debug

void printType(Type* type);
void printConstantValue(ConstantValue* value);
void printObject(Object* obj, int indent);
void printObjectList(ObjectNode* objList, int indent);
void printScope(Scope* scope, int indent);

void pad(int n) {
  int i;
  for (i = 0; i < n ; i++) printf(" ");
}

void printType(Type* type) {
  switch (type->typeClass) {
  case TP_INT:
    printf("Int");
    break;
  case TP_CHAR:
    printf("Char");
    break;
  case TP_ARRAY:
    printf("Arr(%d,",type->arraySize);
    printType(type->elementType);
    printf(")");
    break;
  }
}

void printConstantValue(ConstantValue* value) {
  switch (value->type) {
  case TP_INT:
    printf("%d",value->intValue);
    break;
  case TP_CHAR:
    printf("\'%c\'",value->charValue);
    break;
  default:
    break;
  }
}

void printObject(Object* obj, int indent) {
  switch (obj->kind) {
  case OBJ_CONSTANT:
    pad(indent);
    printf("Const %s = ", obj->name);
    printConstantValue(obj->constAttrs->value);
    break;
  case OBJ_TYPE:
    pad(indent);
    printf("Type %s = ", obj->name);
    printType(obj->typeAttrs->actualType);
    break;
  case OBJ_VARIABLE:
    pad(indent);
    printf("Var %s : ", obj->name);
    printType(obj->varAttrs->type);
    printf(" at offset %d", obj->varAttrs->localOffset);
    break;
  case OBJ_PARAMETER:
    pad(indent);
    if (obj->paramAttrs->kind == PARAM_VALUE) 
      printf("Param %s : ", obj->name);
    else
      printf("Param VAR %s : ", obj->name);
    printType(obj->paramAttrs->type);
    printf(" at offset %d", obj->paramAttrs->localOffset);
    break;
  case OBJ_FUNCTION:
    pad(indent);
    printf("Function %s : ",obj->name);
    printType(obj->funcAttrs->returnType);
    printf(" at address %d\n", obj->funcAttrs->codeAddress);
    printScope(obj->funcAttrs->scope, indent + 4);
    break;
  case OBJ_PROCEDURE:
    pad(indent);
    printf("Procedure %s at address %d\n",obj->name, obj->procAttrs->codeAddress);
    printScope(obj->procAttrs->scope, indent + 4);
    break;
  case OBJ_PROGRAM:
    pad(indent);
    printf("Program %s at address %d\n",obj->name, obj->progAttrs->codeAddress);
    printScope(obj->progAttrs->scope, indent + 4);
    break;
  }
}

void printObjectList(ObjectNode* objList, int indent) {
  ObjectNode *node = objList;
  while (node != NULL) {
    printObject(node->object, indent);
    printf("\n");
    node = node->next;
  }
}

void printScope(Scope* scope, int indent) {
  printObjectList(scope->objList, indent);
}


/* Semantics */

Object* lookupObject(char *name) {
  Scope* scope = symtab->currentScope;
  Object* obj;

  while (scope != NULL) {
    obj = findObject(scope->objList, name);
    if (obj != NULL) return obj;
    scope = scope->outer;
  }
  obj = findObject(symtab->globalObjectList, name);
  if (obj != NULL) return obj;
  return NULL;
}

void checkFreshIdent(char *name) {
  if (findObject(symtab->currentScope->objList, name) != NULL)
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
}

Object* checkDeclaredIdent(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_IDENT,currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredConstant(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_CONSTANT,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_CONSTANT)
    error(ERR_INVALID_CONSTANT,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredType(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_TYPE,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_TYPE)
    error(ERR_INVALID_TYPE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredVariable(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_VARIABLE,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_VARIABLE)
    error(ERR_INVALID_VARIABLE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredFunction(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL)
    error(ERR_UNDECLARED_FUNCTION,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_FUNCTION)
    error(ERR_INVALID_FUNCTION,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredProcedure(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) 
    error(ERR_UNDECLARED_PROCEDURE,currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_PROCEDURE)
    error(ERR_INVALID_PROCEDURE,currentToken->lineNo, currentToken->colNo);

  return obj;
}

Object* checkDeclaredLValueIdent(char* name) {
  Object* obj = lookupObject(name);
  Scope* scope;

  if (obj == NULL)
    error(ERR_UNDECLARED_IDENT,currentToken->lineNo, currentToken->colNo);

  switch (obj->kind) {
  case OBJ_VARIABLE:
  case OBJ_PARAMETER:
    break;
  case OBJ_FUNCTION:
    scope = symtab->currentScope;
    while ((scope != NULL) && (scope != obj->funcAttrs->scope)) 
      scope = scope->outer;

    if (scope == NULL)
      error(ERR_INVALID_IDENT,currentToken->lineNo, currentToken->colNo);
    break;
  default:
    error(ERR_INVALID_IDENT,currentToken->lineNo, currentToken->colNo);
  }

  return obj;
}


void checkIntType(Type* type) {
  if ((type != NULL) && (type->typeClass == TP_INT))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

void checkCharType(Type* type) {
  if ((type != NULL) && (type->typeClass == TP_CHAR))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

void checkBasicType(Type* type) {
  if ((type != NULL) && ((type->typeClass == TP_INT) || (type->typeClass == TP_CHAR)))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

void checkArrayType(Type* type) {
  if ((type != NULL) && (type->typeClass == TP_ARRAY))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

void checkTypeEquality(Type* type1, Type* type2) {
  if (compareType(type1, type2) == 0)
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}


// Codegen

#define CODE_SIZE 10000

#define PROCEDURE_PARAM_COUNT(proc) (proc->procAttrs->numOfParams)
#define PROCEDURE_SCOPE(proc) (proc->procAttrs->scope)
#define PROCEDURE_FRAME_SIZE(proc) (proc->procAttrs->scope->frameSize)

#define FUNCTION_PARAM_COUNT(func) (func->funcAttrs->numOfParams)
#define FUNCTION_SCOPE(func) (func->funcAttrs->scope)
#define FUNCTION_FRAME_SIZE(func) (func->funcAttrs->scope->frameSize)

#define PROGRAM_SCOPE(prog) (prog->progAttrs->scope)
#define PROGRAM_FRAME_SIZE(prog) (prog->progAttrs->scope->frameSize)

#define VARIABLE_OFFSET(var) (var->varAttrs->localOffset)
#define VARIABLE_SCOPE(var) (var->varAttrs->scope)

#define PARAMETER_OFFSET(param) (param->paramAttrs->localOffset)
#define PARAMETER_SCOPE(param) (param->paramAttrs->scope)

#define RETURN_VALUE_OFFSET 0
#define DYNAMIC_LINK_OFFSET 1
#define RETURN_ADDRESS_OFFSET 2
#define STATIC_LINK_OFFSET 3

CodeBlock* codeBlock;


void genLA(int level, int offset) {
  emitLA(codeBlock, level, offset);
}

void genLV(int level, int offset) {
  emitLV(codeBlock, level, offset);
}

void genLC(WORD constant) {
  emitLC(codeBlock, constant);
}

void genLI(void) {
  emitLI(codeBlock);
}

void genINT(int delta) {
  emitINT(codeBlock,delta);
}

void genDCT(int delta) {
  emitDCT(codeBlock,delta);
}

Instruction* genJ(CodeAddress label) {
  Instruction* inst = codeBlock->code + codeBlock->codeSize;
  emitJ(codeBlock,label);
  return inst;
}

Instruction* genFJ(CodeAddress label) {
  Instruction* inst = codeBlock->code + codeBlock->codeSize;
  emitFJ(codeBlock, label);
  return inst;
}

void genHL(void) {
  emitHL(codeBlock);
}

void genST(void) {
  emitST(codeBlock);
}

void genCALL(int level, CodeAddress label) {
  emitCALL(codeBlock, level, label);
}

void genEP(void) {
  emitEP(codeBlock);
}

void genEF(void) {
  emitEF(codeBlock);
}

void genRC(void) {
  emitRC(codeBlock);
}

void genRI(void) {
  emitRI(codeBlock);
}

void genWRC(void) {
  emitWRC(codeBlock);
}

void genWRI(void) {
  emitWRI(codeBlock);
}

void genWLN(void) {
  emitWLN(codeBlock);
}

void genAD(void) {
  emitAD(codeBlock);
}

void genSB(void) {
  emitSB(codeBlock);
}

void genML(void) {
  emitML(codeBlock);
}

void genDV(void) {
  emitDV(codeBlock);
}

void genNEG(void) {
  emitNEG(codeBlock);
}

void genCV(void) {
  emitCV(codeBlock);
}

void genEQ(void) {
  emitEQ(codeBlock);
}

void genNE(void) {
  emitNE(codeBlock);
}

void genGT(void) {
  emitGT(codeBlock);
}

void genGE(void) {
  emitGE(codeBlock);
}

void genLT(void) {
  emitLT(codeBlock);
}

void genLE(void) {
  emitLE(codeBlock);
}

void genVariableAddress(Object* var) {
  int level = 0;
  int offset = VARIABLE_OFFSET(var);
  genLA(level, offset);
}

void genVariableValue(Object* var) {
  int level = 0;
  int offset = VARIABLE_OFFSET(var);
  genLV(level, offset);
}

void genPredefinedProcedureCall(Object* proc) {
  if (proc == writeiProcedure)
    genWRI();
  else if (proc == writecProcedure)
    genWRC();
  else if (proc == writelnProcedure)
    genWLN();
}

void genPredefinedFunctionCall(Object* func) {
  if (func == readiFunction)
    genRI();
  else if (func == readcFunction)
    genRC();
}

void updateJ(Instruction* jmp, CodeAddress label) {
  jmp->q = label;
}

void updateFJ(Instruction* jmp, CodeAddress label) {
  jmp->q = label;
}

CodeAddress getCurrentCodeAddress(void) {
  return codeBlock->codeSize;
}

int isPredefinedFunction(Object* func) {
  return ((func == readiFunction) || (func == readcFunction));
}

int isPredefinedProcedure(Object* proc) {
  return ((proc == writeiProcedure) || (proc == writecProcedure) || (proc == writelnProcedure));
}

void initCodeBuffer(void) {
  codeBlock = createCodeBlock(CODE_SIZE);
}

void printCodeBuffer(void) {
  printCodeBlock(codeBlock);
}

void cleanCodeBuffer(void) {
  freeCodeBlock(codeBlock);
}

// Parser
void compileProgram(void);
void compileBlock(void);
void compileBlock2(void);
void compileBlock3(void);
void compileBlock4(void);
void compileBlock5(void);
void compileConstDecls(void);
void compileConstDecl(void);
void compileTypeDecls(void);
void compileTypeDecl(void);
void compileVarDecls(void);
void compileVarDecl(void);
void compileSubDecls(void);
void compileFuncDecl(void);
void compileProcDecl(void);
ConstantValue* compileUnsignedConstant(void);
ConstantValue* compileConstant(void);
ConstantValue* compileConstant2(void);
Type* compileType(void);
Type* compileBasicType(void);
void compileParams(void);
void compileParam(void);
void compileStatements(void);
void compileStatement(void);
Type* compileLValue(void);
void compileAssignSt(void);
void compileCallSt(void);
void compileGroupSt(void);
void compileIfSt(void);
void compileElseSt(void);
void compileWhileSt(void);
void compileForSt(void);
void compileArgument(Object* param);
void compileArguments(ObjectNode* paramList);
void compileCondition(void);
Type* compileExpression(void);
Type* compileExpression2(void);
Type* compileExpression3(Type* argType1);
Type* compileTerm(void);
Type* compileTerm2(Type* argType2);
Type* compileFactor(void);
Type* compileIndexes(Type* arrayType);


void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    //    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  Object* program;

  eat(KW_PROGRAM);
  eat(TK_IDENT);

  program = createProgramObject(currentToken->string);
  program->progAttrs->codeAddress = getCurrentCodeAddress();
  enterBlock(program->progAttrs->scope);

  eat(SB_SEMICOLON);

  compileBlock();
  eat(SB_PERIOD);

  // Halt the program
  genHL();

  exitBlock();
}

void compileConstDecls(void) {
  Object* constObj;
  ConstantValue* constValue;

  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    do {
      eat(TK_IDENT);
      checkFreshIdent(currentToken->string);
      constObj = createConstantObject(currentToken->string);
      declareObject(constObj);
      
      eat(SB_EQ);
      constValue = compileConstant();
      constObj->constAttrs->value = constValue;
      
      eat(SB_SEMICOLON);
    } while (lookAhead->tokenType == TK_IDENT);
  }
}

void compileTypeDecls(void) {
  Object* typeObj;
  Type* actualType;

  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    do {
      eat(TK_IDENT);
      
      checkFreshIdent(currentToken->string);
      typeObj = createTypeObject(currentToken->string);
      declareObject(typeObj);
      
      eat(SB_EQ);
      actualType = compileType();
      typeObj->typeAttrs->actualType = actualType;
      
      eat(SB_SEMICOLON);
    } while (lookAhead->tokenType == TK_IDENT);
  } 
}

void compileVarDecls(void) {
  Object* varObj;
  Type* varType;

  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    do {
      eat(TK_IDENT);
      checkFreshIdent(currentToken->string);
      varObj = createVariableObject(currentToken->string);
      eat(SB_COLON);
      varType = compileType();
      varObj->varAttrs->type = varType;
      declareObject(varObj);      
      eat(SB_SEMICOLON);
    } while (lookAhead->tokenType == TK_IDENT);
  } 
}

void compileBlock(void) {
  Instruction* jmp;
  // Jump to the body of the block
  jmp = genJ(DC_VALUE);

  compileConstDecls();
  compileTypeDecls();
  compileVarDecls();
  compileSubDecls();

  // Update the jmp label
  updateJ(jmp,getCurrentCodeAddress());
  // Skip the stack frame
  genINT(symtab->currentScope->frameSize);

  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileSubDecls(void) {
  while ((lookAhead->tokenType == KW_FUNCTION) || (lookAhead->tokenType == KW_PROCEDURE)) {
    if (lookAhead->tokenType == KW_FUNCTION)
      compileFuncDecl();
    else compileProcDecl();
  }
}

void compileFuncDecl(void) {
  Object* funcObj;
  Type* returnType;

  eat(KW_FUNCTION);
  eat(TK_IDENT);

  checkFreshIdent(currentToken->string);
  funcObj = createFunctionObject(currentToken->string);
  funcObj->funcAttrs->codeAddress = getCurrentCodeAddress();
  declareObject(funcObj);

  enterBlock(funcObj->funcAttrs->scope);
  
  compileParams();

  eat(SB_COLON);
  returnType = compileBasicType();
  funcObj->funcAttrs->returnType = returnType;

  eat(SB_SEMICOLON);

  compileBlock();

  eat(SB_SEMICOLON);

  exitBlock();
}

void compileProcDecl(void) {
  Object* procObj;

  eat(KW_PROCEDURE);
  eat(TK_IDENT);

  checkFreshIdent(currentToken->string);
  procObj = createProcedureObject(currentToken->string);
  procObj->procAttrs->codeAddress = getCurrentCodeAddress();
  declareObject(procObj);

  enterBlock(procObj->procAttrs->scope);

  compileParams();

  eat(SB_SEMICOLON);
  compileBlock();

  eat(SB_SEMICOLON);

  exitBlock();
}

ConstantValue* compileUnsignedConstant(void) {
  ConstantValue* constValue;
  Object* obj;

  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    constValue = makeIntConstant(currentToken->intValue);
    break;
  case TK_IDENT:
    eat(TK_IDENT);

    obj = checkDeclaredConstant(currentToken->string);
    constValue = duplicateConstantValue(obj->constAttrs->value);

    break;
  case TK_CHAR:
    eat(TK_CHAR);
    constValue = makeCharConstant(currentToken->string[0]);
    break;
  default:
    error(ERR_INVALID_CONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  return constValue;
}

ConstantValue* compileConstant(void) {
  ConstantValue* constValue;

  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    constValue = compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    constValue = compileConstant2();
    constValue->intValue = - constValue->intValue;
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    constValue = makeCharConstant(currentToken->string[0]);
    break;
  default:
    constValue = compileConstant2();
    break;
  }
  return constValue;
}

ConstantValue* compileConstant2(void) {
  ConstantValue* constValue;
  Object* obj;

  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    constValue = makeIntConstant(currentToken->intValue);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    obj = checkDeclaredConstant(currentToken->string);
    if (obj->constAttrs->value->type == TP_INT)
      constValue = duplicateConstantValue(obj->constAttrs->value);
    else
      error(ERR_UNDECLARED_INT_CONSTANT,currentToken->lineNo, currentToken->colNo);
    break;
  default:
    error(ERR_INVALID_CONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  return constValue;
}

Type* compileType(void) {
  Type* type;
  Type* elementType;
  int arraySize;
  Object* obj;

  switch (lookAhead->tokenType) {
  case KW_INTEGER: 
    eat(KW_INTEGER);
    type =  makeIntType();
    break;
  case KW_CHAR: 
    eat(KW_CHAR); 
    type = makeCharType();
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);

    arraySize = currentToken->intValue;

    eat(SB_RSEL);
    eat(KW_OF);
    elementType = compileType();
    type = makeArrayType(arraySize, elementType);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    obj = checkDeclaredType(currentToken->string);
    type = duplicateType(obj->typeAttrs->actualType);
    break;
  default:
    error(ERR_INVALID_TYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  return type;
}

Type* compileBasicType(void) {
  Type* type;

  switch (lookAhead->tokenType) {
  case KW_INTEGER: 
    eat(KW_INTEGER); 
    type = makeIntType();
    break;
  case KW_CHAR: 
    eat(KW_CHAR); 
    type = makeCharType();
    break;
  default:
    error(ERR_INVALID_BASICTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  return type;
}

void compileParams(void) {
  if (lookAhead->tokenType == SB_LPAR) {
    eat(SB_LPAR);
    compileParam();
    while (lookAhead->tokenType == SB_SEMICOLON) {
      eat(SB_SEMICOLON);
      compileParam();
    }
    eat(SB_RPAR);
  }
}

void compileParam(void) {
  Object* param;
  Type* type;
  enum ParamKind paramKind = PARAM_VALUE;

  if (lookAhead->tokenType == KW_VAR) {
    paramKind = PARAM_REFERENCE;
    eat(KW_VAR);
  }

  eat(TK_IDENT);
  checkFreshIdent(currentToken->string);
  param = createParameterObject(currentToken->string, paramKind);
  eat(SB_COLON);
  type = compileBasicType();
  param->paramAttrs->type = type;
  declareObject(param);
}

void compileStatements(void) {
  compileStatement();
  while (lookAhead->tokenType == SB_SEMICOLON) {
    eat(SB_SEMICOLON);
    compileStatement();
  }
}

void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALID_STATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

Type* compileLValue(void) {
  Object* var;
  Type* varType;

  eat(TK_IDENT);
  
  var = checkDeclaredLValueIdent(currentToken->string);

  switch (var->kind) {
  case OBJ_VARIABLE:
    // push the variable address onto the stack
    genVariableAddress(var);

    if (var->varAttrs->type->typeClass == TP_ARRAY) {
      // compute the element address
      varType = compileIndexes(var->varAttrs->type);
    }
    else
      varType = var->varAttrs->type;
    break;
  case OBJ_PARAMETER:
    // TEMPORARY: halt the program
    genHL();
    varType = var->paramAttrs->type;
    break;
  case OBJ_FUNCTION:
    // TEMPORARY: halt the program
    genHL();
    varType = var->funcAttrs->returnType;
    break;
  default: 
    error(ERR_INVALID_LVALUE,currentToken->lineNo, currentToken->colNo);
  }

  return varType;
}

void compileAssignSt(void) {
  // TODO
  Type* varType;
  Type* expType;

  varType = compileLValue();
  
  eat(SB_ASSIGN);
  expType = compileExpression();
  checkTypeEquality(varType, expType);
  
  genST();
}

void compileCallSt(void) {
  // Generate code for call-statement
  // TEMPORARY. halt
  Object* proc;

  eat(KW_CALL);
  eat(TK_IDENT);

  proc = checkDeclaredProcedure(currentToken->string);

  if (isPredefinedProcedure(proc)) {
    compileArguments(proc->procAttrs->paramList);
    genPredefinedProcedureCall(proc);
  } else {
    compileArguments(proc->procAttrs->paramList);
    genHL();
  }
}

void compileGroupSt(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileIfSt(void) {
  // TODO
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  
  Instruction* jmpFJ = genFJ(DC_VALUE); // Jump False to Else/End
  
  compileStatement();
  
  if (lookAhead->tokenType == KW_ELSE) {
    Instruction* jmpJ = genJ(DC_VALUE); // Jump to End (skip Else)
    updateFJ(jmpFJ, getCurrentCodeAddress()); // Start of Else
    
    eat(KW_ELSE);
    compileStatement();
    
    updateJ(jmpJ, getCurrentCodeAddress()); // End of If
  } else {
    updateFJ(jmpFJ, getCurrentCodeAddress()); // End of If
  }
}

void compileWhileSt(void) {
  // TODO
   CodeAddress labelLoop = getCurrentCodeAddress();
  
  eat(KW_WHILE);
  compileCondition();
  
  Instruction* jmpFJ = genFJ(DC_VALUE); // Exit loop if false
  
  eat(KW_DO);
  compileStatement();
  
  genJ(labelLoop); // Jump back to condition
  updateFJ(jmpFJ, getCurrentCodeAddress());
}

void compileForSt(void) {
  // TODO
  // FOR var := expr1 TO expr2 DO statement
  eat(KW_FOR);
  
  // Xử lý biến đếm (LValue)
  // Ta cần địa chỉ để gán, và giá trị để so sánh/tăng
  // Vì compileLValue sẽ 'eat' token, ta gọi nó để lấy địa chỉ
  // Lưu ý: compileLValue sinh mã đẩy địa chỉ lên stack
  eat(TK_IDENT);
  Object* var = checkDeclaredLValueIdent(currentToken->string);
  genVariableAddress(var); 

  eat(SB_ASSIGN);
  Type* type = compileExpression();
  checkTypeEquality(var->varAttrs->type, type);
  
  genST(); // Khởi tạo: var := expr1
  
  CodeAddress labelLoop = getCurrentCodeAddress();
  
  eat(KW_TO);
  
  // Đẩy giá trị var lên stack để so sánh
  genVariableValue(var);
  
  type = compileExpression(); // expr2 (Limit)
  checkTypeEquality(var->varAttrs->type, type);
  
  genLE(); // var <= limit ?
  
  Instruction* jmpFJ = genFJ(DC_VALUE); // Nếu false (var > limit) thì thoát
  
  eat(KW_DO);
  compileStatement();
  
  // Tăng biến đếm: var := var + 1
  genVariableAddress(var);
  genVariableValue(var);
  genLC(1);
  genAD();
  genST();
  
  genJ(labelLoop);
  
  updateFJ(jmpFJ, getCurrentCodeAddress());
}

void compileArgument(Object* param) {
  Type* type;

  if (param->paramAttrs->kind == PARAM_VALUE) {
    type = compileExpression();
    checkTypeEquality(type, param->paramAttrs->type);
  } else {
    type = compileLValue();
    checkTypeEquality(type, param->paramAttrs->type);
  }
}

void compileArguments(ObjectNode* paramList) {
  ObjectNode* node = paramList;

  switch (lookAhead->tokenType) {
  case SB_LPAR:
    eat(SB_LPAR);
    if (node == NULL)
      error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
    compileArgument(node->object);
    node = node->next;

    while (lookAhead->tokenType == SB_COMMA) {
      eat(SB_COMMA);
      if (node == NULL)
	error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
      compileArgument(node->object);
      node = node->next;
    }

    if (node != NULL)
      error(ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
    
    eat(SB_RPAR);
    break;
    // Check FOLLOW set 
  case SB_TIMES:
  case SB_SLASH:
  case SB_PLUS:
  case SB_MINUS:
  case KW_TO:
  case KW_DO:
  case SB_RPAR:
  case SB_COMMA:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  case SB_RSEL:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
    break;
  default:
    error(ERR_INVALID_ARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileCondition(void) {
  // TODO
  Type* type1;
  Type* type2;
  TokenType op;

  type1 = compileExpression();
  checkBasicType(type1);

  op = lookAhead->tokenType;
  switch (op) {
  case SB_EQ:
    eat(SB_EQ);
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    break;
  case SB_LE:
    eat(SB_LE);
    break;
  case SB_LT:
    eat(SB_LT);
    break;
  case SB_GE:
    eat(SB_GE);
    break;
  case SB_GT:
    eat(SB_GT);
    break;
  default:
    error(ERR_INVALID_COMPARATOR, lookAhead->lineNo, lookAhead->colNo);
  }

  type2 = compileExpression();
  checkTypeEquality(type1,type2);
  
  switch(op) {
    case SB_EQ: genEQ(); break;
    case SB_NEQ: genNE(); break;
    case SB_LE: genLE(); break;
    case SB_LT: genLT(); break;
    case SB_GE: genGE(); break;
    case SB_GT: genGT(); break;
    default: break;
  }
}

Type* compileExpression(void) {
  // TODO
   Type* type;
  
  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    type = compileExpression2();
    checkIntType(type);
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    type = compileExpression2();
    checkIntType(type);
    genNEG(); // Unary minus
    break;
  default:
    type = compileExpression2();
  }
  return type;
}

Type* compileExpression2(void) {
  Type* type;

  type = compileTerm();
  type = compileExpression3(type);

  return type;
}


Type* compileExpression3(Type* argType1) {
  // TODO
  Type* argType2;
  Type* resultType;

  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    checkIntType(argType1);
    argType2 = compileTerm();
    checkIntType(argType2);
    
    genAD();

    resultType = compileExpression3(argType1);
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    checkIntType(argType1);
    argType2 = compileTerm();
    checkIntType(argType2);
    
    genSB();

    resultType = compileExpression3(argType1);
    break;
    // check the FOLLOW set
  case KW_TO:
  case KW_DO:
  case SB_RPAR:
  case SB_COMMA:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  case SB_RSEL:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
    resultType = argType1;
    break;
  default:
    error(ERR_INVALID_EXPRESSION, lookAhead->lineNo, lookAhead->colNo);
  }
  return resultType;
}

Type* compileTerm(void) {
  Type* type;
  type = compileFactor();
  type = compileTerm2(type);

  return type;
}

Type* compileTerm2(Type* argType1) {
  // TODO
   Type* argType2;
  Type* resultType;

  switch (lookAhead->tokenType) {
  case SB_TIMES:
    eat(SB_TIMES);
    checkIntType(argType1);
    argType2 = compileFactor();
    checkIntType(argType2);
    
    genML();

    resultType = compileTerm2(argType1);
    break;
  case SB_SLASH:
    eat(SB_SLASH);
    checkIntType(argType1);
    argType2 = compileFactor();
    checkIntType(argType2);
    
    genDV();

    resultType = compileTerm2(argType1);
    break;
    // check the FOLLOW set
  case SB_PLUS:
  case SB_MINUS:
  case KW_TO:
  case KW_DO:
  case SB_RPAR:
  case SB_COMMA:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  case SB_RSEL:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
    resultType = argType1;
    break;
  default:
    error(ERR_INVALID_TERM, lookAhead->lineNo, lookAhead->colNo);
  }
  return resultType;
}

Type* compileFactor(void) {
  // TODO
  Type* type;
  Object* obj;

  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    type = intType;
    genLC(currentToken->intValue);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    type = charType;
    genLC(currentToken->string[0]);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    obj = checkDeclaredIdent(currentToken->string);

    switch (obj->kind) {
    case OBJ_CONSTANT:
      switch (obj->constAttrs->value->type) {
      case TP_INT:
        type = intType;
        genLC(obj->constAttrs->value->intValue);
        break;
      case TP_CHAR:
        type = charType;
        genLC(obj->constAttrs->value->charValue);
        break;
      default:
        break;
      }
      break;
    case OBJ_VARIABLE:
      if (obj->varAttrs->type->typeClass == TP_ARRAY) {
        genVariableAddress(obj); // Base address
        type = compileIndexes(obj->varAttrs->type); // Offset calculation
        genLI(); // Load value from address
      } else {
        genVariableValue(obj);
        type = obj->varAttrs->type;
      }
      break;
    case OBJ_PARAMETER:
      if (obj->paramAttrs->kind == PARAM_VALUE) {
          genVariableValue(obj);
      } else {
          genVariableValue(obj); // Lấy địa chỉ tham chiếu
          genLI(); // Lấy giá trị tại địa chỉ đó
      }
      type = obj->paramAttrs->type;
      break;
    case OBJ_FUNCTION:
      if (isPredefinedFunction(obj)) {
        compileArguments(obj->funcAttrs->paramList);
        genPredefinedFunctionCall(obj);
      } else {
        compileArguments(obj->funcAttrs->paramList);
        int diff = 0;
        Scope* scope = symtab->currentScope;
        while (scope != obj->funcAttrs->scope->outer && scope != NULL) {
            scope = scope->outer;
            diff++;
        }
        genCALL(diff, obj->funcAttrs->codeAddress);
      }
      type = obj->funcAttrs->returnType;
      break;
    default: 
      error(ERR_INVALID_FACTOR,currentToken->lineNo, currentToken->colNo);
      break;
    }
    break;
  case SB_LPAR:
    eat(SB_LPAR);
    type = compileExpression();
    eat(SB_RPAR);
    break;
  default:
    error(ERR_INVALID_FACTOR, lookAhead->lineNo, lookAhead->colNo);
  }
  
  return type;
}

Type* compileIndexes(Type* arrayType) {
  // TEMPORARY: halt
  Type* type;

  
  while (lookAhead->tokenType == SB_LSEL) {
    eat(SB_LSEL);
    type = compileExpression();
    checkIntType(type);
    checkArrayType(arrayType);

    genHL();

    arrayType = arrayType->elementType;
    eat(SB_RSEL);
  }
  checkBasicType(arrayType);
  return arrayType;
}

int compile() {
  lineNo = 1;
  colNo = 0;
  readChar();

  currentToken = NULL;
  lookAhead = getValidToken();

  initSymTab();

  compileProgram();

  cleanSymTab();
  free(currentToken);
  free(lookAhead);

  return IO_SUCCESS;
}

int main(int argc, char *argv[]) {
  int i; 
  if (argc <= 1) {
    printf("Usage: %s <source-file>\n", argv[0]);
    return IO_ERROR;
  }
  if (freopen(argv[1], "r", stdin) == NULL) {
    printf("Cannot open file %s\n", argv[1]);
    return 0;
  }

  initCodeBuffer();
  compile();
  printCodeBuffer();
  cleanCodeBuffer();

  return 0;
}

