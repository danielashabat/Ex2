
// Includes --------------------------------------------------------------------


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include "files_functions.h"


// Implementation -------------------------------------------------------


int count_lines(HANDLE hfile) {
	char char_buffer;
	DWORD nBytesToRead = 1;
	DWORD dwBytesRead = 0;
	DWORD dwFileSize = GetFileSize(hfile, NULL);
	BOOL bResult = FALSE;
	int count_lines = 0;

	while (!bResult || (dwBytesRead != 0)) {
		bResult = ReadFile(hfile, &char_buffer, nBytesToRead, &dwBytesRead, NULL);
		if (char_buffer == '\n') {
			count_lines++;
		}
	}
	count_lines++;
	return count_lines;
}




DWORD set_file_size(HANDLE hfile, DWORD new_size) {
	if (new_size == INVALID_FILE_SIZE) {//check if new_size is valid
		return 1;
	}

	//  move output file pointer to the size of input file  
	DWORD dwPtr = SetFilePointer(hfile, new_size, NULL, FILE_BEGIN);

	if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
	{
		return 1;
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
		CHECK_IF_CLOSING_HANDLE_FAILED(ret_val);
	}

	//close input file handle
	ret_val = CloseHandle(hInputFile);
	CHECK_IF_CLOSING_HANDLE_FAILED(ret_val);

	//close output file handle
	ret_val = CloseHandle(hOutputFile);
	CHECK_IF_CLOSING_HANDLE_FAILED(ret_val);
	return 0;//returns 0 if all the handles closed
}


DWORD get_start_point() {
	return file_pointer;
}




DWORD get_end_point(HANDLE hfile, int lines_per_thread) {
	char char_buffer;
	DWORD nBytesToRead = 1;
	DWORD dwBytesRead = 0;
	BOOL bResult = FALSE;
	int count_lines = 0;

	if (lines_per_thread == 0) {//if no lines need to be read, return -1.
		return -1;
	}
	if (file_pointer == 0) {
		//  move  file pointer to the begining  
		DWORD dwPtr = SetFilePointer(hfile, file_pointer, NULL, FILE_BEGIN);

		if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
		{
			return INVALID_SET_FILE_POINTER;
		} // End of error handler 
	}
	while (!bResult || (dwBytesRead != 0) && (count_lines < lines_per_thread)) {//looping until it's end-of-file or it read all the lines needed
		bResult = ReadFile(hfile, &char_buffer, nBytesToRead, &dwBytesRead, NULL);
		file_pointer++;
		if (char_buffer == '\n') {
			count_lines++;
		}
	}
	 
	return (file_pointer -1);//end loop with extra byte

}

int* divide_lines_per_thread(HANDLE h_input_file, int num_threads) {
	int* lines_per_thread = NULL;
	int total_lines_in_file = count_lines(h_input_file);
	int i = 0;

	lines_per_thread=(int*)malloc(num_threads * sizeof(int));// creating array of int in the size of num_threads
	if (lines_per_thread == NULL) {
		return NULL;
	}
	while (i < num_threads) {//initial size for every thread to 0.
		lines_per_thread[i] = 0;
		i++;
	}
	i = 0;
	while (i < total_lines_in_file) {//dividing all lines equally +-1 to threads
		lines_per_thread[i % (num_threads)]++;
		
		i++;
	}

	printf("there is total %d threads in program and %d toal lines, total size of file(bytes):%ld\n", num_threads, total_lines_in_file,);
	i = 0;
	while (i < num_threads) {
	printf("thread %d lines_per_thread:%d\n", i, lines_per_thread[i % (num_threads)]);
	i++;
	}
	return lines_per_thread;
}