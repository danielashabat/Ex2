

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include "files_functions.h"
#include "ThreadFunction.h"///////change to header!!


// Defines --------------------------------------------------------------------

/*if allocation failed exit program*/
#define SUCCSESS TRUE;
#define FAIL FALSE;
#define THREAD_SUCCESS ((int)(0))
#define THREAD_FAIL ((int)(1))

#define CHECK_IF_ALLOCATION_FAILED(RET_VAL) if (RET_VAL == NULL) {\
	printf("memory allocation failed\n");\
	return FAIL;\
}





// Implementation -------------------------------------------------------

//command line: caesar.exe, input_file, key, number of threads, mode 
int main(int argc, char* argv[]) {
	BOOL pass_or_fail = FALSE;
	int i = 0;
	char input_path[MAX_PATH];
	char output_path[MAX_PATH];
	int* lines_per_thread=NULL;
	HANDLE h_input_file=NULL;
	HANDLE h_output_file=NULL;
	DWORD dw_ret = 0;
	HANDLE* threads_handles = NULL; //pointer to threads handles array
	DWORD *thread_ids =NULL; ////pointer to threads ids array
	BOOL ret_val;
	DWORD start_point = 0, end_point = 0;
	ThreadData* ptr_to_thread_data = NULL;
	DWORD exit_code;

	//check if there exactly 5 arguments
	if (argc < 5) {
		printf("ERROR:not enough arguments!");
		return EXIT_FAILURE;
	}
	if (argc > 5) {
		printf("ERROR:too many arguments!");
		return EXIT_FAILURE;
	}
	

	int key = atoi(argv[2]);
	int num_threads =atoi(argv[3]);
	char enc_or_dec = argv[4][1];

	strcpy_s(input_path, MAX_PATH, argv[1]);

	switch (enc_or_dec) {
	case 'd':
		strcpy_s(output_path, MAX_PATH, "decrypted.txt");
		break;
	case 'e':
		strcpy_s(output_path, MAX_PATH, "encrypted.txt");
	}

	h_input_file = CreateFileA(input_path,// file name 
		GENERIC_READ,          // open for reading 
		FILE_SHARE_READ,       // open sharing for reading only 
		NULL,                  // default security 
		OPEN_EXISTING,         // existing file only 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 
	pass_or_fail = check_file_handle(h_input_file,input_path);
	if (!pass_or_fail)
		goto cleanup;

	h_output_file = CreateFileA(output_path,// file name 
		GENERIC_WRITE,          // open for reading 
		FILE_SHARE_WRITE,      // open sharing for writing only 
		NULL,                  // default security 
		CREATE_ALWAYS,         // always creats a new file, if the file exists it overwrites it 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 
	pass_or_fail = check_file_handle(h_output_file, output_path);
	if (!pass_or_fail)
		goto cleanup;
	dw_ret= set_file_size(h_output_file, GetFileSize(h_input_file, NULL));
	if (dw_ret == 1 ){
		pass_or_fail = FAIL;
		printf("ERROR:%d  set_file_size failed\n", GetLastError());
		goto cleanup;
	}

	//lines_per_thread= count_lines(h_input_file) /num_threads;// dividing all the lines in the file with the number of threads 

	threads_handles = (HANDLE*)malloc(sizeof(HANDLE)*num_threads);//creating array of handles in the size of num_threads
	
	for (int i = 0; i < num_threads; i++) {//inital array to NULL for close handles
		threads_handles[i] = NULL;
	}
	if (threads_handles == NULL) {//checl if aloocation failed
		pass_or_fail = FAIL;
		printf("memory allocation failed\n");
		goto cleanup;
	}

	thread_ids = (DWORD*)malloc(num_threads * sizeof(DWORD));// creating array of DWORD in the size of num_threads
	for (int i = 0; i < num_threads; i++) {//inital array to NULL for close handles
		thread_ids[i] = NULL;
	}
	if (thread_ids == NULL) {//check if aloocation failed
		pass_or_fail = FAIL;
		printf("memory allocation failed\n");
		goto cleanup;
	}


	pass_or_fail = divide_lines_per_thread(h_input_file, num_threads,&lines_per_thread);//initial array with the amount of lines thread in index 'i' need to read
	if (!pass_or_fail)
		goto cleanup;


	for (i = 0; i < num_threads; i++)
	{

		get_start_point(&start_point);

		pass_or_fail = get_end_point(h_input_file,lines_per_thread[i],&end_point);
		if (!pass_or_fail)
			goto cleanup;
		
		pass_or_fail = CreateThreadData(start_point, end_point, input_path, output_path, key,&ptr_to_thread_data);
		if (!pass_or_fail)
			goto cleanup;
		printf( "thread index: %d, lines_per_thread:%d, startpoint: %ld, endpoint:%ld\n",i, lines_per_thread[i], ptr_to_thread_data->start_point, ptr_to_thread_data->end_point);
		
		
		if (enc_or_dec == 'd') {
			*(threads_handles + i) = CreateThread(/////need to handle with this warning!!
				NULL,                   // default security attributes
				0,                    // use default stack size  
				decrypt_thread,       // thread function name
				ptr_to_thread_data,          // argument to thread function 
				0,                      // use default creation flags 
				&thread_ids[i]);   // returns the thread identifie
		}

		if (enc_or_dec == 'e') {
			*(threads_handles + i) = CreateThread(/////need to handle with this warning!!
				NULL,                   // default security attributes
				0,                    // use default stack size  
				encrypt_thread,       // thread function name
				ptr_to_thread_data,          // argument to thread function 
				0,                      // use default creation flags 
				&thread_ids[i]);   // returns the thread identifie
		}



	 //Check the return value for success.
	 //If CreateThread fails, terminate execution. 
	 //This will automatically clean up threads and memory. 

		if (*(threads_handles+i) == NULL)
		{
			printf("ERRROR: %d while attempt to create thread\n",	 GetLastError());
			pass_or_fail = FAIL;
			goto cleanup;
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
		pass_or_fail = FAIL;
		goto cleanup;
	}
	//check exit code of each thread
	for (int i = 0; i < num_threads; i++) {
		ret_val = GetExitCodeThread(*(threads_handles + i), &exit_code);
		if (ret_val == 0) {
			printf("Error when getting thread exit code\n");
			pass_or_fail = FAIL;
			goto cleanup;
		}
		if (THREAD_FAIL == (int)exit_code) {
			pass_or_fail = FAIL;
			goto cleanup;
		}
		if (THREAD_SUCCESS != (int)exit_code){
			pass_or_fail = FAIL;
			goto cleanup;
		}
	}
	cleanup:
	ret_val=close_all_handles(h_input_file, h_output_file, threads_handles, num_threads);//closing all the handles
	if (ret_val == 1) {
		printf("ERROR:closing one of the handles handles failed!\n");
		return 1;
	}
	free(lines_per_thread);
	free(threads_handles);
	free(thread_ids);
	if (!pass_or_fail) {
		return 1;
	}
	return 0;
}

