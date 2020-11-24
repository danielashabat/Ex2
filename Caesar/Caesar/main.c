

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include "decryptor.h"
#include "files_functions.h"
#include "decryptor_thread.h"///removeeeeeeeeeeeeeeeee  this



// Defines --------------------------------------------------------------------

/*if allocation failed exit program*/
#define CHECK_IF_ALLOCATION_FAILED(RET_VAL) if (RET_VAL == NULL) {\
	printf("memory allocation failed\n");\
	exit(1);\
}


// Structs --------------------------------------------------------------------

////the purpose of this struct is to hold the necessary arguments to 'ThreadFunction'
typedef struct ThreadData {
	char input_path[MAX_PATH];// the path of the input file
	char output_path[MAX_PATH];// the path of the output file
	int  start_point;//the statring point (by bytes) the thread need to decrypte/encrypte 
	int end_point;//the ending point (by bytes) the thread need to decrypte/encrypte (include this bytes)
	int key; // the key for the decryption/encryption
} ThreadData;



// Function Declarations -------------------------------------------------------

//the purpose of this function is to create data for passsing the necessary arguments to 'ThreadFunction'
//it returns pointer to ThreadData that contains the relevant data per thread by it's index thread.
//if the function fails it returns NULL
ThreadData* CreateThreadData(int thread_index, //the thread index
	int lines_per_thread,  // the number of lines the thread need to read/write to file  
	char input_path[],//input file path
	char output_path[], //output file path
	int key,// the key for the decryption/encryption
	HANDLE h_input_file); //handle to input file

// Implementation -------------------------------------------------------

//command line: caesar.exe, input_file, key, number of threads
int main(int argc, char* argv[]) {
	int i = 0;
	int lines_per_thread = 0;
	char input_path[MAX_PATH];
	char output_path[MAX_PATH];
	HANDLE h_input_file;
	HANDLE h_output_file;
	DWORD input_file_size = 0;
	DWORD dw_ret = 0;
	HANDLE* threads_handles = NULL; //pointer to threads handles array
	DWORD *thread_ids =NULL; ////pointer to threads ids array
	BOOL ret_val;
	

	//check if there 4 arguments
	if (argc < 4) {
		printf("ERROR:not enough arguments!");
		return EXIT_FAILURE;
	}
	if (argc > 4) {
		printf("ERROR:too many arguments!");
		return EXIT_FAILURE;
	}
	
	strcpy_s(input_path,MAX_PATH,argv[1]);
	strcpy_s(output_path, MAX_PATH, "decrypted.txt");
	int key = atoi(argv[2]);
	int num_threads =atoi(argv[3]);

	h_input_file = CreateFileA(input_path,// file name 
		GENERIC_READ,          // open for reading 
		FILE_SHARE_READ,       // open sharing for reading only 
		NULL,                  // default security 
		OPEN_EXISTING,         // existing file only 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 


	h_output_file = CreateFileA(output_path,// file name 
		GENERIC_WRITE,          // open for reading 
		FILE_SHARE_WRITE,      // open sharing for writing only 
		NULL,                  // default security 
		CREATE_ALWAYS,         // always creats a new file, if the file exists it overwrites it 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 


	dw_ret= set_file_size(h_output_file, GetFileSize(h_input_file, NULL));
	/// check for failure
	lines_per_thread= count_lines(h_input_file) /num_threads;// dividing all the lines in the file with the number of threads 

	threads_handles = (HANDLE*)malloc(sizeof(HANDLE)*num_threads);//creating array of handles in the size of num_threads
	CHECK_IF_ALLOCATION_FAILED(threads_handles);

	thread_ids = (DWORD*)malloc(num_threads * sizeof(DWORD));// creating array of DWORD in the size of num_threads
	CHECK_IF_ALLOCATION_FAILED(thread_ids)


	for (i = 0; i < num_threads; i++)
	{
		ThreadData* data = CreateThreadData(i, lines_per_thread, input_path, output_path, key,h_input_file);
		printf( "startpoint: %d, endpoint:%d\n", data->start_point, data->end_point);
		CHECK_IF_ALLOCATION_FAILED(data);

		*(threads_handles+i) = CreateThread(/////need to handle with this warning!!
			NULL,                   // default security attributes
			0,                    // use default stack size  
			decryptor_thread,       // thread function name
			NULL,          // argument to thread function 
			0,                      // use default creation flags 
			&thread_ids[i]);   // returns the thread identifie


	// Check the return value for success.
	// If CreateThread fails, terminate execution. 
	// This will automatically clean up threads and memory. 

		if (*(threads_handles+i) == NULL)
		{
			printf("create thread failed\n");
			ExitProcess(3);
		}
	}// End of main thread creation loop.

	ret_val=close_all_handles(h_input_file, h_output_file, threads_handles, num_threads);//closing all the handles
	if (ret_val == 1) {
		printf("ERROR:closing one of the handles handles failed!\n");
		return 1;
	}

	free(threads_handles);
	free(thread_ids);
	return 0;
}



ThreadData* CreateThreadData(int thread_index, //the thread index
							 int lines_per_thread,  // the number of lines the thread need to read/write to file  
							 char input_path[],//input file path
							char output_path[], //output file path
							int key,// the key for the decryption/encryption
							HANDLE h_input_file) //handle to input file
{
	static int file_pointer=0;
	ThreadData *ptr_to_thread_data = NULL;
	ptr_to_thread_data = (ThreadData*)malloc(sizeof(ThreadData));
	if (ptr_to_thread_data == NULL) {
		printf("ERROR: allocation failed!\n");
		return NULL;
	}
	strcpy_s(ptr_to_thread_data->input_path, MAX_PATH, input_path);
	strcpy_s(ptr_to_thread_data->output_path, MAX_PATH, output_path);
	ptr_to_thread_data->start_point = file_pointer;
	file_pointer = adding_lines_to_file_pointer(h_input_file, lines_per_thread, file_pointer);
	ptr_to_thread_data->end_point = file_pointer;
	ptr_to_thread_data->key = key;

	file_pointer++;//moving the file pointer by one byte for the next thread's starting point
	return ptr_to_thread_data;
}
