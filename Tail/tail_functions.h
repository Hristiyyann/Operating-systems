#ifndef TAIL_FUNCTIONS_H
#define TAIL_FUNCTIONS_H

int is_file_directory(char *file);
void open_errors(char* file);
void read_error(char* file);
void write_error();
void print_file_names(char* file, int success);
void read_and_print_last_ten_lines(char* file_for_read, int argc);
void input_from_console(int argc);

#endif
