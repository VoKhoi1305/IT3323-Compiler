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
  //printf("%c\n", currentChar);
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
  CHAR_DOUBLEQUOTE,              // ADDED FOR EXAMINATION
  CHAR_LPAR,
  CHAR_RPAR,
  CHAR_LBRAC,                    // ADDED FOR EXAMINATION
  CHAR_RBRAC,                    // ADDED FOR EXAMINATION
  CHAR_UNKNOWN
} CharCode;

CharCode charCodes[256] = {
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_SPACE, CHAR_EXCLAMATION, CHAR_DOUBLEQUOTE, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_SINGLEQUOTE,   // ADDED FOR EXAM: DOUBLEQUOTE
  CHAR_LPAR, CHAR_RPAR, CHAR_TIMES, CHAR_PLUS, CHAR_COMMA, CHAR_MINUS, CHAR_PERIOD, CHAR_SLASH,
  CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT,
  CHAR_DIGIT, CHAR_DIGIT, CHAR_COLON, CHAR_SEMICOLON, CHAR_LT, CHAR_EQ, CHAR_GT, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LBRAC, CHAR_UNKNOWN, CHAR_RBRAC, CHAR_UNKNOWN, CHAR_UNKNOWN,                    // ADDED FOR EXAM: LBRAC, RBRAC

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
#define MAX_STRING_LEN 256
#define KEYWORDS_COUNT 21

typedef enum {
  TK_NONE, TK_IDENT, TK_NUMBER, TK_CHAR, TK_STR, TK_EOF,              //   ADDED FOR EXAM: TK_STR

  KW_PROGRAM, KW_CONST, KW_TYPE, KW_VAR,
  KW_INTEGER, KW_CHAR, KW_STRING, KW_ARRAY, KW_OF,                    //   ADDED FOR EXAM: KW_STRING 
  KW_FUNCTION, KW_PROCEDURE,
  KW_BEGIN, KW_END, KW_CALL,
  KW_IF, KW_THEN, KW_ELSE,
  KW_WHILE, KW_DO, KW_FOR, KW_TO,

  SB_SEMICOLON, SB_COLON, SB_PERIOD, SB_COMMA,
  SB_ASSIGN, SB_EQ, SB_NEQ, SB_LT, SB_LE, SB_GT, SB_GE,
  SB_PLUS, SB_MINUS, SB_TIMES, SB_SLASH,
  SB_LPAR, SB_RPAR, SB_LSEL, SB_RSEL, SB_LBRAC, SB_RBRAC              //   ADDED FOR EXAM: SB_LBRAC, SB_RBRAC
} TokenType; 

typedef struct {
  char string[MAX_IDENT_LEN + 1];
  int lineNo, colNo;
  TokenType tokenType;
  union {
    int intValue;
    char charValue;
    char *stringValue;
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
  {"STRING", KW_STRING},
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

Token *currentToken;
Token *lookAhead;

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

void freeToken(Token* tk) {
  if (tk == NULL)
    return;
  if (tk->tokenType == TK_STR)
    free(tk->stringValue);
  free(tk);
}

char *tokenToString(TokenType tokenType) {
  switch (tokenType) {
  case TK_NONE: return "None";
  case TK_IDENT: return "an identification";
  case TK_NUMBER: return "a number";
  case TK_CHAR: return "a constant char";
  case TK_STR: return "a constant string";             //  ADDED FOR EXAM
  case TK_EOF: return "end of file";

  case KW_PROGRAM: return "keyword PROGRAM";
  case KW_CONST: return "keyword CONST";
  case KW_TYPE: return "keyword TYPE";
  case KW_VAR: return "keyword VAR";
  case KW_INTEGER: return "keyword INTEGER";
  case KW_CHAR: return "keyword CHAR";
  case KW_STRING: return "keyword STRING";               //  ADDED FOR EXAM
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
  case SB_LBRAC: return "\'[\'";                       //  ADDED FOR EXAM
  case SB_RBRAC: return "\']\'";                       //  ADDED FOR EXAM
  default: return "";
  }
}

/* Error */

typedef enum {
  ERR_END_OF_COMMENT,
  ERR_IDENT_TOO_LONG,
  ERR_NUMBER_TOO_LONG,
  ERR_INVALID_CONSTANT_CHAR,
  ERR_INVALID_CONSTANT_STRING,
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
  ERR_INVALID_RETURN,
  ERR_UNDECLARED_FUNCTION,
  ERR_UNDECLARED_PROCEDURE,
  ERR_DUPLICATE_IDENT,
  ERR_TYPE_INCONSISTENCY,
  ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY
} ErrorCode;

#define NUM_OF_ERRORS 32

struct ErrorMessage {
  ErrorCode errorCode;
  char *message;
};

struct ErrorMessage errors[NUM_OF_ERRORS] = {
  {ERR_END_OF_COMMENT, "End of comment expected."},
  {ERR_IDENT_TOO_LONG, "Identifier too long."},
  {ERR_NUMBER_TOO_LONG,"Value of integer number exceeds the range!"},
  {ERR_INVALID_CONSTANT_CHAR, "Invalid char constant."},
  {ERR_INVALID_CONSTANT_STRING, "Invalid string constant."},               // ADDED FOR EXAM
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
  {ERR_INVALID_RETURN, "Expect the owner of the current scope."},
  {ERR_UNDECLARED_PROCEDURE, "Undeclared procedure."},
  {ERR_UNDECLARED_FUNCTION, "Undeclared function."},
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

/* Scanner */

int state;
int ln, cn, len;
TokenType tkType;
char str[MAX_STRING_LEN];
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
	  case CHAR_DOUBLEQUOTE: 
	    state = 44;
	    break;
	  case CHAR_LBRAC: 
	    state = 48;
	    break;
	  case CHAR_RBRAC: 
	    state = 49;
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
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) state = 14;
      else state = 15; 
      return getToken();
    case 14:
      readChar();
      return makeToken(SB_LE, ln, cn);
    case 15:
      return makeToken(SB_LT, ln, cn);
    case 16:
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) state = 17;
      else state = 18; 
      return getToken();
    case 17:
      readChar();
      return makeToken(SB_GE, ln, cn);
    case 18:
      return makeToken(SB_GT, ln, cn);
    case 19:
      token = makeToken(SB_EQ, lineNo, colNo); 
      readChar();
      return token;
    case 20:
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) state = 21;
      else state = 22; 
      return getToken();
    case 21:
      readChar();
      return makeToken(SB_NEQ, ln, cn);
    case 22:
      error(ERR_INVALID_SYMBOL, ln, cn);
    case 23:
      token = makeToken(SB_COMMA, lineNo, colNo);
      readChar();
      return token;
    case 24:
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_RPAR) state = 25;
      else state = 26; 
      return getToken();
    case 25:
      readChar();
      return makeToken(SB_RSEL, ln, cn);
    case 26:
      return makeToken(SB_PERIOD, ln, cn);
    case 27:
      token = makeToken(SB_SEMICOLON, lineNo, colNo);
      readChar();
      return token;
    case 28:
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) state = 29;
      else state = 30; 
      return getToken();
    case 29:
      readChar();
      return makeToken(SB_ASSIGN, ln, cn);
    case 30:
      return makeToken(SB_COLON, ln, cn);
    case 31:
      ln = lineNo;
      cn = colNo;
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
      token = makeToken(TK_CHAR, ln, cn);
      strcpy(token->string, str);
      token->charValue = str[0];
      readChar();
      return token;
    case 34:
      error(ERR_INVALID_CONSTANT_CHAR, ln, cn);
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
      return makeToken(SB_LSEL, ln, cn);
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
      token = makeToken(SB_RPAR, lineNo, colNo);
      readChar();
      return token;
    case 43:
      error(ERR_INVALID_SYMBOL, lineNo, colNo);
    case 44:        // ADDED FOR EXAM: read a double quote
      // TODO
     ln = lineNo;
      cn = colNo;
      len = 0;        
      readChar();     


      while (currentChar != EOF && charCodes[currentChar] != CHAR_DOUBLEQUOTE) {
        if (len < MAX_STRING_LEN) {
           str[len++] = currentChar; 
        }
        readChar();   
      }
      
      str[len] = '\0'; 
      if (currentChar == EOF) {
        state = 47;   
      } else {
        state = 46;
      }
      return getToken();
    case 45:     
      // TODO
    state = 47; 
    return getToken();
    case 46:      
      // TODO
    token = makeToken(TK_STR, ln, cn);
      token->stringValue = strdup(str); 
      readChar(); 
      return token;
    case 47:      // ADDED FOR EXAM: invalid constant string
      // TODO
      error(ERR_INVALID_CONSTANT_STRING, ln, cn);
      break;
    case 48:      // ADDED FOR EXAM: SB_LBRAC
      // TODO
      token = makeToken(SB_LBRAC, lineNo, colNo);
      readChar();
      return token;
    case 49:      // ADDED FOR EXAM: SB_RBRAC
      // TODO
      token = makeToken(SB_RBRAC, lineNo, colNo);
      readChar();
      return token;
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
  case TK_STR: printf("TK_STR(\'%s\')\n", token->stringValue); break;   // ADDED FOR EXAM
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_STRING: printf("KW_STRING\n"); break;                // ADDED FOR EXAM
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
  case SB_LBRAC: printf("SB_LBRAC\n"); break;    // ADDED FOR EXAM
  case SB_RBRAC: printf("SB_RBRAC\n"); break;    // ADDED FOR EXAM
  }
}

