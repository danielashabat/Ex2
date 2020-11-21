#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>

int text_size(FILE* fp) {
	int f_size;

	fseek(fp, 0, SEEK_END);
	f_size = ftell(fp);
	printf("text size: size=%ld", (unsigned long)f_size);
	return f_size;
}

//command line: caesar.exe, input_file, key, number of threads
int main(int argc, char* argv[]) {
	errno_t err;
	FILE* input_file=NULL;
	int thread_size = 0;
	HANDLE* threads_handles = NULL; //pointer to threads handles array
	DWORD *thread_ids; ////pointer to threads ids array

	////check if there 4 arguments
	//if (argc < 4) {
	//	printf("ERROR:not enough arguments!");
	//	return EXIT_FAILURE;
	//}
	//if (argc > 4) {
	//	printf("ERROR:too many arguments!");
	//	return EXIT_FAILURE;
	//}
	err = fopen_s(&input_file, argv[1], "r"); //open input file 
	int key = argv[2];
	int num_threads = argv[3];

	thread_size= text_size(input_file)/num_threads;//counting chars in input text and dividing it in the number of threads


	if (!err && input_file != NULL) {
		fclose(input_file);
	}
	return 0;
	
}


//19