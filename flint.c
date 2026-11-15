#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int command_finder(char *argv[]);
int common_cli(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  if (argc == 3 && strcmp(argv[1], "run") == 0) {
    return command_finder(argv);
  }

  return common_cli(argc, argv);
}

int command_finder(char *argv[]) {
  FILE *file = fopen(argv[2], "r");
  if (file == NULL) {
    fprintf(stderr, "Error opening file.");
    return 1;
  }
  int content;
  char word[50];
  int line = 1;
  int i = 0;
  char to_print[100];

  while ((content = fgetc(file)) != EOF) {
    if (content == '\n') {
      line++;
      continue;
    }

    if (isspace(content)) {
      continue;
    }

    word[i++] = content;

    while ((content = fgetc(file)) != EOF && isalpha(content)) {
      word[i++] = content;
    }

    if (content == '\n') {
      line++;
    }

    word[i++] = '\0';
    i = 0;

    if (strcmp(word, "print") == 0) {
      while (content != '"' && (content = fgetc(file)) != EOF) {
        if (content == '\n') {
          line++;
        }
      }

      while ((content = fgetc(file)) != EOF && content != '"') {
        to_print[i++] = content;
      }

      to_print[i++] = '\0';
      i = 0;

      printf("%s\n", to_print);
    } else if (strcmp(word, "repeat") == 0) {
      char num_str[10];

      while (content != '"' && !isdigit(content) &&
             (content = fgetc(file)) != EOF) {
        if (content == '\n') {
          line++;
        }
      }

      while (content != EOF && isdigit(content)) {
        num_str[i++] = content;
        content = fgetc(file);
      }
      num_str[i++] = '\0';

      if (strcmp(num_str, "") == 0) {
        printf("%s: line %i: Repeat number not specified.\n", argv[2], line);
        fclose(file);
        return 1;
      }

      i = 0;

      int count = atoi(num_str);
      if (count <= 0) {
        printf("%s: line %i: Repeat count can't be 0 or less.\n", argv[2],
               line);
        fclose(file);
        return 1;
      }

      while (content != '"' && (content = fgetc(file)) != EOF) {
        if (content == '\n') {
          line++;
        }
      }

      while ((content = fgetc(file)) != EOF && content != '"') {
        to_print[i++] = content;
      }

      to_print[i++] = '\0';
      i = 0;

      for (; i < count; i++) {
        printf("%s\n", to_print);
      }
      i = 0;
    } else if (strcmp(word, "add") == 0) {
      char num1_str[10];
      char num2_str[10];

      while (!isdigit(content) && (content = fgetc(file)) != EOF) {
        if (content == '\n') {
          line++;
        }
      }

      while (content != EOF && isdigit(content)) {
        num1_str[i++] = content;
        content = fgetc(file);
      }

      num1_str[i++] = '\0';
      i = 0;

      int num1 = atoi(num1_str);

      while (!isdigit(content) && (content = fgetc(file)) != EOF) {
        if (content == '\n') {
          line++;
        }
      }

      while (content != EOF && isdigit(content)) {
        num2_str[i++] = content;
        content = fgetc(file);
      }
      if (content == '\n') {
        line++;
      }

      num2_str[i++] = '\0';
      if (strcmp(num2_str, "") == 0) {
        printf("%s: line %i: Second number to be added not specified.\n",
               argv[2], line);
        fclose(file);
        return 1;
      }
      i = 0;

      int num2 = atoi(num2_str);

      printf("%i\n", num1 + num2);
    }
  }

  fclose(file);
  return 0;
}

int common_cli(int argc, char *argv[]) {
  char *cmd[] = {"new", "help", "version"};
  if (argc > 2) {
    printf("Excess options.\n");
    return 1;
  } else if (argc < 2) {
    printf("Missing arguments.\n");
    return 1;
  }

  if (strcmp(argv[1], cmd[0]) == 0) {
    FILE *file;
    file = fopen("main.fme", "wx");
    if (file == NULL) {
      fprintf(stderr, "Error creating workspace.\n");
      file = NULL;
      return 1;
    }

    fprintf(file, "print \"Hello, World\"\n");
    fputs("repeat 3 \"FLInt\"\n", file);
    fputs("add 14 75\n", file);

    fclose(file);
    file = NULL;
    return 0;
  } else if (strcmp(argv[1], cmd[1]) == 0) {
    printf("Available options: \n\tnew      -  create template file\n\thelp    "
           " -  get this message\n\tversion  -  see current version\n");
    return 0;
  } else if (strcmp(argv[1], cmd[2]) == 0) {
    printf("Frame Literal Interpreter v2026.11.14\n");
    return 0;
  } else {
    printf("drift: %s: command not found.\n", argv[1]);
    return 1;
  }
}