/******************************************************************/

Token* getValidToken(void) {

  state=0;
  Token *token = getToken();
  
  while (token->tokenType == TK_NONE) {
    freeToken(token);
    state = 0;
    token = getToken();
  }
  return token;
}


/* Symtab */
enum TypeClass {
  TP_INT,
  TP_CHAR,
  TP_STRING,                                     // ADDED FOR EXAM
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
    char *stringValue;                    // ADDED FOR EXAM
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
};

struct TypeAttributes_ {
  Type *actualType;
};

struct ProcedureAttributes_ {
  struct ObjectNode_ *paramList;
  struct Scope_* scope;
};

struct FunctionAttributes_ {
  struct ObjectNode_ *paramList;
  Type* returnType;
  struct Scope_ *scope;
};

struct ProgramAttributes_ {
  struct Scope_ *scope;
};

struct ParameterAttributes_ {
  enum ParamKind kind;
  Type* type;
  struct Object_ *function;
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
};

typedef struct Scope_ Scope;

struct SymTab_ {
  Object* program;
  Scope* currentScope;
  ObjectNode *globalObjectList;
};

typedef struct SymTab_ SymTab;

SymTab* symtab;
Type* intType;
Type* charType;
Type* stringType;     // ADDED FOR EXAM

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

// ADDED FOR EXAM
Type* makeStringType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_STRING;
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
  case TP_STRING:
    free(type);
    break;
  case TP_ARRAY:
    freeType(type->elementType);
    free(type);
    break;
  }
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

