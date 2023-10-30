/*
Name : Arun karthik k 
Date : 30-10-2023
File name : test_encode.c   (Main function)
 */

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main( int argc, char *argv[])
{
		// Declaring the structure for encode and decode operation
		EncodeInfo encInfo;
		DecodeInfo decInfo;
        //checking count argument
		if(argc < 3)
		{
				// if false,then print error message
				printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
                printf("Decoding : ./a.out -d <.bmp file> [output file]\n");
				return -1;
		}
        //calling check operation function
		if (check_operation_type(argv) == e_encode )
		{
				//if function return e_encode,then checks for count
				if(argc < 4)
				{
						//if count is less than 4,then prints error message
						printf("Encoding : ./a.out -e <.bmp file> <.txt file> [output file]\n");
						return -1;
				}	
	            //calling read and validate function			
				if(read_and_validate_encode_args(argv, &encInfo) == e_success)
				{
						//if function return e_success,then function call do encode is performed
						if (do_encoding(&encInfo) == e_success)
						{
								//if function return return e_success,then prints Success message
								printf("** ->ENCODING COMPLETED SUCCESSFULLY<- **\n");
						}
						//if functiion returns e_failure,then print error messgae
						else
						{
								printf("Encoding is a failure\n");
						}
				}
				//if read and validate function return e_failure,then prints error message
				else
				{
						printf("Read and validate was not success\n");
						return 1;
				}
		}
		//function calling on check operation
		else if ( check_operation_type(argv)  == e_decode )
		{
				//if function return e_decode,then check for count
				if (argc < 3)
				{
						//if count is lessthan 3,then print error message
						printf("Decoding : ./a.out -d <.bmp file> [output file]\n");
						return -1;
				}
				//function calling on read and validate
				if (read_and_validate_decode_argv(argv, &decInfo) == d_success)
				{
						//if function return d_success,then print success message
						if(do_decoding(&decInfo) == d_success)
						{
								printf("** ->DECODING DONE SUCCESSFULLY<-**\n");
						}
				}
				//if function return d_failure,then prints error message
				else
				{
						printf("Read and validate was failure\n");
						return 1;
				}
		}
		return 0;
}

// --------------------------------------------------------------------------------------------------------------

//function defination for check operation

OperationType check_operation_type(char *argv[])
{
		//checking for argv[1]
		if (strcmp(argv[1], "-e") == 0)
		{
				//if argv[1] == -e,then return e_encode
				return e_encode;
		}
		else if (strcmp(argv[1], "-d") == 0)
		{
				//if argv[1] == -d,then return e_decode
				return e_decode;
		}
		else
		{
				//if argv[1] != -e and -d,then return e_unsupported
				return e_unsupported;
		}
}
