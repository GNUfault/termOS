#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#include <termios.h>
#include <stdio.h>
#include <sys/mount.h>

void putsc(const char *text, int r, int g, int b) {
  printf("\033[38;2;%d;%d;%dm%s\033[0m", r, g, b, text);
}

void sigchld_handler(int sig) {
  (void)sig;
  int status;
  pid_t pid;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    printf("Ended process %d with status %d\n", pid, status);
  }
}

struct termios orig_term;

void restore_terminal() {
  // restore terminal to original state
  tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
}

void setup_terminal() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &orig_term); // save original
  atexit(restore_terminal);            // restore on exit

  t = orig_term;
  t.c_lflag |= (ICANON | ECHO);       // enable canonical mode & echo
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int main() {
  char exePath[512] = {0};
  ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
  exePath[len] = '\0';   // null-terminate

  // strip filename → get directory
  if (mount(NULL, "/", NULL, MS_REMOUNT, NULL) < 0) {
    perror("remount / rw");
  }
  system("clear");
  char *lastSlash = strrchr(exePath, '/');
  if (lastSlash) {
    *lastSlash = '\0';
    chdir(exePath);
    printf("[termOS] Working directory: %s\n", exePath);
  }
  putsc("**red test**\n", 255, 0, 0);
  putsc("**green test**\n", 0, 255, 0);
  putsc("**blue test**\n", 0, 0, 255);

  puts("  /\\--=--/\\");
  puts(" /  \\   /  \\");
  puts("|term|=|inal|");
  puts(" \\ O/   \\S /");
  puts("  \\/--=--\\/");
  puts("\nA Linux Subsystem\n");

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }
  
  char buf[256] = {0};

  while (1) {
    printf("termOS> ");
    fflush(stdout);
    
    if (!fgets(buf, sizeof(buf), stdin)) {
      puts("\nEOF, rebooting");
      break;
    }

    buf[strcspn(buf, "\n")] = 0;

    char *command = strtok(buf, " ");
    char *arg1 = strtok(NULL, " ");
    char *arg2 = strtok(NULL, " ");

    if (!command) {puts("Empty"); continue;}

    if (!strcmp(command, "exit")) {
      break;
    } else if (!strcmp(command, "q")) {
      break;
    } else if (!strcmp(command, "quit")) {
      break;
    } else if (!strcmp(command, "exsb")) {
      char execPath[512] = {0};
      snprintf(execPath, sizeof(execPath) - sizeof(char), "./sbin/%s", arg1);
      system(execPath);
    } else if (!strcmp(command, "rand")) {
      
    }
  }

  restore_terminal();
  reboot(LINUX_REBOOT_CMD_RESTART);
  while (1) {}
}