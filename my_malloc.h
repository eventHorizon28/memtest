#ifndef _MY_MALLOC_H
#define _MY_MALLOC_H

#define malloc(x) my_malloc((x), __FILE__, __LINE__)
#define free(x) my_free((x), __FILE__, __LINE__)

void* my_malloc(int num_bytes, char* file_name, int line_no);
void my_free(void* ptr, char* file_name, int line_no);

#endif
