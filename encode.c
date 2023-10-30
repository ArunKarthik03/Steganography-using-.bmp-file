
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

//function defination of read and validate
Status read_and_validate_encode_args( char *argv[], EncodeInfo *encInfo )
{
		//comparing argv[2] == .bmp file or not
		if (strcmp((strstr(argv[2],".")) ,".bmp") == 0 )
		{
				//if yes,then storing in src_image_fname
				encInfo -> src_image_fname = argv[2];
		}
		else
		{
				//if not,then return e_failure
				return e_failure;
		}
		//comparing argv[3] == .txt file or not
		if (strcmp((strstr(argv[3],".")) ,".txt") == 0 )
		{
				//if yes,then storing in secret_fname
				encInfo -> secret_fname = argv[3];
				strcpy(encInfo -> extn_secret_file, ".txt");
		}
		else
		{
				//if not,then return e_failure
				return e_failure;
		}
		//checking argv[4]
		if (argv[4] != NULL)
		{
				//if filename give,then same filename will store in stego_image_fname as filename.bmp
				encInfo ->stego_image_fname = argv[4];
		}
		else
		{
				//if not then file store ase default filename as default.bmp
				printf("Output file not mentioned.\nCreating default.bmp as default\n");
				encInfo -> stego_image_fname = "default.bmp";
		}
		//if all opearion is completed successfully,then returns e_success to test encode.c for e_encode function
		return e_success;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------

Status do_encoding(EncodeInfo *encInfo)
{
		//calling open files function
		if(open_files(encInfo) == e_success)
		{
				//if function return e_success,then print success message
				printf("** ->Encoding Procedure Started<- **\n");
				//printf("open files is success\n");
				//calling check capacity function
				if(check_capacity(encInfo) == e_success)
				{
						//if function retun e_success,then print success messgae
						printf("check capacity successfully completed...\n");
						//calling copy bmp header function
						if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
						{
								//if function return e_success,then print success message
								printf("copy bmp header successfully completed...\n");
								//calling encode magic string function
								if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
								{
										//if function return e_success,then print success message
										printf("encode maagic string successfully completed...\n");
										//calling encode secrte file extension size function
										if(encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo) == e_success)
										{
												//if function return e_success,then print success message
												printf("encode secret file extension file size successfully completed...\n");
												//calling encode secret file extension function
												if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
												{
														//if function return e_success,then print success message
														printf("encode secret file extension successfully completed...\n");
														//calling encode secret file size function
														if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
														{
																//if function return e_success,then print success message
																printf("encode secret file size successfully completed...\n");
																//calling encode secret file data function
																if(encode_secret_file_data(encInfo) == e_success)
																{  
																		//if function return e_success,then print success message
																		printf("encode secret file data successfully completed...\n");
																		//calling copy remaininng image dataa function
																		if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
																		{
																				//if function return e_success,then print success message
																				printf("copy remaining image data successfully completd...\n");
																		}
																		else
																		//if any one of above function returns e_failure,then print error message and return e_failure to test encode.c for encode  operation
																		{
																				printf("Failed to copy remaining data\n");
																				return e_failure;
																		}
																}
																else
																{
																		printf("Failed to encode secret file data\n");
																		return e_failure;
																}

														}
														else
														{
																printf("Encode secret file size was a failure\n");
																return e_failure;
														}
												}
												else
												{
														printf("Encode secret file extenstion was failure\n");
														return e_failure;
												}
										}
										else
		    {
			printf("Encode secret file extenstion size was a failure\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("Magic string was not encoded\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Couldn't copy the bmp header\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Check capacity is a failure\n");
	    return e_failure;
	}
    }
	
    else
    {
	printf("open file function is a failure\n");
	return e_failure;
    }
	//if all above function is successfully completed,then return e_success to  test encode.c for encode operation
    return e_success;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------  

/* Function defination for check capacity */

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

    printf("Checking for secret.txt size\n");
    if (encInfo -> size_secret_file)
    {
	printf("Done. Not Empty\n");
    }

    printf("Checking for beautiful.bmp capacity to handle secret.txt\n");
    if (encInfo -> image_capacity > (54 + (2 + 4 + 4 + 4 + encInfo -> size_secret_file) * 8))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for get file size */

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for copy bmp header */

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    printf("Copying Image Header\n");
    char str[54];
    fseek(fptr_src_image, 0, SEEK_SET);
    fread(str, 54, 1, fptr_src_image);
    fwrite(str, 54, 1, fptr_dest_image);
    return e_success;
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for encode magic string */

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("Encoding Magic String Signature\n");
    encode_data_to_image((char *)magic_string, strlen(magic_string), encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
    return e_success;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for encode sata to image */

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    int i;
    for ( i = 0 ; i < size ; i++ )
    {
	fread(buffer, 8, 1, fptr_src_image);
	encode_byte_to_lsb(data[i], buffer);
	fwrite(buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for encode byte to lsb */

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i;

    for ( i = 0 ; i < 8 ; i++ )
    {
	image_buffer[i] = (image_buffer[i] & 0xFE) | (((unsigned) data >> (7 - i)) & 1);
    }
    return e_success;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* eFunction defination for encode secret file extension size*/

Status encode_secret_file_extn_size(long file_size, EncodeInfo *encInfo)
{
    printf("Encoding secret.txt File Extenstion size\n");
    char buffer[32];

    fread(buffer, 32, 1, encInfo -> fptr_src_image);
    encode_int_to_lsb(file_size, buffer);
    fwrite(buffer, 32, 1, encInfo -> fptr_stego_image);

    return e_success;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for encode int to lsb*/

Status encode_int_to_lsb( long data, char *image_buffer)
{
    int i;

    for ( i = 0 ; i < 32 ; i++ )
    {
	image_buffer[i] =  (image_buffer[i] & 0xFE) | (((unsigned) data >> (31 - i)) & 1);
    }
    return e_success;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for encode decret file extension */

Status encode_secret_file_extn( const char *file_extn, EncodeInfo *encInfo)
{
    printf("Encodig secret.txt File Extenstion\n");
    char buffer[8];

    int i;
    for ( i = 0 ; i < strlen(file_extn) ; i++ )
    {
	fread(buffer, 8, 1, encInfo -> fptr_src_image);
	encode_byte_to_lsb(file_extn[i], buffer);
	fwrite(buffer, 8, 1, encInfo -> fptr_stego_image);
    }
    return e_success;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination encode secret file size */

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    printf("Encoding secret.txt File Size\n");
    char buffer[32];

    fread(buffer, 32, 1, encInfo -> fptr_src_image);
    encode_int_to_lsb(file_size, buffer);
    fwrite(buffer, 32, 1, encInfo -> fptr_stego_image);

    return e_success;
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for encode secret file data */

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("Encoding secret.txt File Data\n");
    char buffer[8];

    fseek(encInfo -> fptr_secret, 0, SEEK_SET);

    for (int i = 0 ; i < (encInfo -> size_secret_file - 1) ; i++ )
    {
	fread(buffer, 8, 1, encInfo -> fptr_src_image);
	fread(&(encInfo -> secret_data[0]), 1, 1, encInfo -> fptr_secret);
	encode_byte_to_lsb(encInfo -> secret_data[0], buffer);
	fwrite(buffer, 8, 1, encInfo -> fptr_stego_image);
    }
    return e_success;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* Function defination for copy remaining image data */

Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    printf("Copying Left Over Data\n");
    char ch;
    while (fread(&ch, 1, 1, fptr_src_image) > 0)
    {
	fwrite(&ch, 1, 1, fptr_stego_image);
    }
    return e_success;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

 /*
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

/* Function defination for get image size for bmp */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

/* Function defination for open files */

Status open_files(EncodeInfo *encInfo)
{
    printf("Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }
    printf("Opened %s\n", encInfo -> src_image_fname);

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }
    printf("Opened %s\n", encInfo -> secret_fname);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }
    printf("Opened %s\n", encInfo -> stego_image_fname);
    printf("Done\n");

    // No failure return e_success
    return e_success;
}


