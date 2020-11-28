#ifndef	THREADFUNCTION_H
#define THREADFUNCTION_H

// Includes --------------------------------------------------------------------
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <stdbool.h>
#include <assert.h>
#include <Windows.h>
#define SUCCSESS TRUE;
#define FAIL FALSE;
// Structs --------------------------------------------------------------------

////the purpose of this struct is to hold the necessary arguments to 'ThreadFunction'
typedef struct ThreadData {
	char input_path[MAX_PATH];// the path of the input file
	char output_path[MAX_PATH];// the path of the output file
	DWORD  start_point;//the statring point (by bytes) the thread need to decrypte/encrypte 
	DWORD end_point;//the ending point (by bytes) the thread need to decrypte/encrypte (include this bytes)
	int key; // the key for the decryption/encryption
} ThreadData;


// Function Declarations -------------------------------------------------------

//decrepy thread - this function define as thread function if we get '-d' as function
// It get pointer to Thread data that hold all the paramters needed for reading and writing 
// the function read data from file in specific line , decrypt and write to the same place in file
// it return 0 as exit code if it's successed and 1 if it failed.
DWORD WINAPI decrypt_thread(LPVOID lpParam);
//decrepy thread - this function define as thread function if we get '-e' as function
// It get pointer to Thread data that hold all the paramters needed for reading and writing 
// the function read data from file in specific line , encrypt and write to the same place in file
// it return 0 as exit code if it's successed and 1 if it failed.
DWORD WINAPI encrypt_thread(LPVOID lpParam);

//This function get the offset from the begining of the line and number of bytes to read 
//We set pointer to this byte in the file and then reading this the number of bytes to the end of the line
//we return FALSE if we failed (in allocation )and TRUE if success
BOOL get_data_from_file_in_specipfic_lines(char* file_name_to_read, char* data_from_file, LONG offset, DWORD number_of_bytes_to_read);
//This function get the offset from the begining of the line and number if bytes to write.
//We set pointer to this byte in the file and then writing this the number of bytes to the end of the line
//we return FALSE if we failed (in allocation )and TRUE if success
BOOL write_to_specific_lines(char* file_name_to_write, char* data_to_file, LONG offset, DWORD number_of_bytes_to_read);

//the purpose of this function is to create data for passsing the necessary arguments to 'ThreadFunction'
//it returns pointer to ThreadData that contains the relevant data per thread by it's index thread.
//if the function fails it returns NULL
BOOL CreateThreadData(DWORD start_point,////the statring point (by bytes) the thread need to decrypte/encrypte
	DWORD end_point,//the ending point (by bytes) the thread need to decrypte/encrypte (include this bytes)
	char input_path[],//input file path
	char output_path[], //output file path
	int key,// the key for the decryption/encryption
	ThreadData** ptr_to_thread_data);// the key for the decryption/encryption
#endif
#pragma once


#pragma once


#pragma once
