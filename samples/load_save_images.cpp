/** 
 * This sample shows how to open and save an image file. The image file format
 * for both open and save an image is given by the file extension.
 * 
 * Licensed under the MIT License (MIT)
 * Copyright (c) 2015 Eder Perez
 * 
 * @author: Eder Perez.
 */
#include <iostream>
#include "Image.h"
#include "ImageFile.h"


int main(int argc, char** argv)
{
    if ( argc != 2 )
    {
        exit(1);
    }

    std::string imageFilePath = argv[1];

    owl::ImageByte image;
    
    if ( !owl::ImageFile::load( imageFilePath, image ) )
    {
        std::cout << "Fail to open file.\n";
        exit(1);
    }
    
    owl::ImageFile::save("lena_copy.jpg", image);
    
    return 0;
}

