#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

/* reader */

#define IO_ERROR 0
#define IO_SUCCESS 1

FILE *inputStream;
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

/* CharCode */

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
  CHAR_EXCLAMATION,
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

  CHAR_SPACE, CHAR_EXCLAMATION, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_SINGLEQUOTE,
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

/* Token */
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
  int value;
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

/* Error */

typedef enum {
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_NUMBERTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSYMBOL,
  ERR_INVALIDCONSTANT,
  ERR_INVALIDTYPE,
  ERR_INVALIDBASICTYPE,
  ERR_INVALIDPARAM,
  ERR_INVALIDSTATEMENT,
  ERR_INVALIDARGUMENTS,
  ERR_INVALIDCOMPARATOR,
  ERR_INVALIDEXPRESSION,
  ERR_INVALIDTERM,
  ERR_INVALIDFACTOR
} ErrorCode;

#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_NUMBERTOOLONG "Value of integer number exceeds the range!"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"
#define ERM_INVALIDCONSTANT "Invalid constant!"
#define ERM_INVALIDTYPE "Invalid type!"
#define ERM_INVALIDBASICTYPE "Invalid basic type!"
#define ERM_INVALIDPARAM "Invalid parameter!"
#define ERM_INVALIDSTATEMENT "Invalid statement!"
#define ERM_INVALIDARGUMENTS "Invalid arguments!"
#define ERM_INVALIDCOMPARATOR "Invalid comparator!"
#define ERM_INVALIDEXPRESSION "Invalid expression!"
#define ERM_INVALIDTERM "Invalid term!"
#define ERM_INVALIDFACTOR "Invalid factor!"

void error(ErrorCode err, int lineNo, int colNo) {
  switch (err) {
  case ERR_ENDOFCOMMENT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_ENDOFCOMMENT);
    break;
  case ERR_IDENTTOOLONG:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_IDENTTOOLONG);
    break;
  case ERR_NUMBERTOOLONG:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_NUMBERTOOLONG);
    break;
  case ERR_INVALIDCHARCONSTANT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDCHARCONSTANT);
    break;
  case ERR_INVALIDSYMBOL:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDSYMBOL);
    break;
  case ERR_INVALIDCONSTANT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDCONSTANT);
    break;
  case ERR_INVALIDTYPE:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDTYPE);
    break;
  case ERR_INVALIDBASICTYPE:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDBASICTYPE);
    break;
  case ERR_INVALIDPARAM:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDPARAM);
    break;
  case ERR_INVALIDSTATEMENT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDSTATEMENT);
    break;
  case ERR_INVALIDARGUMENTS:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDARGUMENTS);
    break;
  case ERR_INVALIDCOMPARATOR:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDCOMPARATOR);
    break;
  case ERR_INVALIDEXPRESSION:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDEXPRESSION);
    break;
  case ERR_INVALIDTERM:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDTERM);
    break;
  case ERR_INVALIDFACTOR:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDFACTOR);
    break;
  }
  exit(0);
}

void missingToken(TokenType tokenType, int lineNo, int colNo) {
  printf("%d-%d:Missing %s\n", lineNo, colNo, tokenToString(tokenType));
  exit(0);
}

void assert(char *msg) {
  printf("%s\n", msg);
}

/* Scanner */

int state;
int ln, cn, len;
TokenType tkType;
char str[1000];
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
	    state = 12;
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
	  case CHAR_EXCLAMATION:
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
	error(ERR_IDENTTOOLONG, ln, cn);
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
      // numbers
      ln = lineNo;
      cn = colNo;
      len = 0;
      str[len] = '\0';
      while (charCodes[currentChar] == CHAR_DIGIT) {
	str[len++] = currentChar;
	readChar();
      }
      str[len] = '\0';
      state = 8;
      return getToken();
    case 8:
      if (len > 10)
	error(ERR_NUMBERTOOLONG, ln, cn);
      else {
	if (atoll(str) > INT_MAX)
	  error(ERR_NUMBERTOOLONG, ln, cn);
	else {
	  token = makeToken(TK_NUMBER, ln, cn);
	  strcpy(token->string, str);
	  token->value = atoi(str);
	  return token;
	}
      }
    case 9:
      token = makeToken(SB_PLUS, lineNo, colNo);
      readChar();
      return token;
    case 10:
      token = makeToken(SB_MINUS, lineNo, colNo);
      readChar();
      return token;
    case 11:
      token = makeToken(SB_TIMES, lineNo, colNo);
      readChar();
      return token;
    case 12:
      token = makeToken(SB_SLASH, lineNo, colNo);
      readChar();
      return token;
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
      error(ERR_INVALIDSYMBOL, lineNo, colNo-1);
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
      token = makeToken(SB_SEMICOLON, lineNo, colNo);
      readChar();
      return token;
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
      readChar();
      return token;
    case 34:
      error(ERR_INVALIDCHARCONSTANT, lineNo, colNo-2);
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
      error(ERR_ENDOFCOMMENT, lineNo, colNo);
    case 41:
      return makeToken(SB_LPAR, ln, cn);
    case 42:
      readChar();
      return makeToken(SB_RPAR, lineNo, colNo);
    case 43:
      error(ERR_INVALIDSYMBOL, lineNo, colNo);
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
  return token;
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



