/* test_make_wav.c
 * Fri Jun 18 17:13:19 PDT 2010 Kevin Karplus
 * Test program for the write_wav function in make_wav.c
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>  




#include "make_wav.h"

#define PI 3.14159265

 
int main()
{
    unsigned i = 0;
    int main_choice;
    float t = 0;
    
    char * infile_name;
    FILE * fp;

    char * buffer = malloc(sizeof(char)* 1024 * 2 * 8);


    printf("***Welcome to eeg data testing system!!!***\n");
    printf("1. read a wav file.\n") ;
    printf("2. check the data of loaded wav file.\n");
    printf("3. set a sin wav.\n");
    printf("4. write the new file\n");   
    printf("******************************************\n");
    
    
    scanf("%d", &main_choice);    
    safe_flush(stdin);

    printf("your choice: %d!\n", main_choice);
    printf("\n");

    if (main_choice == 1) {
    infile_name = "4cmotion.wav"; 
 
    if(( fp = fopen(infile_name, "rb")) == NULL){
	fprintf(stderr, "ERROR: cannot open file: %s!\n", infile_name);
	return 1;
	}

	if( fread(buffer, 1, 44, fp) != 44 ||
	    memcmp(buffer, "RIFF", 4) ||
	    memcmp(buffer + 8, "WAVEfmt ", 8) ||
	    memcmp(buffer + 36, "data", 4)
	){  
	    fprintf(stderr, "This file is not wav file file! Check the header\n");
	    fclose(fp);
	    return 1;
	}
	unsigned bps = 16;


//read chunk size

/*
	unsigned * pos = (unsigned *) malloc (4);
	fseek(fp, 4, SEEK_SET);
	fread(pos, 1, 4, fp);
	unsigned chunksize = array2data(pos, 4);
	printf("chunksize is %u\n",chunksize);
	fflush(stdout);
	fflush(stdin);
	safe_flush(stdout);
	safe_flush(stdin);
*/

//read channel number

	unsigned * pos =(unsigned *) malloc(4);
	fseek(fp, 22, SEEK_SET);
	fread(pos, 2, 1, fp);
	unsigned channel = array2data(pos,2);
        printf("channel number: %u\n", channel);
	

//read sample rate number

	fseek(fp, 24, SEEK_SET);
	fread(pos, 4, 1, fp);
	unsigned sample_rate = array2data( pos, 4);
	printf("sample rate is %u\n", sample_rate); 

//read total samples
	
	fseek(fp, 40, SEEK_SET);
	fread(pos, 1, 4, fp);
	unsigned total_samples = array2data(pos, 4) / 2;
	printf("total samples is %u\n", total_samples);
	free(pos);

	sleep(1);

//data store section
	
	short * data_store = (short *)malloc(total_samples * 4);
	fseek(fp, 44, SEEK_SET);
        if ( fread(data_store, 2, total_samples, fp) == 0)
	{   
	    printf("I can't read any data from wav file!\n");
	}

	

/*	for( i = 0; i < total_samples; i++)
	{
	    printf("The data_store[%u] is %hd\n", i, data_store[i]);
	}*/
	printf("total_samples is %u\n", total_samples);
	unsigned sub_samples = total_samples / 4;
	
	
	short * channel1_data =(short *)malloc(total_samples);
	short * channel2_data =(short *)malloc(total_samples);
	short * channel3_data =(short *)malloc(total_samples);
	short * channel4_data =(short *)malloc(total_samples);
	
	for( i = 0; i < sub_samples; i++)
	{
	    channel1_data[i] = data_store[i*4];
	    channel2_data[i] = data_store[i*4+1];
	    channel3_data[i] = data_store[i*4+2];
	    channel4_data[i] = data_store[i*4+3];  
	}
	
	for( i = 0; i < sub_samples; i++)
	{
	    printf("channel4[%u] value is:%hd\n ", i, channel4_data[i]);

	}

//find max and min value in subchannels
	
	unsigned channel1_diff = array_diff( channel1_data, sub_samples);
	unsigned channel2_diff = array_diff( channel2_data, sub_samples);
	unsigned channel3_diff = array_diff( channel3_data, sub_samples);
	unsigned channel4_diff = array_diff( channel4_data, sub_samples) ;	
    
	printf("The channel1_diff is %u, channel2 %u, channel3 %u, channel4 %u\n", channel1_diff,channel2_diff, channel3_diff, channel4_diff);

	//generate sine wave
	short * sine_data = (short *)malloc(total_samples);
	double amplitude = channel1_diff;
	//short freq = sample_rate;
	
	for(i = 0; i < sub_samples; i++)
	{
	    sine_data[i] = amplitude * sin(2 * PI * sample_rate * i);
	    printf("sine data [%u] is %hu\n", i, sine_data[i]);
	}

	printf("freq is %u\n",sample_rate);
 




}













/*int i;
    //float t;
    float amplitude = 32000;
    float freq_Hz = 440;
    float phase=0;
 
    float freq_radians_per_sample = freq_Hz*2*M_PI/S_RATE;
 */
    /* fill buffer with a sine wave */
 /*   for (i=0; i<BUF_SIZE; i++)
    {
        phase += freq_radians_per_sample;
    buffer[i] = (int)(amplitude * sin(phase));
    }
 
    write_wav("test.wav", BUF_SIZE, buffer, S_RATE);
 
    return 0;

*/


}
