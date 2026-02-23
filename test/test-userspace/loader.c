#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_DIR "/var/tmp/test_module"
#define MAX_PATH_LEN 256
#define MAX_FILENAME_LEN 128

static int is_positive_integer(const char *str) {
  if (!str || !*str)
    return 0;

  for (int i = 0; str[i]; i++) {
    if (str[i] < '0' || str[i] > '9')
      return 0;
  }
  return atoi(str) > 0;
}

static int is_valid_filename(const char *filename) {
  if (!filename || !*filename || strlen(filename) > MAX_FILENAME_LEN)
    return 0;

  return strchr(filename, '/') == NULL;
}

int main(int argc, char *argv[]) {
  const char *program_name = argv[0];

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <period>\n", program_name);
    fprintf(stderr, "  filename: only name of file without \"/\" symbols\n");
    fprintf(stderr, "  period:  positive number (> 0)\n");
    fprintf(stderr, "Example: %s test.log 5\n", program_name);
    return 1;
  }

  const char *filename = argv[1];
  const char *period_str = argv[2];

  if (!is_valid_filename(filename)) {
    fprintf(stderr,
            "Error: '%s' has incorrect symbols. Maybe it's a dir? (/)\n",
            filename);
    fprintf(stderr, "Use only the name of file: test.log, output.txt\n");
    return 1;
  }

  if (!is_positive_integer(period_str)) {
    fprintf(stderr, "Error: '%s' should be a positive!\n", period_str);
    return 1;
  }

  int period = atoi(period_str);

  if (mkdir(DEFAULT_DIR, 0755) == 0) {
    printf("Create dir: %s\n", DEFAULT_DIR);
  } else if (errno == EEXIST) {
    printf("This dir %s alredy exsist.\n", DEFAULT_DIR);
  } else {
    perror("Error: Error while creating a dir /var/tmp/test_module.");
    fprintf(stderr, "Do yourself: sudo mkdir -p %s\n", DEFAULT_DIR);
    return 1;
  }

  char cmd[MAX_PATH_LEN + 128];
  snprintf(cmd, sizeof(cmd),
           "insmod ./test-module/test_module.ko log_file_path=%s period=%d",
           filename, period);

  printf("\nLoading...\n");
  printf("File:     /var/tmp/test_module/%s\n", filename);
  printf("Period:   %d sec\n", period);
  printf("Command:  %s\n", cmd);

  int ret = system(cmd);
  if (ret != 0) {
    fprintf(stderr, "\nError while loading the test-module.\n");
    perror("insmod");
    return 1;
  }

  printf("\nFor check result:\n");
  printf("tail -f /var/tmp/test_module/%s\n", filename);
  printf("For stop module work:\n");
  printf("sudo rmmod test_module\n");

  return 0;
}
