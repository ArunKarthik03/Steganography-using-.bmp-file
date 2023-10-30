#ifndef DECODE_H
#define DECODE_H


#include "types.h"

#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    char *src_image_fname;
    FILE *fptr_src_image;

    char *secret_fname;
    long size_extn;
    char extn_secret_file[MAX_FILE_SUFFIX];
    FILE *fptr_secret;
    long size_secret_file;

}DecodeInfo;

/* Read and validate decode args and argv */
Status read_and_validate_decode_argv(char *argv[], DecodeInfo *decInfo);

/* Perform decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get file pointers for input and oupt files */
Status open_img_file(DecodeInfo *decInfo);

/* Magic string of decoding */
Status decode_magic_string(FILE *fptr_img);

/* Decode bte to lsb */
char decode_byte_from_lsb(char *buffer);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* decode size to lsb */
long decode_size_from_lsb(char *buffer);

/* decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode file data size */ 
Status decode_file_data_size(DecodeInfo *decInfo);

/* decode file data */
Status decode_file_data(DecodeInfo *decInfo);

#endif
