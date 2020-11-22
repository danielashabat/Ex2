

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <stdbool.h>
#include "decryptor.h"
#include "decryptor_thread.h"


// Defines --------------------------------------------------------------------
#define MAX_PATH_LENGTH 200 //the maximun length of the input/output path

// Structs --------------------------------------------------------------------
// Sample custom data structure for threads to use.
typedef struct ThreadData {
	char input_path[MAX_PATH_LENGTH];
	char output_path[MAX_PATH_LENGTH];
	int  startpoint;
	int endpoint;
	int key;
} ThreadData;

// Function Declarations -------------------------------------------------------
ThreadData* CreateThreadData(int thread_index, //the thread index
	int thread_size,  // the size of bytes the thread need to read from input file 
	char input_path[],//input file path
	char output_path[], //output file path
	int key); // the key for the decryption/encryption

int count_lines(HANDLE hfile);//this function counts the number of lines in the file

// Implementation -------------------------------------------------------

//command line: caesar.exe, input_file, key, number of threads
int main(int argc, char* argv[]) {
	int i = 0;
	FILE* input_file=NULL;
	int thread_size = 0;
	char input_path[MAX_PATH_LENGTH];
	char output_path[MAX_PATH_LENGTH];
	HANDLE h_input_file;
	HANDLE h_output_file;
	DWORD input_file_size = 0;
	DWORD dwPtr = 0;
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
	
	strcpy_s(input_path,MAX_PATH_LENGTH,argv[1]);
	strcpy_s(output_path, MAX_PATH_LENGTH, "decrypted.txt");
	int key = atoi(argv[2]);
	int num_threads =atoi(argv[3]);

	h_input_file = CreateFile(input_path,// file name 
		GENERIC_READ,          // open for reading 
		FILE_SHARE_READ,       // open sharing for reading only 
		NULL,                  // default security 
		OPEN_EXISTING,         // existing file only 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 

	h_output_file = CreateFile(output_path,// file name 
		GENERIC_WRITE,          // open for reading 
		FILE_SHARE_WRITE,      // open sharing for writing only 
		NULL,                  // default security 
		CREATE_ALWAYS,         // always creats a new file, if the file exists it overwrites it 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 


	//  move output file pointer to the size of input file  
	dwPtr = SetFilePointer(h_output_file, GetFileSize(h_input_file, NULL),NULL,FILE_BEGIN);

	if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
	{
		// Obtain the error code. 
		DWORD dwError = GetLastError();

		// Deal with failure 
		// . . . 

	} // End of error handler 
	SetEndOfFile(h_output_file);//setting the output size equal to the input file size

	thread_size= count_lines(h_input_file) /num_threads;// dividing input text lines in the number of threads 

	threads_handles = (HANDLE*)malloc(sizeof(HANDLE)*num_threads);

	if (threads_handles == NULL) {
		printf("ERROR: allocation failed!\n");
			return 1;
	}

	thread_ids = (DWORD*)malloc(num_threads * sizeof(DWORD));
	if (thread_ids == NULL) {
		printf("ERROR: allocation failed!\n");
		return 1;
	}


	for (i = 0; i < num_threads; i++)
	{
		ThreadData* data = CreateThreadData(i, thread_size, argv[1], "decrypted.txt", key);
		printf("startline: %d, endline:%d\n", data->startpoint, data->endpoint);
		if (data == NULL) {//check if the allocation failed
			printf("data allocation failed\n");
			ExitProcess(3);
		}

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



	 //Close thread handles
	for (i = 0; i < num_threads; i++)
	{
		ret_val = CloseHandle(*(threads_handles+i));
		if (false == ret_val)
		{
			printf("Error when closing\n");
			return 1;
		}
	}

	//close input file handle
	ret_val = CloseHandle(h_input_file);
	if (false == ret_val)
	{
		printf("Error when closing\n");
		return 1;
	}
	//close output file handle
	ret_val = CloseHandle(h_output_file);
	if (false == ret_val)
	{
		printf("Error when closing\n");
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
	strcpy_s(ptr_to_thread_data->input_path, MAX_PATH_LENGTH, input_path);
	strcpy_s(ptr_to_thread_data->output_path, MAX_PATH_LENGTH, output_path);
	ptr_to_thread_data->startpoint = (thread_size * thread_index);
	ptr_to_thread_data->endpoint = thread_size *(thread_index+1)-1;
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
	int count_chars = 0;

	while (count_chars< dwFileSize) {
		bResult = ReadFile(hfile, &char_buffer, nBytesToRead, &dwBytesRead, NULL);
		count_chars++;
		if (char_buffer == '\n') {
			count_lines++;
		}
	}
	count_lines++;
	return count_lines;
}