// ADDED FOR EXAM
ConstantValue* makeStringConstant(char* s) {
  ConstantValue* value = (ConstantValue*) malloc(sizeof(ConstantValue));
  value->type = TP_STRING;
  value->stringValue = strdup(s);
  return value;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  ConstantValue* value = (ConstantValue*) malloc(sizeof(ConstantValue));
  value->type = v->type;
  switch (v->type) {
  case TP_INT:
    value->intValue = v->intValue;
    break;
  case TP_CHAR:
    value->charValue = v->charValue;
    break;
  case TP_STRING:               // ADDED FOR EXAM
    value->stringValue = strdup(v->stringValue);
    break;
  }
  return value;
}

// ADDED FOR EXAM
void freeConstantValue(ConstantValue* v) {
  if (v->type == TP_STRING)
    free(v->stringValue);
  free(v);
}

/******************* Object utilities ******************************/


Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
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
  obj->varAttrs->scope = symtab->currentScope;
  return obj;
}

Object* createFunctionObject(char *name) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_FUNCTION;
  obj->funcAttrs = (FunctionAttributes*) malloc(sizeof(FunctionAttributes));
  obj->funcAttrs->paramList = NULL;
  obj->funcAttrs->scope = createScope(obj, symtab->currentScope);
  return obj;
}

Object* createProcedureObject(char *name) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PROCEDURE;
  obj->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));
  obj->procAttrs->paramList = NULL;
  obj->procAttrs->scope = createScope(obj, symtab->currentScope);
  return obj;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  Object* obj = (Object*) malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PARAMETER;
  obj->paramAttrs = (ParameterAttributes*) malloc(sizeof(ParameterAttributes));
  obj->paramAttrs->kind = kind;
  obj->paramAttrs->function = owner;
  return obj;
}

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

