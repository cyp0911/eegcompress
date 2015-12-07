/* make_wav.c
 * Creates a WAV file from an array of ints.
 * Output is monophonic, signed 16-bit samples
 * copyright
 * Fri Jun 18 16:36:23 PDT 2010 Kevin Karplus
 * Creative Commons license Attribution-NonCommercial
 *  http://creativecommons.org/licenses/by-nc/3.0/
 */
 
#include <stdio.h>
#include <assert.h>
 
#include "make_wav.h"


//data convert function

void data2array(unsigned int x, unsigned char a[], unsigned char n)
//convert real data to storage data
{
    unsigned char i;
    for(i = 0;i < n;i++)
    {
    a[i] = x & 0xff;
    x = x >> 8;
    }
}

unsigned array2data(unsigned a[], unsigned n)
//convert storage data to real data
{
    unsigned i;
    unsigned int x = 0;
    for(i = 0;i < n;i++)
    {
    x = x * 256 + a[n - 1 - i];
    }
    return x;
}
 
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
 
/* information about the WAV file format from
    http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 */
 
void write_wav(char * filename, unsigned long num_samples, short int * data, int s_rate)
{
    FILE* wav_file;
    unsigned int sample_rate;
    unsigned int num_channels;
    unsigned int bytes_per_sample;
    unsigned int byte_rate;
    unsigned long i;    /* counter for samples */
 
    num_channels = 1;   /* monoaural */
    bytes_per_sample = 2;
 
    if (s_rate<=0) sample_rate = 44100;
    else sample_rate = (unsigned int) s_rate;
 
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
    write_little_endian(bytes_per_sample* num_samples*num_channels, 4, wav_file);
    for (i=0; i< num_samples; i++)
    {   write_little_endian((unsigned int)(data[i]),bytes_per_sample, wav_file);
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


