RGB565 Tools
====

Simple examples of how to convert raw rgb565 frames into other formats.

  * RAW (rgb888)
  * BMP (32-bit)
  * PPM (P3 Plain)

Usage
----

raw rgb565 to ppm:

    # rgb565toppm <infile> <width> <height> <maxval> fb.ppm
    # rgb565 has maxval of 255 per pixel because it is converted to rgb888
    rgb565toppm fb.rgb565.bin 720 480 255 fb.ppm

raw rgb565 to bmp:

    # rgb565toppm <infile> <width> <height> <bitdepth> fb.ppm
    # Bug: only reliably works converting from a depth of 16 to 32
    rgb565tobmp fb.rgb565.bin 720 480 32 fb.bmp
    

Dependecies
====

  * [libbmp](http://code.google.com/p/libbmp/)


Bugs
====

bmptorgb565
----
`bmptorgb565` only supports bitmaps with headers that meet the formal spec... so basically it doesn't work

Image size is (720*480*3) or 1036800 bytes
Header is 54 bytes

Header of reference.24.bmp:

    0000000 42 4d 36 d2 0f 00 00 00 00 00 36 00 00 00 28 00
    0000010 00 00 d0 02 00 00 20 fe ff ff 01 00 18 00 00 00
    0000020 00 00 00 00 00 00 13 0b 00 00 13 0b 00 00 00 00
    0000030 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

Note that the width is not written correctly by either of OSX Preview or OSX Paintbrush

`fe ff ff ff` is completely wrong for the height. `00 00 00 00` is obviously not the correct size.

Header read:

    type: 4d42
    total size: 15
    offset: 2621440
    width: 4263510016
    width: 131071
    bitcount(color depth): 0
    size image (image size): 185794560
    image size in bytes: 181440

     Red16: 0 
     Green16: 0 
     Blue16: 0 
     RGB2Bytes: 0 
    Segmentation fault <-- Obvious sign that it doesn't work


References
====

[RGB565 To PNG/JPEG](http://www.swview.org/node/165):

    # Single RAW Image
    ffmpeg -vcodec rawvideo -f rawvideo -pix_fmt rgb565 -s 1024x768 -i image.raw -f image2 -vcodec png image.png

    # Multiple Images / RAW Video
    ffmpeg -vcodec rawvideo -f rawvideo -pix_fmt rgb565 -s 1024x768 -i movie.raw -f image2 -vcodec png image%d.png

    # Stream / RAW Video
    command-that-streams-to-stdout | ffmpeg -vcodec rawvideo -f rawvideo -pix_fmt rgb565 -s 1024x768 -i - -f image2 -vcodec png image%d.png