void freeObject(Object* obj) {
  switch (obj->kind) {
  case OBJ_CONSTANT:
    freeConstantValue(obj->constAttrs->value);
    free(obj->constAttrs);
    break;
  case OBJ_TYPE:
    free(obj->typeAttrs->actualType);
    free(obj->typeAttrs);
    break;
  case OBJ_VARIABLE:
    freeType(obj->varAttrs->type);
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

void freeReferenceList(ObjectNode *objList) {
  ObjectNode* list = objList;

  while (list != NULL) {
    ObjectNode* node = list;
    list = list->next;
    free(node);
  }
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

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  // ADDED FOR EXAM: 
  // Functions:
  //      LEN(s: string) : integer
  //      READS : string
  //      CHR(i: integer) : char
  //      ORD(ch: char) : integer
  //      SUBSTR(s : string, i : integer, l : integer) : string
  //      STR(ch : char) : string
  // Procedures:
  //      WRITES(s : string)

  // TODO
 
  obj = createFunctionObject("LEN");
  obj->funcAttrs->returnType = makeIntType();
  param = createParameterObject("s", PARAM_VALUE, obj);
  param->paramAttrs->type = makeStringType();
  addObject(&(obj->funcAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READS");
  obj->funcAttrs->returnType = makeStringType();
  addObject(&(symtab->globalObjectList), obj);
  
  obj = createFunctionObject("CHR");
  obj->funcAttrs->returnType = makeCharType();
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->funcAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);
  
  obj = createFunctionObject("ORD");
  obj->funcAttrs->returnType = makeIntType();
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->funcAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("SUBSTR"); 
  obj->funcAttrs->returnType = makeStringType();
  param = createParameterObject("s", PARAM_VALUE, obj);
  param->paramAttrs->type = makeStringType();
  addObject(&(obj->funcAttrs->paramList), param);
  
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->funcAttrs->paramList), param);
  
  param = createParameterObject("l", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->funcAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("STR");
  obj->funcAttrs->returnType = makeStringType();
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->funcAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITES");
  param = createParameterObject("s", PARAM_VALUE, obj);
  param->paramAttrs->type = makeStringType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
  stringType = makeStringType();    // ADDED FOR EXAM
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
  freeType(stringType);          // ADDED FOR EXAM
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  addObject(&(symtab->currentScope->objList), obj);
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
  if (obj == NULL)
    error(ERR_UNDECLARED_IDENT,currentToken->lineNo, currentToken->colNo);

  switch (obj->kind) {
  case OBJ_VARIABLE:
  case OBJ_PARAMETER:
    break;
  case OBJ_FUNCTION:
    if (obj != symtab->currentScope->owner) 
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

void checkNumberType(Type* type) {
  if ((type != NULL) && (type->typeClass == TP_INT))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

void checkCharType(Type* type) {
  if ((type != NULL) && (type->typeClass == TP_CHAR))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

void checkIterableType(Type* type) {
  if ((type != NULL) && ((type->typeClass == TP_CHAR) || (type->typeClass == TP_INT)))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

// ADDED FOR EXAM
void checkAddibleType(Type* type) {
  if ((type != NULL) && ((type->typeClass == TP_STRING) || (type->typeClass == TP_INT)))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

// ADDED FOR EXAM
void checkStringType(Type* type) {
  if ((type != NULL) && (type->typeClass == TP_STRING))
    return;
  else error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

void checkBasicType(Type* type) {
  if ((type != NULL) &&
      ((type->typeClass == TP_INT) ||
       (type->typeClass == TP_CHAR) ||
       (type->typeClass == TP_STRING)))   // ADDED FOR EXAM
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

void checkNumberConstant(ConstantValue* v) {
  if (v->type != TP_INT)
    error(ERR_TYPE_INCONSISTENCY, currentToken->lineNo, currentToken->colNo);
}

/* printing */
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
  case TP_STRING:              // ADDED FOR EXAM
    printf("String");
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
  case TP_STRING:              // ADDED FOR EXAM
    printf("\"%s\"",value->stringValue);
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
    break;
  case OBJ_PARAMETER:
    pad(indent);
    if (obj->paramAttrs->kind == PARAM_VALUE) 
      printf("Param %s : ", obj->name);
    else
      printf("Param VAR %s : ", obj->name);
    printType(obj->paramAttrs->type);
    break;
  case OBJ_FUNCTION:
    pad(indent);
    printf("Function %s : ",obj->name);
    printType(obj->funcAttrs->returnType);
    printf("\n");
    printScope(obj->funcAttrs->scope, indent + 4);
    break;
  case OBJ_PROCEDURE:
    pad(indent);
    printf("Procedure %s\n",obj->name);
    printScope(obj->procAttrs->scope, indent + 4);
    break;
  case OBJ_PROGRAM:
    pad(indent);
    printf("Program %s\n",obj->name);
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
Type* compileExpression3(void);
Type* compileTerm(void);
Type* compileTerm2(void);
Type* compileFactor(void);
Type* compileIndexes(Type* arrayType);
Type* compileStringAccess(void);

int compile();

/* parser */

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  freeToken(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    // printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  Object* program;

  eat(KW_PROGRAM);
  eat(TK_IDENT);

  program = createProgramObject(currentToken->string);
  enterBlock(program->progAttrs->scope);

  eat(SB_SEMICOLON);

  compileBlock();
  eat(SB_PERIOD);

  exitBlock();
}

void compileBlock(void) {
  Object* constObj;
  ConstantValue* constValue;

  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);

    do {
      eat(TK_IDENT);
      
      checkFreshIdent(currentToken->string);
      constObj = createConstantObject(currentToken->string);
      
      eat(SB_EQ);
      constValue = compileConstant();
      
      constObj->constAttrs->value = constValue;
      declareObject(constObj);
      
      eat(SB_SEMICOLON);
    } while (lookAhead->tokenType == TK_IDENT);

    compileBlock2();
  } 
  else compileBlock2();
}

void compileBlock2(void) {
  Object* typeObj;
  Type* actualType;

  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);

    do {
      eat(TK_IDENT);
      
      checkFreshIdent(currentToken->string);
      typeObj = createTypeObject(currentToken->string);
      
      eat(SB_EQ);
      actualType = compileType();
      
      typeObj->typeAttrs->actualType = actualType;
      declareObject(typeObj);
      
      eat(SB_SEMICOLON);
    } while (lookAhead->tokenType == TK_IDENT);

    compileBlock3();
  } 
  else compileBlock3();
}

void compileBlock3(void) {
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
  default:
    printf("adasdasdasd\n");
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
    checkNumberConstant(constValue);
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    constValue = compileConstant2();
    checkNumberConstant(constValue);
    if (constValue->type == TP_INT)
      constValue->intValue = - constValue->intValue;
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    constValue = makeCharConstant(currentToken->string[0]);
    break;
  case TK_STR:             // ADDED FOR EXAM
    // TODO
    eat(TK_STR);
    constValue = makeStringConstant(currentToken->stringValue);
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
    constValue = duplicateConstantValue(obj->constAttrs->value);
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
  ConstantValue* constValue; 

  switch (lookAhead->tokenType) {
  case KW_INTEGER: 
    eat(KW_INTEGER);
    type =  makeIntType();
    break;
  case KW_CHAR: 
    eat(KW_CHAR); 
    type = makeCharType();
    break;
  case KW_STRING:               // ADDED FOR EXAM
    // TODO
    eat(KW_STRING);
    type = makeStringType();
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    constValue = compileUnsignedConstant();
    arraySize = constValue->intValue;
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
  Object* obj;

  switch (lookAhead->tokenType) {
  case KW_INTEGER: 
    eat(KW_INTEGER); 
    type = makeIntType();
    break;
  case KW_CHAR: 
    eat(KW_CHAR); 
    type = makeCharType();
    break;
  case KW_STRING:                   // ADDED FOR EXAM
    // TODO
    eat(KW_STRING);
    type = makeStringType();
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    obj = checkDeclaredType(currentToken->string);
    if (obj->typeAttrs->actualType->typeClass == TP_ARRAY)
      error(ERR_INVALID_BASICTYPE, currentToken->lineNo, currentToken->colNo);
    type = duplicateType(obj->typeAttrs->actualType);
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

  switch (lookAhead->tokenType) {
  case TK_IDENT:
    eat(TK_IDENT);
    checkFreshIdent(currentToken->string);
    param = createParameterObject(currentToken->string, PARAM_VALUE, symtab->currentScope->owner);
    eat(SB_COLON);
    type = compileBasicType();
    param->paramAttrs->type = type;
    declareObject(param);
    break;
  case KW_VAR:
    eat(KW_VAR);
    eat(TK_IDENT);
    checkFreshIdent(currentToken->string);
    param = createParameterObject(currentToken->string, PARAM_REFERENCE, symtab->currentScope->owner);
    eat(SB_COLON);
    type = compileBasicType();
    param->paramAttrs->type = type;
    declareObject(param);
    break;
  default:
    error(ERR_INVALID_PARAMETER, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
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
  // TODO : element of string may appear as a lvalue
  // example:   s[i] := 'a';
   Type* type;
  Object* obj;

  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    type = intType;
    break;
  case TK_STR:            // ADDED FOR EXAM
    eat(TK_STR);
    type = stringType;
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    type = charType;
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    obj = checkDeclaredIdent(currentToken->string);
    switch (obj->kind) {
    case OBJ_CONSTANT:
      switch (obj->constAttrs->value->type) {
      case TP_INT:
        type = intType;
        break;
      case TP_CHAR:
        type = charType;
        break;
      case TP_STRING:           
        type = stringType;
        break;
      default:
        break;
      }
      break;
    case OBJ_VARIABLE:
      if (obj->varAttrs->type->typeClass == TP_ARRAY)
        type = compileIndexes(obj->varAttrs->type);
      else if (obj->varAttrs->type->typeClass == TP_STRING) {
        if (lookAhead->tokenType == SB_LBRAC) {
            type = compileStringAccess();
        } else {
            type = obj->varAttrs->type;
        }
      }
      else 
        type = obj->varAttrs->type;
      break;
    case OBJ_PARAMETER:
      if (obj->paramAttrs->type->typeClass == TP_STRING) {
        if (lookAhead->tokenType == SB_LBRAC) {
          type = compileStringAccess();
        } else {
          type = obj->paramAttrs->type;
        }
      } else {
        type = obj->paramAttrs->type;
      }
      break;
    case OBJ_FUNCTION:
      compileArguments(obj->funcAttrs->paramList);
      type = obj->funcAttrs->returnType;
      break;
    default: 
      error(ERR_INVALID_FACTOR,currentToken->lineNo, currentToken->colNo);
      break;
    }
    break;
  default:
    error(ERR_INVALID_FACTOR, lookAhead->tokenType, lookAhead->colNo);
  }
  
  return type;
}

void compileAssignSt(void) {
  Type* varType;
  Type* expType;

  varType = compileLValue();
  
  eat(SB_ASSIGN);
  expType = compileExpression();

  checkTypeEquality(varType, expType);
}

void compileCallSt(void) {
  Object* proc;

  eat(KW_CALL);
  eat(TK_IDENT);

  proc = checkDeclaredProcedure(currentToken->string);

  compileArguments(proc->procAttrs->paramList);
}

void compileGroupSt(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileIfSt(void) {
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
}

void compileForSt(void) {
  Object* var;
  Type *type1;
  Type *type2;

  eat(KW_FOR);
  
  type1 = compileLValue();
  checkIterableType(type1);

  eat(SB_ASSIGN);
  type2 = compileExpression();
  checkTypeEquality(type1, type2);

  eat(KW_TO);
  type2 = compileExpression();
  checkTypeEquality(type1, type2);

  eat(KW_DO);
  compileStatement();
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
  case SB_RBRAC:
  case SB_SEMICOLON:
  case KW_END:
  case SB_ASSIGN:
  case KW_ELSE:
  case KW_THEN:
    break;
  default:
    error(ERR_INVALID_ARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileCondition(void) {
  Type* type1;
  Type* type2;

  type1 = compileExpression();
  checkBasicType(type1);

  switch (lookAhead->tokenType) {
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
}

Type* compileExpression(void) {
  Type* type;
  
  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    type = compileExpression2();
    checkNumberType(type);
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    type = compileExpression2();
    checkNumberType(type);
    break;
  default:
    type = compileExpression2();
  }
  return type;
}

Type* compileExpression2(void) {
  Type* type1;
  Type* type2;

  type1 = compileTerm();
  type2 = compileExpression3();

  if (type2 != NULL)
    // make sure 2 operands of PLUS and MINUS have the same type
    checkTypeEquality(type1, type2);

  return type1;
}


Type* compileExpression3(void) {
  // TODO : support operation + for string
  
  // example: s := s + "Hanoi";
  Type* type1;
  Type* type2;

  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    type1 = compileTerm();
    //checkNumberType(type1);
    checkAddibleType(type1);      // ADDED FOR EXAM
    type2 = compileExpression3();
    if (type2 != NULL)
      checkTypeEquality(type1, type2);
    return type1;
  case SB_MINUS:
    eat(SB_MINUS);
    type1 = compileTerm();
    checkNumberType(type1);
    type2 = compileExpression3();
    if (type2 != NULL)
      // make sure 2 operands of PLUS and MINUS have the same type
      checkTypeEquality(type1, type2);
    return type1;
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
  case SB_RBRAC:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
    return NULL;
    break;
  default:
    error(ERR_INVALID_EXPRESSION, lookAhead->lineNo, lookAhead->colNo);
  }
}

Type* compileTerm(void) {
  Type* type1;
  Type* type2;

  type1 = compileFactor();
  type2 = compileTerm2();
  if (type2 != NULL)
    //make sure 2 operands of TIMES and SLASH have the same type
    checkTypeEquality(type1, type2);
  return type1;
}

Type* compileTerm2(void) {
  Type* type1;
  Type* type2;

  switch (lookAhead->tokenType) {
  case SB_TIMES:
    eat(SB_TIMES);
    type1 = compileFactor();
    checkNumberType(type1);
    type2 = compileTerm2();
    if (type2 != NULL)
      checkTypeEquality(type1, type2);
    return type1;
  case SB_SLASH:
    eat(SB_SLASH);
    type1 = compileFactor();
    checkNumberType(type1);
    type2 = compileTerm2();
    if (type2 != NULL)
      checkTypeEquality(type1, type2);
    return type1;
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
  case SB_RBRAC:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
    return NULL;
  default:
    error(ERR_INVALID_TERM, lookAhead->lineNo, lookAhead->colNo);
  }
  return NULL;
}


Type* compileFactor(void) {
  // TODO  : factor may be a string constant, an element of a string
  // example: var s : string; ..... ; s := "abc";
  // example: var s : string; ..... ; i := ord(s[1]) + 10;
  Type* type;
  Object* obj;

  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    type = intType;
    break;
  case TK_STR:            // ADDED FOR EXAM
    eat(TK_STR);
    type = stringType;
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    type = charType;
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    obj = checkDeclaredIdent(currentToken->string);
    switch (obj->kind) {
    case OBJ_CONSTANT:
      switch (obj->constAttrs->value->type) {
      case TP_INT:
        type = intType;
        break;
      case TP_CHAR:
        type = charType;
        break;
      case TP_STRING:           
        type = stringType;
        break;
      default:
        break;
      }
      break;
    case OBJ_VARIABLE:
      if (obj->varAttrs->type->typeClass == TP_ARRAY)
        type = compileIndexes(obj->varAttrs->type);
      else if (obj->varAttrs->type->typeClass == TP_STRING) {
        if (lookAhead->tokenType == SB_LBRAC) {
            type = compileStringAccess(); 
        } else {
            type = obj->varAttrs->type; 
        }
      }
      else 
        type = obj->varAttrs->type;
      break;
    case OBJ_PARAMETER:
            if (obj->paramAttrs->type->typeClass == TP_STRING) {
          if (lookAhead->tokenType == SB_LBRAC) {
            type = compileStringAccess();
          } else {
            type = obj->paramAttrs->type;
          }
        } else {
          type = obj->paramAttrs->type;
        }
        break;
    case OBJ_FUNCTION:
      compileArguments(obj->funcAttrs->paramList);
      type = obj->funcAttrs->returnType;
      break;
    default: 
      error(ERR_INVALID_FACTOR,currentToken->lineNo, currentToken->colNo);
      break;
    }
    break;
  default:
    error(ERR_INVALID_FACTOR, lookAhead->lineNo, lookAhead->colNo);
  }
  
  return type;
}

Type* compileIndexes(Type* arrayType) {
  Type* type;

  while (lookAhead->tokenType == SB_LSEL) {
    eat(SB_LSEL);
    type = compileExpression();
    checkIntType(type);
    checkArrayType(arrayType);
    arrayType = arrayType->elementType;
    eat(SB_RSEL);
  }
  checkBasicType(arrayType);
  return arrayType;
}

// ADDED FOR EXAM : compile string index
Type* compileStringAccess() {
  Type* type;
  eat(SB_LBRAC);
  type = compileExpression();
  checkIntType(type);
  eat(SB_RBRAC);
  return charType;
}

int compile() {
  lineNo = 1;
  colNo = 0;
  readChar();
  currentToken = NULL;
  lookAhead = getValidToken();

  initSymTab();

  compileProgram();

  printObject(symtab->program,0);

  cleanSymTab();

  freeToken(currentToken);
  freeToken(lookAhead);
  return IO_SUCCESS;

}



/* main */
int main(int argc, char *argv[]) {
  compile();    
  return 0;
}
