#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <stdbool.h>
#include <assert.h>
#include <Windows.h>
#include "decryptor.h"
#include "ThreadFunction.h"
#include "files_functions.h"


//DEFINE -------------------------------------------------------

#define MYNULL ((void*)0)
#define SUCCSESS TRUE;
#define FAIL FALSE;



//THREAD FUNCTIONS -----------------------------------------------



BOOL CreateThreadData(DWORD start_point,//
	DWORD end_point,
	char input_path[],//input file path
	char output_path[], //output file path
	int key, ThreadData** ptr_to_thread_data)// the key for the decryption/encryption

{
	*ptr_to_thread_data = NULL;
	*ptr_to_thread_data = (ThreadData*)malloc(sizeof(ThreadData));
	if (*ptr_to_thread_data == NULL) {
		printf("ERROR: allocation failed!\n");
		return FAIL;
	}
	strcpy_s((*ptr_to_thread_data)->input_path, MAX_PATH, input_path);
	strcpy_s((*ptr_to_thread_data)->output_path, MAX_PATH, output_path);
	(*ptr_to_thread_data)->start_point = start_point;
	(*ptr_to_thread_data)->end_point = end_point;
	(*ptr_to_thread_data)->key = key;
	return SUCCSESS;
}


DWORD WINAPI encrypt_thread(LPVOID lpParam) {
	//Initalize
	char file_name_to_read[MAX_PATH];
	char file_name_to_write[MAX_PATH];
	int start_char;
	int end_char;
	int key;
	int number_of_bytes_to_read;
	BOOL pass_or_fail = FALSE;

	//Cast from LPVOID
	ThreadData* p_params;
	p_params = (ThreadData*)lpParam;

	//Get parameters
	strcpy_s(file_name_to_read, MAX_PATH, p_params->input_path);
	strcpy_s(file_name_to_write, MAX_PATH, p_params->output_path);
	start_char = p_params->start_point;
	end_char = p_params->end_point;
	key = p_params->key;
	free(lpParam);

	if (end_char == -1)//if -1, the file reach to EOF
		return 1;

	//calculate number of bytes to read and write
	number_of_bytes_to_read = end_char - start_char + 1;
	//Create string to ReadFile and WriteFile
	char* data;
	data = (char*)malloc(number_of_bytes_to_read * sizeof(char));
	if (data == NULL) {
		printf("ERROR: allocation failed!\n");
		return 1;
	}
	//memset(data, 0, (number_of_bytes_to_read));


	//Read from file 
	pass_or_fail=get_data_from_file_in_specipfic_lines(file_name_to_read, data, start_char, number_of_bytes_to_read);
	if (!pass_or_fail) {
		free(data);
		return 1;
	}
	//decrypt data
	string_encryptor(data, key, number_of_bytes_to_read);
	//Write to file
	pass_or_fail = write_to_specific_lines(file_name_to_write, data, start_char, number_of_bytes_to_read);
	if (!pass_or_fail) {
		free(data);
		return 1;
	}
	free(data);
	return 0;
	
}

DWORD WINAPI decrypt_thread(LPVOID lpParam) {
	//Initalize
	char file_name_to_read[MAX_PATH];
	char file_name_to_write[MAX_PATH];
	int start_char;
	int end_char;
	int key;
	int number_of_bytes_to_read;
	BOOL pass_or_fail = FALSE;
	//Cast from LPVOID
	ThreadData* p_params;
	p_params = (ThreadData*)lpParam;

	//Get parameters
	strcpy_s(file_name_to_read, MAX_PATH, p_params->input_path);
	strcpy_s(file_name_to_write, MAX_PATH, p_params->output_path);
	start_char = p_params->start_point;
	end_char = p_params->end_point;
	key = p_params->key;
	free(lpParam);

	if (end_char == -1)//if -1, the file reach to EOF
		return 1;

	//calculate number of bytes to read and write
	number_of_bytes_to_read = end_char - start_char +1 ;
	//Create string to ReadFile and WriteFile
	char* data;
	data = (char*)malloc(number_of_bytes_to_read * sizeof(char));
	//memset(data, 0, (number_of_bytes_to_read));


	//Read from file 
	pass_or_fail=get_data_from_file_in_specipfic_lines(file_name_to_read, data, start_char, number_of_bytes_to_read);
	if (!pass_or_fail) {
		free(data);
		return 1;
	}
	//decrypt data
	string_decryptor(data, key,number_of_bytes_to_read);
	//Write to file
	pass_or_fail=write_to_specific_lines(file_name_to_write, data, start_char, number_of_bytes_to_read);
	if (!pass_or_fail) {
		free(data);
		return 1;
	}
	free(data);
	return 0; 
	

}


//READ AND WRITE FUNCTIONS -----------------------------------------

BOOL get_data_from_file_in_specipfic_lines(char* file_name_to_read, char* data_from_file, LONG offset, DWORD number_of_bytes_to_read) {

	HANDLE hfile_to_read;
	BOOL bErrorFlag = FALSE;
	DWORD lpNumberOfBytesRead;
	BOOL pass_or_fail = FALSE;
	int dw_pointer;

	//Create handle to output file 
	hfile_to_read = CreateFileA((LPCSTR)file_name_to_read, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, MYNULL);
	pass_or_fail = check_file_handle(hfile_to_read, file_name_to_read);
	if (!pass_or_fail) {
		CloseHandle(hfile_to_read);
		return FAIL;
	}
	// Set pointer to the place we want to start reading.
	//every thread will get the offset from the begining of the file. 

	dw_pointer = SetFilePointer(hfile_to_read, offset, MYNULL, FILE_BEGIN);
	// read from the given number of file that given from the pointer 
	bErrorFlag = ReadFile(hfile_to_read, data_from_file, number_of_bytes_to_read, &lpNumberOfBytesRead, MYNULL);
	pass_or_fail = check_ReadFile_WriteFile(bErrorFlag, number_of_bytes_to_read, lpNumberOfBytesRead);
	if (!pass_or_fail) {
		CloseHandle(hfile_to_read);
		return FAIL;
	}
	CloseHandle(hfile_to_read);
	return SUCCSESS;

}

BOOL write_to_specific_lines(char* file_name_to_write, char* data_to_file, LONG offset, DWORD number_of_bytes_to_read) {
	HANDLE hfile_to_write=NULL;
	BOOL bErrorFlag = FALSE;
	DWORD lpNumberOfBytesWritten;
	BOOL pass_or_fail = FALSE;
	//Create HANDLE for input file 
	hfile_to_write = CreateFileA((LPCSTR)file_name_to_write, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, MYNULL);
	pass_or_fail = check_file_handle(hfile_to_write, file_name_to_write);
	if (!pass_or_fail) {
		CloseHandle(hfile_to_write);
		return FAIL;
	}
	// Set pointer to the place we want to start reading.
	//every thread will get the offset from the begining of the file. 
	SetFilePointer(hfile_to_write, offset, MYNULL, FILE_BEGIN);
	// read from the given number of file that given from the pointer 
	bErrorFlag = WriteFile(hfile_to_write, data_to_file, number_of_bytes_to_read, &lpNumberOfBytesWritten, MYNULL);
	pass_or_fail = check_ReadFile_WriteFile(bErrorFlag, number_of_bytes_to_read, lpNumberOfBytesWritten);
	if (!pass_or_fail) {
		CloseHandle(hfile_to_write);
		return FAIL;
	}
	//check_ReadFile_WriteFile_errors(bErrorFlag,number_of_bytes_to_read,lpNumberOfBytesWritten,file_name_to_write);
	CloseHandle(hfile_to_write);
	return SUCCSESS;
}




