#ifndef File_hpp
#define File_hpp

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

namespace TSS {
unsigned long get_file_size(char *path);
unsigned long get_file_size_internal(FILE *file);

void write_file(char *path, void *data, unsigned long size);
void append_file(char *path, void *data, unsigned long size);

std::vector<std::string> read_file(char *path);
}  // namespace TSS

#endif
