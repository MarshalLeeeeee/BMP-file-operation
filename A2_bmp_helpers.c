/* FILE: A2_bmp_helpers.c is where you will code your answers for Assignment 2.
 * 
 * Each of the functions below can be considered a start for you. 
 *
 * You should leave all of the code as is, except for what's surrounded
 * in comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A2_bmp_headers.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define MIN(x,y) x-y<0?x:y
#define MAX(x,y) x-y>0?x:y

int bmp_open( char* bmp_filename,        unsigned int *width, 
              unsigned int *height,      unsigned int *bits_per_pixel, 
              unsigned int *padding,     unsigned int *data_size, 
              unsigned int *data_offset, unsigned char** img_data ){

              
  /* YOUR CODE FOR Q1 SHOULD REPLACE EVERYTHING FROM HERE
  printf( "BMP_OPEN is not yet implemented. Please help complete this code!\n" );
  *width=0;
  *height=0;
  *bits_per_pixel=0;
  *padding=0;
  *data_size=0;
  *data_offset=0;
  *img_data=NULL;
   TO HERE */

	FILE *fp=fopen(bmp_filename,"rb");
	if(fp==0) return 0; 

	fseek(fp, 0x12, 0);
	fread(width, 4, 1, fp);

	fseek(fp, 0x16, 0);
	fread(height, 4, 1, fp);

	fseek(fp, 0x1C, 0);
	fread(bits_per_pixel, 4, 1, fp);

	fseek(fp, 0x02, 0);
	fread(data_size, 4, 1, fp);

	fseek(fp, 0x0A, 0);
	fread(data_offset, 4, 1, fp);

	*padding = ((*data_size - *data_offset) / (*height)) - ((*width) * ((*bits_per_pixel) / 8));

	char* bmap;
	bmap = (char *)malloc(*data_size);
	fseek(fp, 0, 0);
	fread(bmap, sizeof(char), *data_size, fp);
	*img_data = bmap;

	//printf("width: %u %u %u %u\n", bmap[0x12], bmap[0x13], bmap[0x14], bmap[0x15]);
	//printf("*******************************\n");
	/*
	printf("%s: width: %d and height: %d\n", bmp_filename, *width, *height);
	printf("%s: actual data_size: %d\n", bmp_filename, *data_size);
	printf("%s: actual data_offset: %d\n", bmp_filename, *data_offset);
	printf("%s: actual padding: %d\n", bmp_filename, *padding);
	printf("%s: actaul color byte: %d\n",bmp_filename, (*bits_per_pixel) / 8);
	printf("%s: actaul width byte: %d\n",bmp_filename, (*width) * ((*bits_per_pixel) / 8));
	printf("%s: actaul width+padding byte: %d\n",bmp_filename, ((*data_size - *data_offset) / (*height)));
	*/

	fclose(fp);

	return 0;  
}

// We've implemented bmp_close for you. No need to modify this function
void bmp_close( unsigned char **img_data ){

  if( *img_data != NULL ){
    free( *img_data );
    *img_data = NULL;
  }
}

int bmp_mask( char* input_bmp_filename, char* output_bmp_filename, 
              unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max,
              unsigned char red, unsigned char green, unsigned char blue )
{
  unsigned int img_width;
  unsigned int img_height;
  unsigned int bits_per_pixel;
  unsigned int data_size;
  unsigned int padding;
  unsigned int data_offset;
  unsigned char* img_data    = NULL;
  
  int open_return_code = bmp_open( input_bmp_filename, &img_width, &img_height, &bits_per_pixel, &padding, &data_size, &data_offset, &img_data ); 
  
  if( open_return_code ){ printf( "bmp_open failed. Returning from bmp_mask without attempting changes.\n" ); return -1; }
 
  // YOUR CODE FOR Q2 SHOULD REPLACE EVERYTHING FROM HERE

  	//unsigned char *pixel_data = img_data + data_offset;
  	unsigned int num_colors = bits_per_pixel/8;
  	printf("row: %d  col: %d\n", img_height, img_width);
  	
	for(int i = y_min; i <= y_max; i++)
	{
		for(int j = x_min; j<= x_max; j++)
		{
			img_data[ i*(img_width*num_colors+padding) + j*num_colors + 2 + data_offset] = red;
			img_data[ i*(img_width*num_colors+padding) + j*num_colors + 1 + data_offset] = green;
			img_data[ i*(img_width*num_colors+padding) + j*num_colors + 0 + data_offset] = blue;

			//pixel_data[ i*(img_width*num_colors+padding) + i*num_colors + 2] = red;
			//pixel_data[ i*(img_width*num_colors+padding) + i*num_colors + 1] = green;
			//pixel_data[ i*(img_width*num_colors+padding) + i*num_colors + 0] = blue;
		}
	}
	
	FILE *fout=fopen(output_bmp_filename, "wb");
	fwrite(img_data, sizeof(char), data_size, fout);
  // TO HERE!
  
  bmp_close( &img_data );
  
  return 0;
}         

