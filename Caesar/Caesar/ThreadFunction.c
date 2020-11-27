#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <stdbool.h>
#include <assert.h>
#include <Windows.h>
#include "decryptor.h"
#include "encryptor.h"
#include "ThreadFunction.h"

//DEFINE -------------------------------------------------------

#define MYNULL ((void*)0)




//THREAD FUNCTIONS -----------------------------------------------



ThreadData* CreateThreadData(DWORD start_point,//
	DWORD end_point,
	char input_path[],//input file path
	char output_path[], //output file path
	int key)// the key for the decryption/encryption

{
	ThreadData* ptr_to_thread_data = NULL;
	ptr_to_thread_data = (ThreadData*)malloc(sizeof(ThreadData));
	if (ptr_to_thread_data == NULL) {
		printf("ERROR: allocation failed!\n");
		return NULL;
	}
	strcpy_s(ptr_to_thread_data->input_path, MAX_PATH, input_path);
	strcpy_s(ptr_to_thread_data->output_path, MAX_PATH, output_path);
	ptr_to_thread_data->start_point = start_point;
	ptr_to_thread_data->end_point = end_point;
	ptr_to_thread_data->key = key;
	return ptr_to_thread_data;
}


DWORD WINAPI encrypt_thread(LPVOID lpParam) {
	//Initalize
	char file_name_to_read[MAX_PATH];
	char file_name_to_write[MAX_PATH];
	int start_char;
	int end_char;
	int key;
	int number_of_bytes_to_read;

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
	//calculate number of bytes to read and write
	number_of_bytes_to_read = end_char - start_char + 1;
	//Create string to ReadFile and WriteFile
	char* data;
	data = (char*)malloc(number_of_bytes_to_read * sizeof(char));
	//memset(data, 0, (number_of_bytes_to_read));


	//Read from file 
	get_data_from_file_in_specipfic_lines(file_name_to_read, data, start_char, number_of_bytes_to_read);
	//decrypt data
	string_encryptor(data, key, number_of_bytes_to_read);
	//Write to file
	write_to_specific_lines(file_name_to_write, data, start_char, number_of_bytes_to_read);
	free(data);

	
}

DWORD WINAPI decrypt_thread(LPVOID lpParam) {
	//Initalize
	char file_name_to_read[MAX_PATH];
	char file_name_to_write[MAX_PATH];
	int start_char;
	int end_char;
	int key;
	int number_of_bytes_to_read;

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
	//calculate number of bytes to read and write
	number_of_bytes_to_read = end_char - start_char +1 ;
	//Create string to ReadFile and WriteFile
	char* data;
	data = (char*)malloc(number_of_bytes_to_read * sizeof(char));
	//memset(data, 0, (number_of_bytes_to_read));


	//Read from file 
	get_data_from_file_in_specipfic_lines(file_name_to_read, data, start_char, number_of_bytes_to_read);
	//decrypt data
	string_decryptor(data, key,number_of_bytes_to_read);
	//Write to file
	write_to_specific_lines(file_name_to_write, data, start_char, number_of_bytes_to_read);
	free(data);
	

}


//READ AND WRITE FUNCTIONS -----------------------------------------

void get_data_from_file_in_specipfic_lines(char* file_name_to_read, char* data_from_file, LONG offset, DWORD number_of_bytes_to_read) {

	HANDLE hfile_to_read;
	BOOL bErrorFlag = FALSE;
	DWORD lpNumberOfBytesRead;
	int dw_pointer;

	//Create handle to output file 
	hfile_to_read = CreateFileA((LPCSTR)file_name_to_read, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, MYNULL);
	check_file_handle(hfile_to_read, file_name_to_read);

	// Set pointer to the place we want to start reading.
	//every thread will get the offset from the begining of the file. 

	dw_pointer = SetFilePointer(hfile_to_read, offset, MYNULL, FILE_BEGIN);
	// read from the given number of file that given from the pointer 
	bErrorFlag = ReadFile(hfile_to_read, data_from_file, number_of_bytes_to_read, &lpNumberOfBytesRead, MYNULL);
	check_ReadFile_WriteFile(bErrorFlag, number_of_bytes_to_read, lpNumberOfBytesRead);

	CloseHandle(hfile_to_read);


}

void write_to_specific_lines(char* file_name_to_write, char* data_to_file, LONG offset, DWORD number_of_bytes_to_read) {
	HANDLE hfile_to_write;
	BOOL bErrorFlag = FALSE;
	DWORD lpNumberOfBytesWritten;

	//Create HANDLE for input file 
	hfile_to_write = CreateFileA((LPCSTR)file_name_to_write, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, MYNULL);
	check_file_handle(hfile_to_write, file_name_to_write);

	// Set pointer to the place we want to start reading.
	//every thread will get the offset from the begining of the file. 
	SetFilePointer(hfile_to_write, offset, MYNULL, FILE_BEGIN);
	// read from the given number of file that given from the pointer 
	bErrorFlag = WriteFile(hfile_to_write, data_to_file, number_of_bytes_to_read, &lpNumberOfBytesWritten, MYNULL);
	check_ReadFile_WriteFile(bErrorFlag, number_of_bytes_to_read, lpNumberOfBytesWritten);
	//check_ReadFile_WriteFile_errors(bErrorFlag,number_of_bytes_to_read,lpNumberOfBytesWritten,file_name_to_write);
	CloseHandle(hfile_to_write);

}


//CHECKING FUNCTIONS ----------------------------------------------------------------------------

void check_file_handle(HANDLE h_file, char* file_name) {
	if (h_file == INVALID_HANDLE_VALUE)

		printf("Could not open %s file, error %ld\n", file_name, GetLastError());

	else

		printf("%s file HANDLE is OK!\n", file_name);

}

void check_ReadFile_WriteFile(BOOL bErrorFlag, DWORD number_of_bytes_to_read_or_write, DWORD lpNumberOfBytesRead_or_Written) {

	if (FALSE == bErrorFlag)
	{
		printf("Terminal failure: Unable to write to file.\n");
	}
	else
	{
		if (number_of_bytes_to_read_or_write != lpNumberOfBytesRead_or_Written)
		{
			// This is an error because a synchronous write that results in
			// success (WriteFile returns TRUE) should write all data as
			// requested. This would not necessarily be the case for
			// asynchronous writes.
			printf("Error: dwBytesWritten != dwBytesToWrite\n");
		}
		else
		{
			printf("Read or Write successfully.\n");
		}
	}



}