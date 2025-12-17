

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  printf("Working directory: %s\n", getcwd(NULL, 0));

  char buf[2048] = {0};

  while (1) {
    printf("linTerm> ");
    fflush(stdout);
    
    if (!fgets(buf, sizeof(buf), stdin)) {
      puts("\nEOF, rebooting");
      break;
    }

    buf[strcspn(buf, "\n")] = '\0';

    if (!strcmp(buf, "q")) {
      break;
    } else if (!strcmp(buf, "quit")) {
      break;
    } else if (!strcmp(buf, "exit")) {
      break;
    }
    
    system(buf);
  }
  _exit(0);
}