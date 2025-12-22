#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

/* Reader */
#define IO_ERROR 0
#define IO_SUCCESS 1

FILE *inputStream;
int lineNo, colNo;
int currentChar;
int len;
int readChar(void) {
  currentChar = getc(stdin);
  colNo ++;
  if (currentChar == '\n') {
    lineNo ++;
    colNo = 0;
  }
  return currentChar;
}

int openInputStream(char *fileName) {
  inputStream = fopen(fileName, "rt");
  if (inputStream == NULL)
    return IO_ERROR;

  lineNo = 1;
  colNo = 0;
  readChar();
  return IO_SUCCESS;
}

void closeInputStream() {
  fclose(inputStream);
}

/* Error */

typedef enum {
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSYMBOL,
  ERR_NUMBERTOOLONG
} ErrorCode;


#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"
#define ERM_NUMBERTOOLONG "Value of integer number exceeds the range!"

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
  }
  // quit immediately when getting error 
  exit(0);
}

/* CharCode  */

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

/* Token */

#define MAX_IDENT_LEN 15
#define KEYWORDS_COUNT 21

typedef enum {
  TK_NONE, TK_IDENT, TK_NUMBER, TK_REAL, TK_CHAR, TK_EOF,

  KW_PROGRAM, KW_CONST, KW_TYPE, KW_VAR,
  KW_INTEGER, KW_REAL, KW_CHAR, KW_ARRAY, KW_OF, 
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
  {"REAL", KW_REAL},
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

/* Scanner */

int state;
int ln, cn, count;
TokenType tkType;
char str[1000];
char c;

// Biến toàn cục để lưu token type trước đó
TokenType prevTokenType = TK_NONE;

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
      // numbers, combine to state 8
      token = makeToken(TK_NUMBER, lineNo, colNo);
      len = 0;
      while (charCodes[currentChar] == CHAR_DIGIT) {
	str[len++] = currentChar;
	readChar();
      }
      str[len] = '\0';

      if (len > 10)
	error(ERR_NUMBERTOOLONG, ln, cn);
      else {
	strcpy(token->string, str);
	token->value = atoi(str);
	return token;
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

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_REAL: printf("TK_REAL(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_REAL: printf("KW_REAL\n"); break;
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


int scan() {
  Token *token;

  lineNo = 1;
  colNo = 0;
  readChar();
  state = 0;
  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    state = 0;
    token = getToken();
  }

  free(token);
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  scan();
  return 0;
}