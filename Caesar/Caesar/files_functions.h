#ifndef FIELS_FUNCTIONS_H
#define FIELS_FUNCTIONS_H

// Defines --------------------------------------------------------------------


/*if closing handler failed exit program*/
#define CHECK_IF_CLOSING_HANDLE_FAILED(RET_VAL) if (false == RET_VAL) {\
	printf("Error when closing\n");\
	return 1;\
}


// Function Declarations -------------------------------------------------------


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

/*move pointer by number of lines 'lines_per_thread'*/
DWORD adding_lines_to_file_pointer(HANDLE hfile, int lines_per_thread,int file_pointer);




#endif
#pragma once