int bmp_collage( char* bmp_input1, char* bmp_input2, char* bmp_result, int x_offset, int y_offset ){

  unsigned int img_width1;
  unsigned int img_height1;
  unsigned int bits_per_pixel1;
  unsigned int data_size1;
  unsigned int padding1;
  unsigned int data_offset1;
  unsigned char* img_data1    = NULL;
  
  int open_return_code = bmp_open( bmp_input1, &img_width1, &img_height1, &bits_per_pixel1, &padding1, &data_size1, &data_offset1, &img_data1 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input1 ); return -1; }
  
  unsigned int img_width2;
  unsigned int img_height2;
  unsigned int bits_per_pixel2;
  unsigned int data_size2;
  unsigned int padding2;
  unsigned int data_offset2;
  unsigned char* img_data2    = NULL;
  
  open_return_code = bmp_open( bmp_input2, &img_width2, &img_height2, &bits_per_pixel2, &padding2, &data_size2, &data_offset2, &img_data2 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input2 ); return -1; }
  
  // YOUR CODE FOR Q3 SHOULD REPLACE EVERYTHING FROM HERE

  unsigned int num_colors1 = bits_per_pixel1/8;
  unsigned int num_colors2 = bits_per_pixel2/8;

  	int img_left = MIN(0,x_offset);
  	int img_down = MIN(0,y_offset);
  	int img_right = MAX(img_width1, (int)(x_offset+(int)img_width2));
  	int img_up = MAX(img_height1, (int)(y_offset+img_height2));
  	unsigned int img_width = img_right - img_left;
  	unsigned int img_height = img_up - img_down;
  	unsigned int img_x_offset = MAX(0,-x_offset);
  	unsigned int img_y_offset = MAX(0,-y_offset);
  	unsigned int data_offset = data_offset1;
  	unsigned int num_colors = num_colors1;
  	unsigned int padding = (4 - (img_width*num_colors) % 4) % 4;
  	unsigned int data_size = data_offset + img_height *(img_width * num_colors + padding);
  	unsigned char* img_data = (unsigned char*)malloc(data_offset + img_height *(img_width * num_colors + padding));
/*
  	printf("%s should left: %d and right: %d\n",bmp_result ,img_left, img_right);
  	printf("%s Should width: %d, height: %d\n",bmp_result, img_width, img_height);
  	printf("%s Should img_x_offset: %d, img_y_offset: %d\n",bmp_result, img_x_offset, img_y_offset);
  	printf("%s Should padding: %d\n",bmp_result, padding);
  	printf("%s Should data_size: %d\n",bmp_result, data_size);
  	*/
/*
  if(x_offset + img_width2 <= img_width1 && y_offset + img_height2 <= img_height1)
  //if(1)
  {
  	for(int j = x_offset; j < x_offset + img_width2; j++)
	{
		for(int i = y_offset; i < y_offset + img_height2; i++)
		{
			//img_data1[ i*(img_width1*num_colors1+padding1) + j*num_colors1 + 2 + data_offset1] = 0;
			//img_data1[ i*(img_width1*num_colors1+padding1) + j*num_colors1 + 1 + data_offset1] = 0;
			//img_data1[ i*(img_width1*num_colors1+padding1) + j*num_colors1 + 0 + data_offset1] = 0;

			img_data1[ i*(img_width1*num_colors1+padding1) + j*num_colors1 + 2 + data_offset1] = img_data2[ (i-y_offset)*(img_width2*num_colors2+padding2) + (j-x_offset)*num_colors2 + 2 + data_offset2];
			img_data1[ i*(img_width1*num_colors1+padding1) + j*num_colors1 + 1 + data_offset1] = img_data2[ (i-y_offset)*(img_width2*num_colors2+padding2) + (j-x_offset)*num_colors2 + 1 + data_offset2];
			img_data1[ i*(img_width1*num_colors1+padding1) + j*num_colors1 + 0 + data_offset1] = img_data2[ (i-y_offset)*(img_width2*num_colors2+padding2) + (j-x_offset)*num_colors2 + 0 + data_offset2];
			//pixel_data[ i*(img_width*num_colors+padding) + i*num_colors + 2] = red;
			//pixel_data[ i*(img_width*num_colors+padding) + i*num_colors + 1] = green;
			//pixel_data[ i*(img_width*num_colors+padding) + i*num_colors + 0] = blue;
		}
	}
	FILE *fout=fopen(bmp_result, "wb");
	fwrite(img_data1, sizeof(char), data_size1, fout);
  }
*/  
  //else
  //{
  	/*
  	if(x_offset < 0)
  	{
  		;
  	}
  	if(y_offset < 0)
  	{
  		;
  	}
  	*/
