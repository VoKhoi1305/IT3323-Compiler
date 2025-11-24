
/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * Fixed version
*/

#include <stdio.h>
// #include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;
extern int colNo;
extern int currentChar;
extern CharCode charCodes[];

int state;
int ln, cn;
char str[MAX_IDENT_LEN + 1];
char c;

/***************************************************************/

Token* getToken(void) 
{
  Token *token;
  int count;
  
  switch(state)
  {
    case 0:
      if (currentChar == EOF) {
        state = 1;
      } else {
        switch (charCodes[currentChar]) 
        {
          case CHAR_SPACE: 
            state = 2;
            break;
          case CHAR_LETTER: 
            ln = lineNo;
            cn = colNo;
            state = 3;
            break;
          case CHAR_DIGIT: 
            ln = lineNo;
            cn = colNo;
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
      }
      return getToken();
      
    case 1:
      return makeToken(TK_EOF, lineNo, colNo);
      
    case 2:
      while (currentChar != EOF && charCodes[currentChar] == CHAR_SPACE) {
        readChar();
      }
      state = 0;
      return getToken();
      
    // case 3: // Đọc identifier hoặc keyword
    //   count = 0;
    //   while (currentChar != EOF && 
    //         (charCodes[currentChar] == CHAR_LETTER || 
    //          charCodes[currentChar] == CHAR_DIGIT)) {
    //     if (count < MAX_IDENT_LEN) {
    //       str[count++] = (char)currentChar;
    //       readChar();
    //     } else if (count == MAX_IDENT_LEN) {
    //       error(ERR_IDENTTOOLONG, ln, cn);
    //       // Đọc hết phần thừa nhưng không lưu
    //       readChar();
    //       count++;
    //     } else {
    //       // Tiếp tục đọc nhưng không lưu
    //       readChar();
    //     }
    //   }
    //   str[count < MAX_IDENT_LEN ? count : MAX_IDENT_LEN] = '\0';
    //   state = 4;
    //   return getToken();
      case 3: // Đọc identifier hoặc keyword
  count = 0;
  while (currentChar != EOF && 
        (charCodes[currentChar] == CHAR_LETTER || 
         charCodes[currentChar] == CHAR_DIGIT)) {
    if (count < MAX_IDENT_LEN) {
      str[count++] = (char)currentChar;
    } else if (count == MAX_IDENT_LEN) {
     // error(ERR_IDENTTOOLONG, ln, cn);
        printf("%d-%d:%s\n", ln, cn, ERM_IDENTTOOLONG);exit(0);
      count++; 
    }
    readChar(); 
  }
  
  if (count > MAX_IDENT_LEN) {
    count = MAX_IDENT_LEN;
  }
  str[count] = '\0';
  state = 4;
  return getToken();

    case 4: // Kiểm tra keyword hay identifier
      {
        TokenType type = checkKeyword(str);
        if (type == TK_NONE) {
          state = 5; 
        } else {
          state = 6; 
        }
        return getToken();
      }
      
    case 5: // Trả về identifier
      token = makeToken(TK_IDENT, ln, cn);
      strncpy(token->string, str, MAX_IDENT_LEN);
     // strcpy(token->string, str); 
      token->string[MAX_IDENT_LEN] = '\0'; 
      state = 0;
      return token;
      
    case 6: // Trả về keyword
      token = makeToken(checkKeyword(str), ln, cn);
      state = 0;
      return token;
      
    // case 7: // Đọc số
    //   count = 0;
    //   while (currentChar != EOF && charCodes[currentChar] == CHAR_DIGIT) {
    //     if (count < MAX_IDENT_LEN) { 
    //       str[count++] = (char)currentChar;
    //       readChar();
    //     } else {
    //       // Đọc hết nhưng không lưu để tránh overflow
    //       readChar();
    //       count++;
    //     }
    //   }
    //   str[count < MAX_IDENT_LEN ? count : MAX_IDENT_LEN] = '\0';
      
    //   // Kiểm tra số có quá 10 chữ số không
    //   if (count > 10) {
    //     error(ERR_NUMBERTOOLONG, ln, cn);
    //   }
    //   state = 8;
    //   return getToken();

    case 7: // Đọc số
  count = 0;
  while (currentChar != EOF && charCodes[currentChar] == CHAR_DIGIT) {
    if (count < MAX_IDENT_LEN) {
      str[count++] = (char)currentChar;
    }
    readChar(); 
    if (count > MAX_IDENT_LEN) {
      continue;
    }
  }
  
  if (count > MAX_IDENT_LEN) {
    count = MAX_IDENT_LEN;
  }
  str[count] = '\0';
  
  // Kiểm tra độ dài
  if (count > 10 || (count == 10 && strcmp(str, "2147483647") > 0)) {
    // error(ERR_NUMBERTOOLONG, ln, cn);
        printf("%d-%d:%s\n", ln, cn, ERM_IDENTTOOLONG);exit(0);
  }
  state = 8;
  return getToken();
      
    case 8: // Trả về số
      token = makeToken(TK_NUMBER, ln, cn);
      strncpy(token->string, str, MAX_IDENT_LEN);
      // strcpy(token->string, str); 
      token->string[MAX_IDENT_LEN] = '\0'; // Đảm bảo null-terminated
      token->value = atoi(str);
      state = 0;
      return token;
      
    case 9: // +
      token = makeToken(SB_PLUS, lineNo, colNo);
      readChar();
      state = 0;
      return token;
      
    case 10: // -
      token = makeToken(SB_MINUS, lineNo, colNo);
      readChar();
      state = 0;
      return token;
      
    case 11: // *
      token = makeToken(SB_TIMES, lineNo, colNo);
      readChar();
      state = 0;
      return token;
      
    case 12: // /
      token = makeToken(SB_SLASH, lineNo, colNo);
      readChar();
      state = 0;
      return token;
      
    case 13: // < hoặc <=
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) {
        state = 14;
      } else {
        state = 15;
      }
      return getToken();
      
    case 14: // <=
      token = makeToken(SB_LE, ln, cn);
      readChar();
      state = 0;
      return token;
      
    case 15: // <
      token = makeToken(SB_LT, ln, cn);
      state = 0;
      return token;
      
    case 16: // > hoặc >=
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) {
        state = 17;
      } else {
        state = 18;
      }
      return getToken();
      
    case 17: // >=
      token = makeToken(SB_GE, ln, cn);
      readChar();
      state = 0;
      return token;
      
    case 18: // >
      token = makeToken(SB_GT, ln, cn);
      state = 0;
      return token;
      
    case 19: // =
      token = makeToken(SB_EQ, lineNo, colNo);
      readChar();
      state = 0;
      return token;
      
    case 20: // ! hoặc !=
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) {
        state = 21;
      } else {
        state = 22;
      }
      return getToken();
      
    case 21: // !=
      token = makeToken(SB_NEQ, ln, cn);
      readChar();
      state = 0;
      return token;
      
    case 22: // Lỗi: ! đơn lẻ
      token = makeToken(TK_NONE, ln, cn);
      // error(ERR_INVALIDSYMBOL, ln, cn);
        printf("%d-%d:%s\n", ln, cn, ERM_INVALIDSYMBOL);exit(0);
      state = 0;
      return token;
      
    case 23: // ,
      token = makeToken(SB_COMMA, lineNo, colNo);
      readChar();
      state = 0;
      return token;
      
    case 24: // . hoặc (.
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_LPAR) {
        state = 25;
      } else {
        state = 26;
      }
      return getToken();
      
    case 25: // .)
      token = makeToken(SB_RSEL, ln, cn);
      readChar();
      state = 0;
      return token;
      
    case 26: // .
      token = makeToken(SB_PERIOD, ln, cn);
      state = 0;
      return token;
      
    case 27: // ;
      token = makeToken(SB_SEMICOLON, lineNo, colNo);
      readChar();
      state = 0;
      return token;
      
    case 28: // : hoặc :=
      ln = lineNo;
      cn = colNo;
      readChar();
      if (charCodes[currentChar] == CHAR_EQ) {
        state = 29;
      } else {
        state = 30;
      }
      return getToken();
      
    case 29: // :=
      token = makeToken(SB_ASSIGN, ln, cn);
      readChar();
      state = 0;
      return token;
      
    case 30: // :
      token = makeToken(SB_COLON, ln, cn);
      state = 0;
      return token;
      
    case 31: // Bắt đầu đọc char constant
      ln = lineNo;
      cn = colNo;
      readChar();
      if (currentChar == EOF) {
        state = 34;
      } else if (isprint(currentChar)) {
        state = 32;
      } else {
        state = 34;
      }
      return getToken();
      
    case 32: // Đọc ký tự trong ''
      c = currentChar;
      readChar();
      if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {
        state = 33;
      } else {
        state = 34;
      }
      return getToken();
      
    case 33: // Kết thúc char constant
      token = makeToken(TK_CHAR, ln, cn);
      token->string[0] = c;
      token->string[1] = '\0';
      readChar();
      state = 0;
      return token;
      
    // case 34: // Lỗi char constant
    //   token = makeToken(TK_NONE, ln, cn);
    //   error(ERR_INVALIDCHARCONSTANT, ln, cn);
    //   state = 0;
    //   return token;
        case 34: // Lỗi char constant
      token = makeToken(TK_NONE, ln, cn);
      // error(ERR_INVALIDCHARCONSTANT, ln, cn);
        printf("%d-%d:%s\n", ln, cn, ERM_INVALIDCHARCONSTANT);exit(0);
      
      if (currentChar != EOF && charCodes[currentChar] != CHAR_SINGLEQUOTE) {
        readChar(); // Bỏ qua ký tự lỗi
      }
      if (currentChar != EOF && charCodes[currentChar] == CHAR_SINGLEQUOTE) {
        readChar(); // Bỏ qua dấu ' nếu có
      }
      state = 0;
      return token;
      
    case 35: // ( hoặc (. hoặc (*
      ln = lineNo;
      cn = colNo;
      readChar();
      if (currentChar == EOF) {
        state = 41;
      } else if (charCodes[currentChar] == CHAR_PERIOD) {
        state = 36;
      } else if (charCodes[currentChar] == CHAR_TIMES) {
        state = 37;
      } else {
        state = 41;
      }
      return getToken();
      
    case 36: // (.
      token = makeToken(SB_LSEL, ln, cn);
      readChar();
      state = 0;
      return token;
      
    case 37: // Bên trong comment, đọc ký tự thường
      readChar();
      if (currentChar == EOF) {
        state = 40;
      } else if (charCodes[currentChar] == CHAR_TIMES) {
        state = 38;
      } else {
        state = 37;
      }
      return getToken();
      
    case 38: // Gặp * trong comment
      readChar();
      if (currentChar == EOF) {
        state = 40;
      } else if (charCodes[currentChar] == CHAR_RPAR) {
        state = 39;
      } else if (charCodes[currentChar] == CHAR_TIMES) {
        state = 38;
      } else {
        state = 37;
      }
      return getToken();
      
    case 39: // Kết thúc comment
      readChar();
      state = 0;
      return getToken();
      
   case 40: // Lỗi: comment không đóng

    printf("%d-%d:%s\n", lineNo, colNo, ERM_ENDOFCOMMENT);exit(0);
  
  state = 1; 
  return getToken(); 
      
    case 41: // (
      token = makeToken(SB_LPAR, ln, cn);
      state = 0;
      return token;
      
    case 42: // )
      token = makeToken(SB_RPAR, lineNo, colNo);
      readChar();
      state = 0;
      return token;
      
    case 43: // Ký tự không hợp lệ
      token = makeToken(TK_NONE, lineNo, colNo);
      // error(ERR_INVALIDSYMBOL, lineNo, colNo);
        printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDSYMBOL);exit(0);
        exit(0);
      readChar();
      state = 0;
      return token;
  }
  
  return makeToken(TK_NONE, lineNo, colNo);
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

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  state = 0;
  
  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }

  // printf("=== ENTERED MAIN ===\n");  // ← THÊM DÒNG NÀY
  // fflush(stdout);
  
  // puts("Scanner Test:");
  // error(ERR_ENDOFCOMMENT, 1, 1);
  // puts("End of Test");
  
  return 0;
}