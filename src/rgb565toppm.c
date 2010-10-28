#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Reference
// http://netpbm.sourceforge.net/doc/ppm.html

int main(int argc, char* argv[]) {

  char* infilename;
  char* outfilename;
  FILE* infile;
  FILE* outfile;
  unsigned char red, green, blue; // 8-bits each
  //unsigned int rgb;
  unsigned short pixel; // 16-bits per pixel
  unsigned int maxval; // max color val
  unsigned short width, height;
  int depth;
  size_t i;

  // Parse Args
  if (argc < 6) {
    printf("Usage: %s infile width height depth outfile.\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  infilename = argv[1];
  outfilename = argv[5];

  width = atoi(argv[2]);
  height = atoi(argv[3]);
  depth = atoi(argv[4]);
  maxval = pow(2, depth) - 1;

  if (maxval <= 0 || maxval >= 65536) {
    printf("Err: depth must be between 1 and 65535");
    exit(EXIT_FAILURE);
  }

  // Open appropriate files
  infile = fopen(infilename, "r");
  outfile = fopen(outfilename, "wb");

  // P3 - PPM "plain" header
  fprintf(outfile, "P3\n#created with rgb565toppm\n%d %d\n%d\n", width, height, maxval);

  for (i = 0; i < width * height; i += 1) {
      fread(&pixel, sizeof(unsigned short), 1, infile);

      red = (unsigned short)((pixel & 0xF800) >> 11);  // 5
      green = (unsigned short)((pixel & 0x07E0) >> 5); // 6
      blue = (unsigned short)(pixel & 0x001F);         // 5

      // Increase intensity and make rgb888
      red = red << 3;
      green = green << 2;
      blue = blue << 3;

    //fwrite(rgb, 1, sizeof(unsigned short), outfile);
    fprintf(outfile, "%d %d %d\n", red, green, blue);
  }
  return 0;
}