/*
  	unsigned int img_left = MIN(0,x_offset);
  	unsigned int img_down = MIN(0,y_offset);
  	unsigned int img_right = MAX(img_width1, x_offset+img_width2);
  	unsigned int img_up = MAX(img_height1, y_offset+img_height2);
  	unsigned int img_width = img_right - img_left;
  	unsigned int img_height = img_up - img_down;
  	unsigned int img_x_offset = MAX(0,-x_offset);
  	unsigned int img_y_offset = MAX(0,-y_offset);

  	unsigned int data_size = 54 + img_width * img_height;
  	unsigned char* img_data = (unsigned char*)malloc(data_size1);
  	unsigned int num_colors = num_colors1;
  	unsigned int data_offset = data_offset1;
  	unsigned int padding = (4 - (img_width*num_colors) % 4) % 4;
  */
  	/*
  	fread(img_data+0x12, 4, 1, &img_width);
  	fread(img_data+0x16, 4, 1, &img_height);
  	fread(img_data+0x02, 4, 1, &data_size);
	*/

  	for(unsigned int i = 0; i < data_offset1; i++)
  	{
  		img_data[i] = img_data1[i];
  	}

  	img_data[0x02] = (unsigned char)(data_size & 0xFF);
  	img_data[0x03] = (unsigned char)((data_size >> 8) & 0xFF);
  	img_data[0x04] = (unsigned char)((data_size >> 16) & 0xFF);
  	img_data[0x05] = (unsigned char)((data_size >> 24) & 0xFF);

  	img_data[0x12] = (unsigned char)(img_width & 0xFF);
  	img_data[0x13] = (unsigned char)((img_width >> 8) & 0xFF);
  	img_data[0x14] = (unsigned char)((img_width >> 16) & 0xFF);
  	img_data[0x15] = (unsigned char)((img_width >> 24) & 0xFF);
  	//printf("After altering: %d %d %d %d\n", img_data[0x12], img_data[0x13], img_data[0x14], img_data[0x15]);

  	img_data[0x16] = (unsigned char)(img_height & 0xFF);
  	img_data[0x17] = (unsigned char)((img_height >> 8) & 0xFF);
  	img_data[0x18] = (unsigned char)((img_height >> 16) & 0xFF);
  	img_data[0x19] = (unsigned char)((img_height >> 24) & 0xFF);
  	//printf("After altering: %d %d %d %d\n", img_data[0x16], img_data[0x17], img_data[0x18], img_data[0x19]);


  	for(int j = 0; j < img_height; j++)
  	{
  		for(int i = 0; i < img_width; i++)
  		{
  			if(x_offset + img_x_offset <= i && i < x_offset + img_width2 + img_x_offset
  			 &&y_offset + img_y_offset <= j && j < y_offset + img_height2 + img_y_offset) // up pic
  			{
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 2 + data_offset] = img_data2[(j - y_offset - img_y_offset)*(img_width2*num_colors2+padding2) + (i - x_offset - img_x_offset)*num_colors2 + 2 + data_offset2];
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 1 + data_offset] = img_data2[(j - y_offset - img_y_offset)*(img_width2*num_colors2+padding2) + (i - x_offset - img_x_offset)*num_colors2 + 1 + data_offset2];
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 0 + data_offset] = img_data2[(j - y_offset - img_y_offset)*(img_width2*num_colors2+padding2) + (i - x_offset - img_x_offset)*num_colors2 + 0 + data_offset2];
  			}
  			else if (img_x_offset <= i && i < img_width1 + img_x_offset
  			 	   &&img_y_offset <= j && j < img_height1 + img_y_offset) // down pic
  			{
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 2 + data_offset] = img_data1[(j - img_y_offset)*(img_width1*num_colors1+padding1) + (i - img_x_offset)*num_colors1 + 2 + data_offset1];
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 1 + data_offset] = img_data1[(j - img_y_offset)*(img_width1*num_colors1+padding1) + (i - img_x_offset)*num_colors1 + 1 + data_offset1];
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 0 + data_offset] = img_data1[(j - img_y_offset)*(img_width1*num_colors1+padding1) + (i - img_x_offset)*num_colors1 + 0 + data_offset1];
  			}
  			else // space
  			{
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 2 + data_offset] = 255;
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 1 + data_offset] = 255;
  				img_data[j*(img_width*num_colors+padding) + i*num_colors + 0 + data_offset] = 255;
  			}
  		}
  		for(int k = 0; k < padding; k++) // for padding
  		{
  			img_data[j*(img_width*num_colors+padding) + (img_width)*num_colors + k + data_offset] = 0;
  		}
  		
  	}

  	FILE *fout=fopen(bmp_result, "wb");
  	fwrite(img_data, sizeof(char), data_size, fout);
  //}
  
  
  // TO HERE!
      
  bmp_close( &img_data1 );
  bmp_close( &img_data2 );
  
  return 0;
}

