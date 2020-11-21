

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <stdbool.h>
#include "decryptor.h"
#include "decryptor_thread.h"


//command line: caesar.exe, input_file, key, number of threads
int main(int argc, char* argv[]) {
	int i = 0;
	FILE* input_file=NULL;
	int thread_size = 0;
	HANDLE h_input_file;
	HANDLE h_output_file;
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

	int key = (int )argv[2];
	int num_threads =(int)argv[3];

	h_input_file = CreateFile(argv[1],// file name 
		GENERIC_READ,          // open for reading 
		FILE_SHARE_READ,       // open sharing for reading only 
		NULL,                  // default security 
		OPEN_EXISTING,         // existing file only 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 

	h_output_file = CreateFile("decrypted.txt",// file name 
		GENERIC_WRITE,          // open for reading 
		FILE_SHARE_WRITE,      // open sharing for writing only 
		NULL,                  // default security 
		CREATE_ALWAYS,         // always creats a new file, if the file exists it overwrites it 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 


	thread_size= GetFileSize(h_input_file, NULL) /num_threads;//counting chars in input text and dividing it in the number of threads 

	ret_val = CloseHandle(h_input_file);
	if (false == ret_val)
	{
		printf("Error when closing\n");
		return 1;
	}
	ret_val = CloseHandle(h_output_file);
	if (false == ret_val)
	{
		printf("Error when closing\n");
		return 1;
	}


	threads_handles = (HANDLE*)malloc(num_threads * sizeof(HANDLE));
	
	if (threads_handles == NULL) {
		printf("ERROR: allocation failed!\n");
			return 1;
	}

	thread_ids = (DWORD*)malloc(num_threads * sizeof(DWORD));
	if (thread_ids == NULL) {
		printf("ERROR: allocation failed!\n");
		return 1;
	}


	//for (i = 0; i < num_threads; i++)
	//{
	//	threads_handles[i] = CreateThread(/////need to handle with this warning!!
	//		NULL,                   // default security attributes
	//		0,                    // use default stack size  
	//		decryptor_thread,       // thread function name
	//		NULL,          // argument to thread function 
	//		0,                      // use default creation flags 
	//		&thread_ids[i]);   // returns the thread identifie


	//// Check the return value for success.
	//// If CreateThread fails, terminate execution. 
	//// This will automatically clean up threads and memory. 

	//	if (threads_handles[i] == NULL)
	//	{
	//		printf("create thread failed\n");
	//		ExitProcess(3);
	//	}
	//}// End of main thread creation loop.



	// //Close thread handles
	//for (i = 0; i < num_threads; i++)
	//{
	//	ret_val = CloseHandle(threads_handles[i]);
	//	if (false == ret_val)
	//	{
	//		printf("Error when closing\n");
	//		return 1;
	//	}
	//}



	free(threads_handles);
	thread_ids;
	return 0;
}

