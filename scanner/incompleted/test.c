#include <stdio.h>
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


int main(int argc, char *argv[]) {

  printf("=== ENTERED MAIN ===\n");  // ← THÊM DÒNG NÀY
  fflush(stdout);
  
  puts("Scanner Test:");
  error(ERR_ENDOFCOMMENT, 1, 1);
  puts("End of Test");
  
  return 0;
}