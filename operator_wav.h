/* operator.h
 * Dec 07 2015, Yinpeng Chen
 * Acadia University
 */
 
#ifndef OPERATOR_WAV_H
#define OPERATOR_H
 
void write_wav(char * filename, unsigned long num_samples, short int * data, int s_rate);
    /*write data from arry to a wav file */

void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file);
    /* write data to wav file stored by little_endian */

void safe_flush(FILE *fp);
    /* refresh the stdin point. */

void data2array(unsigned int x, unsigned char a[], unsigned char n);
    /* convert real data to storage format */

unsigned array2data(unsigned a[], unsigned n); 
    /* convert storage data to int format*/

int array_diff(short channelarray[], unsigned size);
    /*compute biggest amplitude in an array*/
#endif
