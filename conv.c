#include "stdio.h"
#include <stdlib.h>
#include "assert.h"
#include <string.h>
//#include "time.h"
#include <sys/time.h>
#include <unistd.h>

#define NTRIALS 20

#define FILTER_BYTES 10*sizeof(char)

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;

typedef struct {
  uchar R;
  uchar G;
  uchar B;
} pixel;


char* alloc_filter() {
  char* filt = (char*) malloc(FILTER_BYTES);
  assert(filt);
  return filt;
}

char* duplicate_filter(char* src) {
  char* dst = alloc_filter();
  memcpy(dst, src, FILTER_BYTES);
  return dst;
}

void print_filter(char* filt) {
  printf("%3d %3d %3d\n",filt[0],filt[1],filt[2]);
  printf("%3d %3d %3d  / %d\n",filt[3],filt[4],filt[5],filt[9]);
  printf("%3d %3d %3d\n",filt[6],filt[7],filt[8]);
}

char* random_filter() {
  char* k = alloc_filter();

  switch (rand() % 4) {
  case 0:  // identity
    k[0] = 0; k[1] = 0; k[2] = 0;
    k[3] = 0; k[4] = 1; k[5] = 0;
    k[6] = 0; k[7] = 0; k[8] = 0;
    k[9] = 1;
    return k;
  case 1:  // edge detection
    k[0] = -1; k[1] = -1; k[2] = -1;
    k[3] = -1; k[4] =  8; k[5] = -1;
    k[6] = -1; k[7] = -1; k[8] = -1;
    k[9] = 1;
    return k;
  case 2:  // sharpen
    k[0] =  0; k[1] = -1; k[2] =  0;
    k[3] = -1; k[4] =  5; k[5] = -1;
    k[6] =  0; k[7] = -1; k[8] =  0;
    k[9] = 1;
    return k;
  case 3:  // gaussian blur
    k[0] =  1; k[1] =  2; k[2] =  1;
    k[3] =  2; k[4] =  4; k[5] =  2;
    k[6] =  1; k[7] =  2; k[8] =  1;
    k[9] = 16;
    return k;
  default: 
    assert(0);
  }  
  return 0;
}

void print_image(pixel* img, uint nrows, uint ncols) {
  for (int i = 0; i < nrows*ncols; i++) {
    printf("(%3d,%3d,%3d) ",img[i].R,img[i].G,img[i].B);
    if ((i+1) % ncols == 0) { 
      printf("\n");
    }
  }
}

pixel* alloc_image(uint nrows, uint ncols) {
  pixel* img = (pixel*) malloc(nrows * ncols * sizeof(pixel));
  assert(img);
  return img;
}

pixel* duplicate_image(pixel* src, uint nrows, uint ncols) {
  pixel* dst = alloc_image(nrows,ncols);
  assert(dst);
  memcpy(dst, src, nrows*ncols*sizeof(pixel));
  return dst;
}

pixel* random_image(uint nrows, uint ncols) {
  pixel* img = alloc_image(nrows,ncols);
  assert(img);
  for (int i = 0; i < nrows*ncols; i++) {
    img[i].R = (uchar) rand();
    img[i].G = (uchar) rand();
    img[i].B = (uchar) rand();
  }
  return img;
}

void conv_ref(uint nrows, uint ncols, pixel* in, char* filt, pixel* out) {
  for (int c = 0; c < ncols; c++) {
    for (int r = 0; r < nrows; r++) {
      uint sum_R = 0;
      uint sum_G = 0;
      uint sum_B = 0;

      for (int dc = -1; dc <= 1; dc++) {
	for (int dr = -1; dr <= 1; dr++) {
	  int cc = c+dc;
	  int rr = r+dr;
	  if (rr >= 0 && cc >= 0 && rr < nrows && cc < ncols) {
	    uint x = ncols*rr+cc;
	    uint y = 3*(dc+1)+(dr+1);
	    assert(x < ncols*nrows);
	    assert(y < FILTER_BYTES);
	    sum_R += in[x].R * filt[y];
	    sum_G += in[x].G * filt[y];
	    sum_B += in[x].B * filt[y];
	  }
	}
      }

      uint x = ncols*r+c;
      uchar d = filt[FILTER_BYTES-1];
      out[x].R = (float) sum_R / d;
      out[x].G = (float) sum_G / d;
      out[x].B = (float) sum_B / d;

    }
  }
}

void conv_opt(uint nrows, uint ncols, pixel* in, char* filt, pixel* out) {
  #include "opt.h"
}

ulong curr_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  ulong ms = tv.tv_usec;
  ms /= 1000;
  ms += (tv.tv_sec * 1000);
  return ms;
}


int main(int argc, char** argv) {
  // check command line
  if (argc < 4 || argc%2) {
    printf("USAGE: %s <seed> <nrows1> <ncols2> <nrows2> <ncols2> ...\n",argv[0]);
    exit(-1);
  }

  // seed random number generator
  srand(atoi(argv[1]));

  // helpers to compute the average speedup
  float speedup_sum = 0.0;
  float nimages = 0;
  printf("\t\tRef\tOpt\tSpeedup\n");

  // for each image size, measure implementations and report speedup
  for (int i = 2; i < argc; i += 2) {
    int nrows = atoi(argv[i]);
    int ncols = atoi(argv[i+1]);

    uint image_bytes = nrows*ncols*sizeof(pixel);

    unsigned long sum_ref = 0;
    unsigned long sum_opt = 0;
    ulong start, stop;
    for (int j = 0; j < NTRIALS; j++) {

      // create random input image, one copy for each version
      pixel* in_ref = random_image(nrows,ncols);
      pixel* in_opt = duplicate_image(in_ref,nrows,ncols);
      assert(memcmp(in_ref,in_opt,image_bytes) == 0);

      // pick random filter, one copy for each version
      char* filt_ref = random_filter();
      char* filt_opt = duplicate_filter(filt_ref);
      assert(memcmp(filt_ref,filt_opt,FILTER_BYTES) == 0);

      // allocate output for each version
      pixel* out_ref = alloc_image(nrows,ncols);
      pixel* out_opt = alloc_image(nrows,ncols);

      // run reference 
      start = curr_ms();
      conv_ref(nrows,ncols,in_ref,filt_ref,out_ref);
      stop = curr_ms();
      sum_ref += (stop-start);

      // run optimized
      start = curr_ms();
      conv_opt(nrows,ncols,in_opt,filt_opt,out_opt);
      stop = curr_ms();
      sum_opt += (stop-start);

      // check correctness of the optimized version 
      assert(memcmp(in_ref,in_opt,image_bytes) == 0);
      assert(memcmp(filt_ref,filt_opt,FILTER_BYTES) == 0);
      assert(memcmp(out_ref,out_opt,image_bytes) == 0);

      // free memory
      free(in_ref);
      free(in_opt);
      free(filt_ref);      
      free(filt_opt);      
      free(out_ref);
      free(out_opt);
    }
    
    // report times and speedup
    ulong ref_ms = sum_ref/NTRIALS;
    ulong opt_ms = sum_opt/NTRIALS;
    float speedup = (float)ref_ms/opt_ms;
    printf("%4d x %4d\t%lu\t%lu\t%.2f\n",nrows,ncols,ref_ms,opt_ms,speedup);

    // accmulate average
    speedup_sum += speedup;
    nimages++;
  }
  
  printf("Average speedup: %.2f\n", speedup_sum/nimages);
  return 0;
}
