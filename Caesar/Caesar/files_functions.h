#ifndef FIELS_FUNCTIONS_H
#define FIELS_FUNCTIONS_H

// Defines --------------------------------------------------------------------
#define SUCCSESS TRUE;
#define FAIL FALSE;

/*if closing handler failed exit program*/
#define CHECK_IF_CLOSING_HANDLE_FAILED(RET_VAL) if (false == RET_VAL) {\
	printf("Error when closing handle\n");\
	return 1;\
}


// Function Declarations -------------------------------------------------------


//this function counts the number of lines in the file
//returns an int that represent the number of lines in the given file
BOOL count_lines(HANDLE hfile, int* count_lines);//handler to file

/*this function setting the file size to new_size(in bytes)
the function return 'INVALID_SET_FILE_POINTER' if it fails, and '0' otherwise*/
DWORD set_file_size(HANDLE hfile,//handler to the file 
	DWORD new_size);// the new size of the file 

/*this function close all the handles that opens in the main
return 0 if all the handles closed, return 1 otherwise*/
int close_all_handles(HANDLE hInputFile, HANDLE hOutputFile, HANDLE threads_handles[], int num_threads);

//returns the starting point for thread (by bytes)
void get_start_point(DWORD * start_point);

/*move pointer by number of lines 'lines_per_thread' and returns the end point for thread*/
BOOL get_end_point(HANDLE hfile, //handler to input file 
	int lines_per_thread,//the number of lines every thread need to read
	DWORD * end_point);
					


BOOL divide_lines_per_thread(h_input_file, num_threads);
BOOL check_ReadFile_WriteFile(BOOL bErrorFlag, DWORD number_of_bytes_to_read_or_write, DWORD lpNumberOfBytesRead_or_Written);
BOOL check_file_handle(HANDLE h_file, char* file_name);

// static vars Declarations -------------------------------------------------------
static DWORD file_pointer = 0;

#endif
#pragma once
