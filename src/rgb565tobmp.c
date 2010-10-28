#include <stdio.h>
#include <stdlib.h>
#include <bmpfile.h>

/*
 test
dd if=/dev/zero bs=9215 count=1 of=blah
raw565tobmp blah 24 24 16 file.bmp 
infile dimensions don't match the size you supplied
dd if=/dev/zero bs=9216 count=1 of=blah
 */
int main(int argc, char **argv)
{
  bmpfile_t *bmp;
  int i, j;
  char* infilename;
  FILE* infile;
  char* outfile;
  int width;
  int height;
  int depth;
  unsigned char red, green, blue; // 8-bits each
  unsigned short pixel; // 16-bits per pixel
  //rgb_pixel_t bpixel = {128, 64, 0, 0};
//make && ./raw565tobmp fb.rgb565 720 480 32 fb.bmp && gnome-open fb.bmp

  if (argc < 6) {
    printf("Usage: %s infile width height depth outfile.\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  infilename = argv[1];
  outfile = argv[5];

  infile = fopen(infilename, "rb");
  if (NULL == infile) {
    perror("Couldn't read infile");
    exit(EXIT_FAILURE);
  }

  width = atoi(argv[2]);
  height = atoi(argv[3]);
  depth = atoi(argv[4]);

  // should be depth/8 at 16-bit depth, but 32-bit depth works better
  short buffer[height*width*(depth/16)];
  printf("depth: %d", depth);
  if (fread(&buffer, 1, height*width*(depth/16), infile) != height*width*(depth/16)) {
    fputs("infile dimensions don't match the size you supplied\n", stderr);
  }
  printf("depth: %d", depth);

  if ((bmp = bmp_create(width, height, depth)) == NULL) {
    printf("Invalid depth value: '%d'. Try 1, 4, 8, 16, 24, or 32.\n", depth);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < width; ++i) { // 720
    for (j = 0; j < height; ++j ) { // 480
      pixel = buffer[width*j+i];

      red = (unsigned short)((pixel & 0xF800) >> 11);  // 5
      green = (unsigned short)((pixel & 0x07E0) >> 5); // 6
      blue = (unsigned short)(pixel & 0x001F);         // 5

      // Increase intensity and make rgb888
      red = red << 3;
      green = green << 2;
      blue = blue << 3;

      rgb_pixel_t bpixel = {blue, green, red, 0};
      bmp_set_pixel(bmp, i, j, bpixel);
      //rgb_pixel_t* bpixel;
      //bpixel = (void*)&pixel; 
      //bmp_set_pixel(bmp, i, j, *bpixel);
    }
  }

  bmp_save(bmp, outfile);
  bmp_destroy(bmp);

  return 0;
}
