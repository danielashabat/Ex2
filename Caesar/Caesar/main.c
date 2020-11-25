

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
	return 1;\
}


// Structs --------------------------------------------------------------------

////the purpose of this struct is to hold the necessary arguments to 'ThreadFunction'
typedef struct ThreadData {
	char input_path[MAX_PATH];// the path of the input file
	char output_path[MAX_PATH];// the path of the output file
	DWORD  start_point;//the statring point (by bytes) the thread need to decrypte/encrypte 
	DWORD end_point;//the ending point (by bytes) the thread need to decrypte/encrypte (include this bytes)
	int key; // the key for the decryption/encryption
} ThreadData;



// Function Declarations -------------------------------------------------------

//the purpose of this function is to create data for passsing the necessary arguments to 'ThreadFunction'
//it returns pointer to ThreadData that contains the relevant data per thread by it's index thread.
//if the function fails it returns NULL
ThreadData* CreateThreadData(int start_point,////the statring point (by bytes) the thread need to decrypte/encrypte
	int end_point,//the ending point (by bytes) the thread need to decrypte/encrypte (include this bytes)
	char input_path[],//input file path
	char output_path[], //output file path
	int key);// the key for the decryption/encryption

// Implementation -------------------------------------------------------

//command line: caesar.exe, input_file, key, number of threads
int main(int argc, char* argv[]) {
	int i = 0;
	int lines_per_thread = 0;
	char input_path[MAX_PATH];
	char output_path[MAX_PATH];
	HANDLE h_input_file;
	HANDLE h_output_file;
	DWORD dw_ret = 0;
	HANDLE* threads_handles = NULL; //pointer to threads handles array
	DWORD *thread_ids =NULL; ////pointer to threads ids array
	BOOL ret_val;
	DWORD start_point = 0, end_point = 0;
	

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
	if (dw_ret == 1 ){
		printf("ERROR:%d  set_file_size failed\n", GetLastError());
		return 1;
	}

	lines_per_thread= count_lines(h_input_file) /num_threads;// dividing all the lines in the file with the number of threads 

	threads_handles = (HANDLE*)malloc(sizeof(HANDLE)*num_threads);//creating array of handles in the size of num_threads
	CHECK_IF_ALLOCATION_FAILED(threads_handles);

	thread_ids = (DWORD*)malloc(num_threads * sizeof(DWORD));// creating array of DWORD in the size of num_threads
	CHECK_IF_ALLOCATION_FAILED(thread_ids)


	for (i = 0; i < num_threads; i++)
	{
		start_point = get_start_point();
		
		if (i < (num_threads-1))//check if it's not the last thread
			end_point = get_end_point(h_input_file, lines_per_thread);
		else
			end_point = GetFileSize(h_input_file, NULL);//the last thread reads until EOF

		ThreadData* data = CreateThreadData(start_point, end_point, input_path, output_path, key);
		printf( "startpoint: %ld, endpoint:%ld\n", data->start_point, data->end_point);
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
			printf("ERRROR: %d while attempt to create thread\n",	 GetLastError());
			return 1;
		}
	}// End of main thread creation loop.

	dw_ret = WaitForMultipleObjects(
		num_threads,  // number of objects in array
		threads_handles,     // array of objects
		TRUE,       // wait for all objects to be signaled
		5000);       // five-second wait

	// check for failure 
	if (dw_ret != WAIT_OBJECT_0) {
		printf("ERROR:%d while waiting to threads to finish\n", GetLastError());
		return 1;
	
	}

	ret_val=close_all_handles(h_input_file, h_output_file, threads_handles, num_threads);//closing all the handles
	if (ret_val == 1) {
		printf("ERROR:closing one of the handles handles failed!\n");
		return 1;
	}

	free(threads_handles);
	free(thread_ids);
	return 0;
}



ThreadData* CreateThreadData(DWORD start_point,//
							DWORD end_point,
							 char input_path[],//input file path
							char output_path[], //output file path
							int key)// the key for the decryption/encryption
				
{
	ThreadData *ptr_to_thread_data = NULL;
	ptr_to_thread_data = (ThreadData*)malloc(sizeof(ThreadData));
	if (ptr_to_thread_data == NULL) {
		printf("ERROR: allocation failed!\n");
		return NULL;
	}
	strcpy_s(ptr_to_thread_data->input_path, MAX_PATH, input_path);
	strcpy_s(ptr_to_thread_data->output_path, MAX_PATH, output_path);
	ptr_to_thread_data->start_point = start_point;
	ptr_to_thread_data->end_point =end_point;
	ptr_to_thread_data->key = key;
	return ptr_to_thread_data;
}
