/* operator_wav.c 
 * This file contains several operations for eeg datas in   
 * my compress_wav.c
 * 2015 Dec 7 Yinpeng Chen, Acadia University
 * MSc student in Computer Science: EEG data compression
 */




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h> 
#include <math.h>
#include "operator_wav.h"

#define PI 3.14159265


//convert data to little endian
void data2array(unsigned int x, unsigned char a[], unsigned char n)
{
    unsigned char i;
    for(i = 0;i < n;i++)
    {
    a[i] = x & 0xff;
    x = x >> 8;
    }
}

//convert little endian data to real data
unsigned array2data(unsigned a[], unsigned n)
{
    unsigned i;
    unsigned int x = 0;
    for(i = 0;i < n;i++)
    {
    x = x * 256 + a[n - 1 - i];
    }
    return x;
}

//store the data as little endian 
void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file)
{
    unsigned buf;
    while(num_bytes>0)
    {   buf = word & 0xff;
        fwrite(&buf, 1,1, wav_file);
        num_bytes--;
    word >>= 8;
    }
}
 
 
void write_wav(char * filename, unsigned num_samples, short * data, unsigned sample_rate, unsigned num_channels)
{
    FILE* wav_file;
    unsigned int bytes_per_sample;
    unsigned int byte_rate;
    unsigned long i;    /* counter for samples */
 
    bytes_per_sample = 2;
 
 
    byte_rate = sample_rate*num_channels*bytes_per_sample;
 
    wav_file = fopen(filename, "w");
    assert(wav_file);   /* make sure it opened */
 
    /* write RIFF header */
    fwrite("RIFF", 1, 4, wav_file);
    write_little_endian(36 + bytes_per_sample* num_samples*num_channels, 4, wav_file);
    fwrite("WAVE", 1, 4, wav_file);
 
    /* write fmt  subchunk */
    fwrite("fmt ", 1, 4, wav_file);
    write_little_endian(16, 4, wav_file);   /* SubChunk1Size is 16 */
    write_little_endian(1, 2, wav_file);    /* PCM is format 1 */
    write_little_endian(num_channels, 2, wav_file);
    write_little_endian(sample_rate, 4, wav_file);
    write_little_endian(byte_rate, 4, wav_file);
    write_little_endian(num_channels*bytes_per_sample, 2, wav_file);  /* block align */
    write_little_endian(8*bytes_per_sample, 2, wav_file);  /* bits/sample */
 
    /* write data subchunk */
    fwrite("data", 1, 4, wav_file);
    write_little_endian(bytes_per_sample * num_samples * num_channels, 4, wav_file);
    for (i=0; i< num_samples * num_channels; i++)
    {   write_little_endian(data[i],bytes_per_sample, wav_file);
    }
 
    fclose(wav_file);
}


//flush the stdin
void safe_flush(FILE *fp){
    int ch;
    while( (ch = fgetc(fp)) != EOF && ch != '\n' );
}

//find the biggest difference value in an array
int array_diff(short channelarray[], unsigned size)
{
    int i;
    int max = channelarray[0];
    int min = channelarray[0];
    for( i = 0; i < size; i++)
    {
	if(channelarray[i] > max) max = channelarray[i];
	if(channelarray[i] < min) min = channelarray[i];
    }
    unsigned diff = (max - min);
    return diff;
}

//compute the best coefficient of sine function
struct sin_coeff find_best_coeff(short channel1[], unsigned channel_diff, unsigned sub_samples, unsigned sample_rate)
{
	int i = 0, j = 0, k = 0;
	int minErrdiff1, minj1, mink1;
	struct sin_coeff r;
	unsigned total_samples = sub_samples * 4;
	short * error = (short *) malloc (total_samples);	    //store the data of error(subchannel - sine_data)
	short * sine = (short *) malloc (total_samples);
	
	/*int minA, maxA, minO, maxO;
	printf("please enter the range of coefficient for sine wave:<usage: minimum-amptitude  maximum-amptitude  minimum-Omega  maximum Omega.>\n");
	scanf("%d %d %d %d", &minA, &maxA, &minO, &maxO);
	printf("your setting is:minA%d,maxA%d,mimO%d,max%d.\n",minA,maxA,minO,maxO);
	fflush(stdin);*/
    for (k = -40; k < 40; k++){
      for(j = (-40); j < 40 ; j++){
	for(i = 0; i < sub_samples; i++)
	{
	    sine[i] = 0.05 * j * channel_diff * sin( 2 * PI * sample_rate * i * k); 
	    error[i] = channel1[i] - sine[i];
	}// j domin
	    if (j == (-20) && k == 1){minErrdiff1 = array_diff( error, sub_samples);}
	    else if ( minErrdiff1 > array_diff( error, sub_samples)){
		 minErrdiff1 = array_diff( error, sub_samples);
		 minj1 = j; mink1 = k;

	    }	
		}//k domin  
	}

	printf("Errintdiff is %d, minA is %d, minOmega is %d\n", minErrdiff1, minj1, mink1);
	r.A = minj1;
	r.omega = mink1;

	return r;
}

