
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

DWORD adding_lines_to_file_pointer(HANDLE hfile, int lines_per_thread,int file_pointer) {
	char char_buffer;
	DWORD nBytesToRead = 1;
	DWORD dwBytesRead = 0;
	BOOL bResult = FALSE;
	int count_lines = 0;
	printf("file_pointer:%d\n", file_pointer);
	//  move  file pointer to the begining  
	DWORD dwPtr = SetFilePointer(hfile,file_pointer, NULL, FILE_BEGIN);

	if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
	{
		return INVALID_SET_FILE_POINTER;
	} // End of error handler 

	while (!bResult || (dwBytesRead != 0) && (count_lines < lines_per_thread)) {//looping until it's end-of-file or it read all the lined needed
		bResult = ReadFile(hfile, &char_buffer, nBytesToRead, &dwBytesRead, NULL);
		file_pointer++;
		if (char_buffer == '\n') {
			count_lines++;
		}
	}
	file_pointer = file_pointer - 1;//end loop with extra byte 
	
	return file_pointer;

}