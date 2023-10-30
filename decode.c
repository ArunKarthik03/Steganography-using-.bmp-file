
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* function defination */

/* Function defination of read and validate */

Status read_and_validate_decode_argv(char *argv[], DecodeInfo *decInfo)
{
		// Checking argv[2] == .bmp
		if( strstr(argv[2], ".bmp"))
		{
				//if yees,then store that file in src_image_fname
				decInfo -> src_image_fname = argv[2];
		}
		//if not return e_failure
		else
		{
				return d_failure;
		}

		//for secret file name allocating the memory
		decInfo -> secret_fname = malloc(12);

		if (argv[3])
		{
				//checking for argv[3],if yes than save the filename.txt
				strcpy(decInfo -> secret_fname, argv[3]);
		}
		else
		{
				//if not,then save as Message.txt
				strcpy(decInfo -> secret_fname, "Message");
		}
		return d_success;
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for do decoding */

Status do_decoding(DecodeInfo *decInfo)
{
		//printing decoding perform's message
		printf("** ->Decoding Procedure Started<- **\n");
		printf("Opening required files\n");
		//calling image file function
		if (open_img_file(decInfo) == d_success)
		{
				//if function return d_success,then print success messgae
				printf("successfully opened %s...\n", decInfo -> src_image_fname);
				//calling decode magic string function
				if (decode_magic_string(decInfo -> fptr_src_image) == d_success)
				{
						//if function return d_success,then print success message
						printf("decode magic string successfully completed...\n");
						//calling decode secret file extension size function
						if (decode_secret_file_extn_size(decInfo) == d_success)
						{
								//if function return d_success,then print success message
								printf("decode secret file extension file size successfully completd...\n");
								//calling decode secret file extension funtion
								if (decode_secret_file_extn(decInfo) == d_success)
								{
										//if function return d_success,then print success message
										printf("secode secret file extension successfully completed...\n");
										//calling decode file data size function
										if (decode_file_data_size(decInfo) == d_success)
										{
                                                //if function return d_success,then print success message
												printf("decode file data size successfully completed...\n");
												//calling decode file data function
												if (decode_file_data(decInfo) == d_success)
												{
														//if function return d_success,then print success message
														printf("decode file data successfully completed...\n");
												}
												//if any above any one function retuen d_failure,then print error message and return d_failure to test encode.c for decoding opeartion
												else
												{
														printf("Decoding secret file data was a failure\n");
														return d_failure;
												}
										}
										else
										{
												printf("Decoding secret file data size was a failure\n");
												return d_failure;
										}
								}
								else
								{
										printf("Decoding secret file extn was a failure\n");
										return d_failure;
								}
						}
						else
						{
								printf("Decoding secret file extn size was a failure\n");
								return d_failure;
						}
				}
				else
				{
						printf("Decoding magic string was a failure\n");
				}
		}
		else
		{
				printf("open file was failure\n");
				return d_failure;
		}
		//if all function successfully completed,then return d_success to test encode.c for decoding operation
		return d_success; 
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for open image file*/

Status open_img_file(DecodeInfo *decInfo)
    
{
    decInfo -> fptr_src_image = fopen(decInfo -> src_image_fname, "r");
    if ( decInfo -> fptr_src_image == NULL )
    {
	perror("fopen");
	fprintf(stderr, "ERROR : Unable to open file %s\n", decInfo -> src_image_fname);
	return d_failure;
    }
    return d_success;
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for decode magic string */

Status decode_magic_string(FILE *fptr_image)
{
    printf("Decoding Magic String Signature\n");
    //declaring one character array with size
    char buffer[8];
    char *ch = malloc(strlen(MAGIC_STRING));

    fseek(fptr_image, 54, SEEK_SET);

    for ( int i=0;i<strlen(MAGIC_STRING);i++)
    {
	fread(buffer,8,1,fptr_image);
	ch[i]=decode_byte_from_lsb(buffer);
    }
    if(strcmp(ch,MAGIC_STRING) == 0)
    {
	return d_success;
    }
    else
    {
	return d_failure;
    }
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for decode byte to lsb function */

char decode_byte_from_lsb(char *buffer)
{
    char ch=0;
    for(int i=0;i<8;i++ )
    {
	ch=(buffer[i] & 1) | (ch << 1);
    }
    return ch;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for decode secret file extension size*/

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    printf("Decoding secret File Extenstion Size\n");
    char buffer[32];
    fread(buffer,32,1,decInfo->fptr_src_image);
    decInfo->size_extn=decode_size_from_lsb(buffer);

    return d_success;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for decode size from lsb */

long decode_size_from_lsb(char *buffer)
{
    long size=0;

    for(int i=0;i<32;i++)
    {
	size=(buffer[i] & 1) | (size << 1);
    }	
    return size;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for decode secret file extension */

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];

    for(int i=0;i<decInfo->size_extn;i++)
    {
	fread(buffer,8,1,decInfo->fptr_src_image);

	decInfo->extn_secret_file[i] = decode_byte_from_lsb(buffer);
    }
    printf("Output File not mentioned. Creating %s%s as default\n", decInfo -> secret_fname, decInfo -> extn_secret_file);
    decInfo -> fptr_secret = fopen(strcat(decInfo -> secret_fname, (char *)decInfo -> extn_secret_file), "w");

    printf("Opened %s\n", decInfo -> secret_fname);
    if(decInfo->fptr_secret == NULL )
    {
	perror("fopen");
	fprintf(stderr,"ERROR : Unable to open file %s\n",decInfo -> secret_fname);
	return d_failure;
    }
    return d_success;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for decode file data size */

Status decode_file_data_size(DecodeInfo *decInfo)
{
    printf("Decoding %s File Size\n", decInfo -> secret_fname);
    char buffer[32];

    fread(buffer,32,1,decInfo -> fptr_src_image);

    decInfo->size_secret_file = decode_size_from_lsb(buffer);

    return d_success;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for decode file data */

Status decode_file_data(DecodeInfo *decInfo)
{
    printf("Decoding %s File Data\n", decInfo -> secret_fname);
    char buffer[8], ch;

    for (int i=0;i<decInfo->size_secret_file-1;i++)
    {
	fread(buffer,8,1,decInfo->fptr_src_image);

	ch = decode_byte_from_lsb(buffer);

	fwrite(&ch,1,1,decInfo->fptr_secret);
    }
    ch = '\n';
    fwrite(&ch,1,1,decInfo->fptr_secret);

    return d_success;
}
