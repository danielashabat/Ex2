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


// Function Declarations -------------------------------------------------------
//this function enryptes the string with the given 'key' and change the string the the decrypted string.

void check_file_handle(HANDLE h_file, char* file_name);
void check_ReadFile_WriteFile(BOOL bErrorFlag, DWORD number_of_bytes_to_read_or_write, DWORD lpNumberOfBytesRead_or_Written);
DWORD WINAPI decrypt_thread(LPVOID lpParam);
DWORD WINAPI encrypt_thread(LPVOID lpParam);
void get_data_from_file_in_specipfic_lines(char* file_name_to_read, char* data_from_file, LONG offset, DWORD number_of_bytes_to_read);
void write_to_specific_lines(char* file_name_to_write, char* data_to_file, LONG offset, DWORD number_of_bytes_to_read);



#endif
#pragma once


#pragma once


#pragma once
