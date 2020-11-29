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


/*this function counts the number of lines in the file and assign it into count_lines
return SUCCESS if the function succeed with no errors, otherwise return FAIL */
BOOL count_lines(HANDLE hfile,//handle to the file for counting the lines from
	int* count_lines);//pointer to int where the result will be assigned

/*this function setting the file size to new_size(in bytes)
the function return 'INVALID_SET_FILE_POINTER' if it fails, and '0' otherwise*/
DWORD set_file_size(HANDLE hfile,//handler to the file 
	DWORD new_size);// the new size of the file 

/*this function close all the handles that opens in the main
return 0 if all the handles closed, return 1 otherwise*/
int close_all_handles(HANDLE hInputFile, HANDLE hOutputFile, HANDLE threads_handles[], int num_threads, int* lines_per_thread, DWORD * thread_ids);

//returns the starting point for a new thread (by bytes)
void get_start_point(DWORD * start_point);

/*this function moves the pointer in hfile by number of lines 'lines_per_thread' and assign the end point for thread in a pointer to DWORD*/
/*return SUCCESS if the function succeed with no errors, otherwise return FAIL*/
BOOL get_end_point(HANDLE hfile, //handler to input file 
	int lines_per_thread,//the number of lines every thread need to read
	DWORD * end_point);//pointer to DWORD where the 'end_point' value will be be assigned
					

/*this function count the total lines in the file and divide it with the toal number of thread in the program in a 'fair way'
return SUCCESS if the function succeed with no errors, otherwise return FAIL */
BOOL divide_lines_per_thread(HANDLE h_input_file,//the file where the lines need to be count
								int num_threads,//total number of threads in the program
								int** lines_per_thread);// an array that holds in the index 'i' the lines per thread 'i'

//checking if reading or writing is pass
BOOL check_ReadFile_WriteFile(BOOL bErrorFlag, DWORD number_of_bytes_to_read_or_write, DWORD lpNumberOfBytesRead_or_Written);
BOOL check_file_handle(HANDLE h_file, char* file_name);

// static vars Declarations -------------------------------------------------------
static DWORD file_pointer = 0; //this variabele holds the pointer in the input file (in the main thread)

#endif
#pragma once
