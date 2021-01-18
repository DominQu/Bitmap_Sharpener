# Bitmap_Sharpener
This is a student project. The task was to do a convolution on a bitmap in order to sharpen sonar image.

### How the program works:

The program loads three lines of the .bmp file to a buffer. The convolution is done on that buffer. The resulting processed data is written to a new file. After that the buffer is updated and the convolution is done again.

### What I learned:

+ how .bmp file is structured and what information its headers hold
+ what is a convolution and how to do it on an image
+ how to operate with input and output streams in C++
