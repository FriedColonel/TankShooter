#include <socket/File.hpp>

unsigned long TSS::get_file_size(char *path) {
  FILE *file = fopen(path, "r");
  fseek(file, 0, SEEK_END);
  unsigned long size = ftell(file);
  fclose(file);
  return size;
}

unsigned long TSS::get_file_size_internal(FILE *file) {
  fseek(file, 0, SEEK_END);
  unsigned long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

void TSS::write_file(char *path, void *data, unsigned long size) {
  FILE *file = fopen(path, "w+");
  if (file == NULL) {
    printf("Error: file not found %s\n", path);
    exit(EXIT_FAILURE);
  }
  fwrite(data, size, 1, file);
  fclose(file);
}

void TSS::append_file(char *path, void *data, unsigned long size) {
  FILE *file = fopen(path, "a+");
  if (file == NULL) {
    printf("Error: file not found %s\n", path);
    exit(EXIT_FAILURE);
  }
  fwrite(data, size, 1, file);
  fclose(file);
}

std::vector<std::string> TSS::read_file(char *path) {
  std::vector<std::string> lines;

  FILE *file = fopen(path, "r+");
  if (file == NULL) {
    printf("Error: file not found %s\n", path);
    return lines;
  }
  char *line = NULL;

  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, file)) != -1) {
    line[read - 1] = '\0';
    lines.push_back(line);
  }

  fclose(file);

  if (line) free(line);

  return lines;
}
