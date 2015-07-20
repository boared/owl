/**
 * This class is used to save/open image files.
 *  
 * Licensed under the MIT License (MIT)
 * Copyright (c) 2015 Eder de Almeida Perez
 * 
 * @author: Eder Perez.
 */

#ifndef IMAGE_FILE_H
#define	IMAGE_FILE_H

#include <string>
#include "Types.h"

namespace owl
{
    template<typename Channel> class Image;
    typedef Image<BYTE> ImageByte;

    class ImageFile
    {
        public:
            
            enum class Format
            {
                UNKNOWN,
                JPEG
            };

            /**
             * Load an image file into an Image object.
             * 
             * For this method works properly, it is necessary that the file
             * extension is correct.
             * @param path File path.
             * @param image An Image object to be populated with the loaded image.
             */
            static bool load( const std::string& path, ImageByte& image );
            
            /**
             * Save an Image into an image file.
             * 
             * For this method works properly, it is necessary that the file
             * extension is correct.
             * @param path File path.
             * @param image An Image object to be populated with the loaded image.
             */
            static bool save( const std::string& path, const ImageByte& image );

        private:
            
            /**
             * Given a file path, return the file image format. The format is
             * given by the file extension.
             * @param path File path.
             * @return The file image format.
             */
            static Format checkFileExtension( const std::string& path );
            
            /**
             * Calculate the size of a scanline in bytes including padding.
             * @param width Image width.
             * @param bpp Bits per pixel.
             * @return The size of a scanline in bytes including padding.
             */
            static unsigned int calculateRowSize(unsigned int width, int bpp);
            
            /**
             * Calculate the bits per pixel given a color space and the size in
             * bytes of the color channel.
             * @param colorSpace A color space.
             * @param channelSize Color channel size in bytes.
             */
            static int calculateBpp( ColorSpace::Type colorSpace, unsigned int channelSize );

            /**
             * Load a jpeg file using libjpeg
             * @param path File path.
             * @param image An Image object to be populated with the loaded image.
             */
            static bool loadJPEG( const std::string& path, ImageByte& image );
            
            /**
             * Save a jpeg file using libjpeg
             * @param path File path.
             * @param image An Image object to be saved into path.
             */
            static bool saveJPEG( const std::string& path, const ImageByte& image, int quality );
    };
}
#endif	// IMAGE_FILE_H
