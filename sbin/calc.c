
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  
  while (1) {
    char buf[256] = {0};
    printf("termOS/calc> ");
    fflush(stdout);
    
    if (!fgets(buf, sizeof(buf), stdin)) {
      puts("\nEOF, exiting");
      break;
    }

    buf[strcspn(buf, "\n")] = 0;

    char *arg1 = strtok(buf, " ");
    char *op = strtok(NULL, " ");
    char *arg3 = strtok(NULL, " ");

    if (!arg1) {puts("Empty"); continue;}

    if (!strcmp(arg1, "q")) {
      break;
    } else if (!strcmp(arg1, "quit")) {
      break;
    } else if (!strcmp(arg1, "exit")) {
      break;
    }

    if (!arg1 || !op || !arg3) {puts("Not enough arguments"); continue;}
    
    float a = atof(arg1);
    float b = atof(arg3);
    float c = 0;

    if (!strcmp(op, "+")) {
      c = a + b;
    } else if (!strcmp(op, "-")) {
      c = a - b;
    } else if (!strcmp(op, "*")) {
      c = a * b;
    } else if (!strcmp(op, "/")) {
      c = a / b;
    } else {
      printf("Unknown operator: %s\n", op);
    }

    printf("%f\n", c);
  }
  _exit(0);
}