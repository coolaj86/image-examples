// Taken from http://www.codeguru.com/forum/showthread.php?t=374650
// Attempted to port from windows example given, Currently broken

#include <stdio.h>
#include <stdlib.h>

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

//#pragma pack(2) /*2 byte packing */

typedef struct
{
  uint16_t type; // uint8_t magic[2]
  uint32_t size;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t offset;
} Header;

//#pragma pop()

//#pragma pack() /* Default packing */

typedef struct
{
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitcount;
    uint32_t compression;
    uint32_t sizeimage;
    uint32_t xpelspermeter;
    uint32_t ypelspermeter;
    uint32_t colorsused;
    uint32_t colorsimportant;
} Infoheader;

//#pragma pop()

typedef struct {
    char red;
    char green;
    char blue;
} Single_pixel;


FILE *fp_rgb; //global file pointer to rgb image

void BMP24ToRGB565 (char *bmp_24)
{
  
    char rgb_565[2];
    
    char Red24 = bmp_24[0];   // 8-bit red
    char Green24 = bmp_24[1]; // 8-bit green
    char Blue24 = bmp_24[2];  // 8-bit blue
    
    char Red16   = Red24   >> 3;  // 5-bit red
    char Green16 = Green24 >> 2;  // 6-bit green
    char Blue16  = Blue24  >> 3;  // 5-bit blue
    
    printf("\n Red16: %x \n", Red16);
    printf(" Green16: %x \n", Green16);
    printf(" Blue16: %x \n", Blue16);
    
    unsigned short RGB2Bytes = Blue16 + (Green16<<5) + (Red16<<(5+6));
    printf(" RGB2Bytes: %d \n", RGB2Bytes);

    rgb_565[0] = ((char*)RGB2Bytes)[1]; // TODO LOBYTE(RGB2Bytes);
    rgb_565[1] = ((char*)RGB2Bytes)[0]; //TODO HIBYTE(RGB2Bytes);
    //printf(" rgb_565[0]: %x \n", rgb_565[0]);
    //printf(" rgb_565[1]: %x \n\n", rgb_565[1]);
    
    //fwrite(rgb_565,1, sizeof(rgb_565), fp_rgb);
    
    char low = rgb_565[0];
    char high = rgb_565[1];
    
    fwrite(&low, 1, sizeof(low), fp_rgb);
    fwrite(&high, 1, sizeof(high), fp_rgb); 
           
}



int main()
{
    Header headfirst;
    Infoheader headsecond;
    Single_pixel single_pixel;
    char* bmp = "test/reference.24.bmp";
    char* rgb = "test/output.rgb565.bin";
    
    int byte_border;
    
    /* binary opening of the input image file (24bit bmp) */
    FILE *infile;
    infile = fopen(bmp,"rb+");
    if(infile==NULL) //file doesnt exist
    {
      printf("Error opening first file");
      exit(0);
      return 0;
    }
    
    /* binary opening of the output image file (rgb565) */
    fp_rgb = fopen(rgb, "wb");
    
    //from first header
    fread(&headfirst,sizeof(headfirst),1,infile);

    printf("type: %x\n",headfirst.type);
    printf("total size: %u\n",headfirst.size);
    printf("offset: %u\n",headfirst.offset);

    //from second header
    fread(&headsecond,sizeof(headsecond),1,infile);

    printf("width: %lu\n",headsecond.width);
    printf("width: %lu\n",headsecond.height);
    printf("bitcount(color depth): %u\n",headsecond.bitcount);
    printf("size image (image size): %lu\n",headsecond.sizeimage);

    //size calculations
    byte_border = headsecond.sizeimage / 1024; //calculate how many bytes the image size is.
    printf("image size in bytes: %d\n", byte_border); 
    
    //BMP pixel array
    char bmp_24[3];
    
    int i;
    for(i = 0; i < byte_border / 3; i++)  //should loop for byte_border/3, consequently reading all BMP pixels in the image
    {
      fread(&single_pixel,sizeof(single_pixel),1,infile);
      
      //printf("red: %x\n",single_pixel.red);
      //printf("green: %x\n",single_pixel.green);
      //printf("blue: %x\n",single_pixel.blue);
      
      bmp_24[0] = single_pixel.red;
      bmp_24[1] = single_pixel.green;
      bmp_24[2] = single_pixel.blue;
      
      BMP24ToRGB565(bmp_24);
    }
    
    fclose(infile);
    fclose(fp_rgb);
    
    /* TEST PART ---> checking output rgb565 file */
    FILE *fp_test;
    fp_test = fopen(rgb,"rb+");
    if(fp_test==NULL) //file doesnt exist
    {
      printf("Error opening first file");
      exit(EXIT_FAILURE);
    }
    
    unsigned short RGB2Bytes;
    char rgb565_test[2];
    char low;
    char high;
      
    for(i = 0; i< 20 /*(byte_border / 3) * 2 */; i++)  //should loop for byte_border/3 *2 , consequently reading all RGB pixels in the image
    {
    
    //fread(&RGB2Bytes,sizeof(RGB2Bytes),1,fp_test);
    //rgb565_test[0] = LOBYTE(RGB2Bytes);
    //rgb565_test[1] = HIBYTE(RGB2Bytes);
    //printf("\n RGB TEST - RGB2Bytes: %d\n", RGB2Bytes);
    //printf("RGB TEST - rgb565_test[0]: %x\n",rgb565_test[0]);
    //printf("RGB TEST - rgb565_test[1]: %x\n\n",rgb565_test[1]);
    
    fread(&low,sizeof(low),1,fp_test);
    fread(&high,sizeof(high),1,fp_test);

    printf("\nRGB TEST - low: %x\n", low);
    printf("RGB TEST - high: %x\n", high);
    
    
    }
    
    system("pause");

    return 0;

}