/* Parser header */
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
void compileUnsignedConstant(void);
void compileConstant(void);
void compileConstant2(void);
void compileType(void);
void compileBasicType(void);
void compileParams(void);
void compileParams2(void);
void compileParam(void);
void compileStatements(void);
void compileStatements2(void);
void compileStatement(void);
void compileAssignSt(void);
void compileCallSt(void);
void compileGroupSt(void);
void compileIfSt(void);
void compileElseSt(void);
void compileWhileSt(void);
void compileForSt(void);
void compileArguments(void);
void compileArguments2(void);
void compileCondition(void);
void compileCondition2(void);
void compileExpression(void);
void compileExpression2(void);
void compileExpression3(void);
void compileTerm(void);
void compileTerm2(void);
void compileFactor(void);
void compileIndexes(void);

int compile();

/* Parser */

Token *currentToken;
Token *lookAhead;

void scan(void) { 
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}

void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void) {
  while (lookAhead->tokenType == TK_IDENT) 
    compileConstDecl();
}

void compileConstDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}

void compileTypeDecls(void) {
  while (lookAhead->tokenType == TK_IDENT) 
    compileTypeDecl();
}

void compileTypeDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}

void compileVarDecls(void) {
  while (lookAhead->tokenType == TK_IDENT) 
    compileVarDecl();
}

void compileVarDecl(void) {
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

void compileSubDecls(void) {
  assert("Parsing subroutines ....");
  while ((lookAhead->tokenType == KW_FUNCTION) || (lookAhead->tokenType == KW_PROCEDURE))
    if (lookAhead->tokenType == KW_FUNCTION)
      compileFuncDecl();
    else
      compileProcDecl();
  assert("Subroutines parsed ....");
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
    
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileConstant(void) {
  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileConstant2();
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  case TK_IDENT:
  case TK_NUMBER:
    compileConstant2();
    break;
    
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileConstant2(void) {
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileType(void) {
  switch (lookAhead->tokenType) {
  case KW_INTEGER: 
    eat(KW_INTEGER); 
    break;
  case KW_CHAR: 
    eat(KW_CHAR); 
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  default:
    error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileBasicType(void) {
  switch (lookAhead->tokenType) {
  case KW_INTEGER: 
    eat(KW_INTEGER); 
    break;
  case KW_CHAR: 
    eat(KW_CHAR); 
    break;
  default:
    error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileParams(void) {
  if (lookAhead->tokenType == SB_LPAR) {
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
  }
}

void compileParams2(void) {
  while (lookAhead->tokenType == SB_SEMICOLON) {
    eat(SB_SEMICOLON);
    compileParam();
  }
}

void compileParam(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT: 
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  case KW_VAR: 
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  default:
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  
}

void compileStatements(void) {
  compileStatement();
  compileStatements2();
}

void compileStatements2(void) {
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
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  eat(TK_IDENT);
  compileIndexes();
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement parsed ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

void compileArguments(void) {
  if (lookAhead->tokenType == SB_LPAR) {
    eat(SB_LPAR);
    compileExpression();
    compileArguments2();
    eat(SB_RPAR);
  }
}

void compileArguments2(void) {
  while (lookAhead->tokenType == SB_COMMA) {
    eat(SB_COMMA);
    compileExpression();
  }
}

void compileCondition(void) {
  compileExpression();
  compileCondition2();
}

void compileCondition2(void) {
  switch (lookAhead->tokenType) {
  case SB_EQ:
    eat(SB_EQ);
    compileExpression();
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    compileExpression();
    break;
  case SB_LE:
    eat(SB_LE);
    compileExpression();
    break;
  case SB_LT:
    eat(SB_LT);
    compileExpression();
    break;
  case SB_GE:
    eat(SB_GE);
    compileExpression();
    break;
  case SB_GT:
    eat(SB_GT);
    compileExpression();
    break;
  default:
    error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileExpression(void) {
  assert("Parsing an expression");
  // TODO
  if (lookAhead->tokenType == SB_PLUS) {
    eat(SB_PLUS);
  }
  else if (lookAhead->tokenType == SB_MINUS) {
    eat(SB_MINUS);
  }
  compileExpression2();
  assert("Expression parsed");
}

void compileExpression2(void) {
  // TODO
  compileTerm();  
  compileExpression3();
}


void compileExpression3(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    compileTerm();
    compileExpression3();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileTerm();
    compileExpression3();
    break;
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
    error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileTerm(void) {
  // TODO
  compileFactor();
  compileTerm2();
}

void compileTerm2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_TIMES:
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
    break;
  case SB_SLASH:
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
    break;
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
    error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileFactor(void) {
  // TODO
  switch (lookAhead->tokenType) {
 case TK_NUMBER:
 eat(TK_NUMBER);
 break;
 case TK_CHAR:
 eat(TK_CHAR);
 break;
 case TK_IDENT:
 eat(TK_IDENT);
 switch (lookAhead->tokenType) {
 case SB_LSEL:
 compileIndexes();
 break;
 case SB_LPAR:
 compileArguments();
 break;
 default: break;
 }
 break;
  case SB_LPAR:
 eat(SB_LPAR);
 compileExpression();
 eat(SB_RPAR);
 break;
 default:
 error(ERR_INVALIDFACTOR, 
lookAhead->lineNo, lookAhead->colNo);
 }
}

void compileIndexes(void) {
  // TODO
  while (lookAhead->tokenType == SB_LSEL) {
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
  }
}

int compile() {
  lineNo = 1;
  colNo = 0;
  readChar();
  currentToken = NULL;
  lookAhead = getValidToken();
  compileProgram();
  free(currentToken);
  free(lookAhead);
  return IO_SUCCESS;
}


/* main */

int main(int argc, char *argv[]) {
  compile();    
  return 0;
}
