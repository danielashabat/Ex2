#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <Windows.h>

#define MYNULL ((void*)0)

//void thread_function_wrapper(lpparms parmas) {
	//Thread_Function(params.file_name_to_read, params.file_name_to_write)
//}

void Thread_Function(char* file_name_to_read, char* file_name_to_write, LONG offset, DWORD number_of_bytes_to_read) {

	HANDLE hfile_to_read;
	HANDLE hfile_to_write;
	LPVOID data_from_file;
	BOOL bErrorFlag = FALSE;
	LPDWORD lpNumberOfBytesRead;

	//Open input file and create HANDLE to file
	hfile_to_read = CreateFileA((LPCSTR)file_name_to_read, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_SHARE_READ, MYNULL);
	// Set pointer to the place we want to start reading.
	//every thread will get the offset from the begining of the file. 
	SetFilePointer(hfile_to_read, offset, MYNULL, FILE_BEGIN);
	// read from the given number of file that given from the pointer 
//	bErrorFlag = ReadFile(hfile_to_read, number_of_bytes_to_read, lpNumberOfBytesRead,data_from_file,MYNULL);
					


}