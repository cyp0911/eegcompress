/* make_wav.h
 * Fri Jun 18 17:06:02 PDT 2010 Kevin Karplus
 */
 
#ifndef MAKE_WAV_H
#define MAKE_WAV_H
 
void write_wav(char * filename, unsigned long num_samples, short int * data, int s_rate);
    /* open a file named filename, write signed 16-bit values as a
        monoaural WAV file at the specified sampling rate
        and close the file
    */

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
