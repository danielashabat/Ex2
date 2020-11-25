

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <stdbool.h>
#include "decryptor.h"
#include "decryptor_thread.h"


// Defines --------------------------------------------------------------------

/*if allocation failed exit program*/
#define CHECK_IF_ALLOCATION_FAILED(RET_VAL) if (RET_VAL == NULL) {\
	printf("memory allocation failed\n");\
	exit(1);\
}

/*if closing handler failed exit program*/
#define CHECK_IF_CLOSING_HANDLER_FAILED(RET_VAL) if (false == RET_VAL) {\
	printf("Error when closing\n");\
	return 1;\
}

// Structs --------------------------------------------------------------------

////the purpose of this struct is to hold the necessary arguments to 'ThreadFunction'
typedef struct ThreadData {
	char input_path[MAX_PATH];// the path of the input file
	char output_path[MAX_PATH];// the path of the output file
	int  startline;//the statring line the thread need to decrypte/encrypte 
	int endline;//the ending line the thread need to decrypte/encrypte (include this line)
	int key; // the key for the decryption/encryption
} ThreadData;

// Function Declarations -------------------------------------------------------

//the purpose of this function is to create data for passsing the necessary arguments to 'ThreadFunction'
//it returns pointer to ThreadData that contains the relevant data per thread by it's index thread.
//if the function fails it returns NULL
ThreadData* CreateThreadData(int thread_index, //the thread index
	int thread_size,  // the size of bytes the thread need to read from input file 
	char input_path[],//input file path
	char output_path[], //output file path
	int key); // the key for the decryption/encryption

//this function counts the number of lines in the file
//returns an int that represent the number of lines in the given file
int count_lines(HANDLE hfile);//handler to file

/*this function setting the file size to new_size(in bytes)
the function return 'INVALID_SET_FILE_POINTER' if it fails, and '0' otherwise*/
DWORD set_file_size(HANDLE hfile,//handler to the file 
					DWORD new_size);// the new size of the file 

/*this function close all the handles that opens in the main
return 0 if all the handles closed, return 1 otherwise*/
int close_all_handles(HANDLE hInputFile, HANDLE hOutputFile, HANDLE threads_handles[], int num_threads);

// Implementation -------------------------------------------------------

//command line: caesar.exe, input_file, key, number of threads
int main(int argc, char* argv[]) {
	int i = 0;
	int thread_size = 0;
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
	if (argc < 5) {
		printf("ERROR:not enough arguments!");
		return EXIT_FAILURE;
	}
	if (argc > 5) {
		printf("ERROR:too many arguments!");
		return EXIT_FAILURE;
	}
	
	strcpy_s(input_path,MAX_PATH,argv[1]);
	strcpy_s(output_path, MAX_PATH, "decrypted.txt");
	int key = atoi(argv[2]);
	int num_threads =atoi(argv[3]);
	int enc_or_dec = argv[4];

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
	thread_size= count_lines(h_input_file) /num_threads;// dividing input text lines in the number of threads 

	threads_handles = (HANDLE*)malloc(sizeof(HANDLE)*num_threads);//creating array of handles in the size of num_threads
	CHECK_IF_ALLOCATION_FAILED(threads_handles);

	thread_ids = (DWORD*)malloc(num_threads * sizeof(DWORD));// creating array of DWORD in the size of num_threads
	CHECK_IF_ALLOCATION_FAILED(thread_ids)


	for (i = 0; i < num_threads; i++)
	{
		ThreadData* data = CreateThreadData(i, thread_size, argv[1], "decrypted.txt", key);
		printf("startline: %d, endline:%d\n", data->startline, data->endline);
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
							 int thread_size,  // the size of bytes the thread need to read from input file 
							 char input_path[],//input file path
							char output_path[], //output file path
							int key) // the key for the decryption/encryption
{
	ThreadData *ptr_to_thread_data = NULL;
	ptr_to_thread_data = (ThreadData*)malloc(sizeof(ThreadData));
	if (ptr_to_thread_data == NULL) {
		printf("ERROR: allocation failed!\n");
		return NULL;
	}
	strcpy_s(ptr_to_thread_data->input_path, MAX_PATH, input_path);
	strcpy_s(ptr_to_thread_data->output_path, MAX_PATH, output_path);
	ptr_to_thread_data->startline = (thread_size * thread_index);
	ptr_to_thread_data->endline = thread_size *(thread_index+1)-1;
	ptr_to_thread_data->key = key;
	return ptr_to_thread_data;
}

int count_lines(HANDLE hfile) {
	char char_buffer;
	DWORD nBytesToRead = 1;
	DWORD dwBytesRead = 0;
	DWORD dwFileSize = GetFileSize(hfile, NULL);
	BOOL bResult = FALSE;
	int count_lines = 0;
	unsigned int count_chars = 0;

	while (count_chars < dwFileSize) {
		bResult = ReadFile(hfile, &char_buffer, nBytesToRead, &dwBytesRead, NULL);
		count_chars++;
		if (char_buffer == '\n') {
			count_lines++;
		}
	}
	count_lines++;
	return count_lines;
}




DWORD set_file_size(HANDLE hfile, DWORD new_size) {
	//  move output file pointer to the size of input file  
	DWORD dwPtr = SetFilePointer(hfile, new_size, NULL, FILE_BEGIN);

	if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
	{
		return INVALID_SET_FILE_POINTER;
	} // End of error handler 
	SetEndOfFile(hfile);//setting the output size equal to the input file size
	return 0;//the function successed
}

int close_all_handles(HANDLE hInputFile, HANDLE hOutputFile, HANDLE threads_handles[], int num_threads) {
	BOOL ret_val;
	//Close thread handles
	for (int i = 0; i < num_threads; i++)
	{
		ret_val = CloseHandle(*(threads_handles + i));
		CHECK_IF_CLOSING_HANDLER_FAILED(ret_val);
	}

	//close input file handle
	ret_val = CloseHandle(hInputFile);
	CHECK_IF_CLOSING_HANDLER_FAILED(ret_val);

	//close output file handle
	ret_val = CloseHandle(hOutputFile);
	CHECK_IF_CLOSING_HANDLER_FAILED(ret_val);
	return 0;//returns 0 if all the handles closed
}