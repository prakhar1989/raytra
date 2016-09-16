Assignment 0
===


You will write a C++ command-line program called "hw0" that takes one argument, the name of an image file in OpenEXR format.

This program will open the OpenEXR file, modify the image,  and then save it out in the file named "hw0.exr".

The modifications are as follows: imagine the image divided into four regions - upper left, upper right, lower left, and lower right. For the upper left, only retain the red channel (the blue and green channels should be set to zero). For the upper right, retain on the green channel. For the lower left, retain only the blue channel. And the lower right should be the LUMINANCE: each of the red, blue and green channels should get the value  L = 0.2126*red + 0.7152*green + 0.0722*blue.

The regions are not divided evenly: the upper left region goes from the left edge to 1/3 the way across, and from the top edge 1/2 the way down. The upper right region starts from 1/3rd of the way across, and continues all the way to the right side, and (like the upper left region) covers half the image starting at the top edge. The lower left region goes from the left edge to 1/3 of the way across, and from 1/2 down the image all the way to the bottom edge. The lower right region cover the remaining area. (note: when 1/3 or 1/2 does not evenly divide the image, use the floor operation to decide the end of the top left regions' boundaries - the other regions should properly meet it).
