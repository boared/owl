/**
 * This class is used to save/open image files.
 *  
 * Licensed under the MIT License (MIT)
 * Copyright (c) 2015 Eder de Almeida Perez
 * 
 * @author: Eder Perez.
 */

#include <fstream>
#include <jpeglib.h>
#include "ImageFile.h"
#include "Image.h"

namespace owl
{
    bool ImageFile::load( const std::string& path, Image<BYTE>& image )
    {
        Format format = checkFileExtension( path );

        switch ( format )
        {
            case Format::JPEG:
                return loadJPEG( path, image );
                
            default:
                return false;
        }
    }
    
    bool ImageFile::save( const std::string& path, const Image<BYTE>& image )
    {
        Format format = checkFileExtension( path );
        
        switch ( format )
        {
            case Format::JPEG:
                return saveJPEG( path, image, 100 );
                
            default:
                return false;
        }
    }
    
    ImageFile::Format ImageFile::checkFileExtension( const std::string& path )
    {
        if ( path.empty() )
        {
            return Format::UNKNOWN;
        }
        
        return Format::JPEG;
    }
    
    unsigned int ImageFile::calculateRowSize(unsigned int width, int bpp)
    {
        return ( (width * bpp + 31) & ~31 ) >> 3;
    }
    
    int ImageFile::calculateBpp( ColorSpace::Type colorSpace, unsigned int channelSize )
    {
        return ColorSpace::calculateNumberOfChannels( colorSpace ) * channelSize * 8;
    }

    bool ImageFile::loadJPEG( const std::string& path, ImageByte& image )
    {
        // Based on source code from http://www.aaronmr.com/en/2010/03/test/
        
        FILE* file = fopen(path.c_str(), "rb");

        if ( file == nullptr )
        {
            return false;
        }
        
        // These are standard libjpeg structures for reading(decompression)
        struct jpeg_decompress_struct cInfo;
        struct jpeg_error_mgr jError;

        // Here we set up the standard libjpeg error handler
        cInfo.err = jpeg_std_error( &jError );
        
        // Setup decompression process and source, then read JPEG header
        jpeg_create_decompress( &cInfo );
        
        // This makes the library read from file
        jpeg_stdio_src( &cInfo, file );

        // Reading the image header which contains image information
        jpeg_read_header( &cInfo, TRUE );

        // Start decompression jpeg here
        jpeg_start_decompress( &cInfo );
        
        // Set color space
        ColorSpace::Type colorSpace;
        switch (cInfo.out_color_space)
        {
            case JCS_GRAYSCALE:
                colorSpace = ColorSpace::Type::GRAYSCALE;
                break;
                
            case JCS_RGB:
            case JCS_EXT_RGB:
                colorSpace = ColorSpace::Type::RGB;
                break;
                
            case JCS_EXT_RGBA: // red/green/blue/alpha
                colorSpace = ColorSpace::Type::RGBA;
                break;
                
            default:
                //JCS_YCbCr: // Y/Cb/Cr (also known as YUV)
                //JCS_CMYK: // C/M/Y/K
                //JCS_YCCK: // Y/Cb/Cr/K
                //JCS_EXT_BGR: // blue/green/red
                //JCS_EXT_BGRA: // blue/green/red/alpha
                //JCS_EXT_ABGR: // alpha/blue/green/red
                //JCS_EXT_ARGB: // alpha/red/green/blue
                
                // Wrap up decompression, destroy objects and close open files
                jpeg_finish_decompress(&cInfo);
                jpeg_destroy_decompress(&cInfo);
                fclose(file);
                return false;
        }
        
        image.create( cInfo.output_width, cInfo.output_height, colorSpace );

        // Read one scan line at a time
        unsigned int row = 0;
        while ( cInfo.output_scanline < cInfo.image_height )
        {
            // libjpeg data structure for storing one row, that is, scanline of an image
            JSAMPROW rowPointer[1];
            rowPointer[0] = image(row++, 0);
            jpeg_read_scanlines( &cInfo, rowPointer, 1 );
        }

        // Wrap up decompression, destroy objects, free pointers and close open files
        jpeg_finish_decompress( &cInfo );
        jpeg_destroy_decompress( &cInfo );
        fclose( file );

        return true;
    }
    
    bool ImageFile::saveJPEG( const std::string& path, const Image<BYTE>& image, int quality )
    {
        // Based on source code from http://www.aaronmr.com/en/2010/03/test/
        
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;

        FILE* outfile = fopen( path.c_str(), "wb" );

        if ( outfile == nullptr )
        {
            return false;
        }
        
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
        jpeg_stdio_dest(&cinfo, outfile);

        // Setting the parameters of the output file here
        cinfo.image_width = image.getWidth();
        cinfo.image_height = image.getHeight();
        cinfo.input_components = image.getNumberOfChannels();
        
        switch ( image.getColorSpace() )
        {
            case owl::ColorSpace::Type::GRAYSCALE:
                cinfo.in_color_space = JCS_GRAYSCALE;
                break;
                
            case owl::ColorSpace::Type::RGB:
                cinfo.in_color_space = JCS_RGB;
                break;
                
            case owl::ColorSpace::Type::RGBA:
                cinfo.in_color_space = JCS_EXT_RGBA;
                break;
                
            default:
                return false;
        }
        
        // Default compression parameters, we shouldn't be worried about these
        jpeg_set_defaults(&cinfo);
        cinfo.num_components = image.getNumberOfChannels();
        //cinfo.data_precision = 4;
        cinfo.dct_method = JDCT_FLOAT;
        jpeg_set_quality(&cinfo, quality, TRUE);
        
        // Now do the compression ..
        jpeg_start_compress(&cinfo, TRUE);
        
        // Like reading a file, this time write one row at a time
        int row = 0;
        while ( cinfo.next_scanline < cinfo.image_height )
        {
            // This is a pointer to one row of image data
            JSAMPROW row_pointer[1];
            row_pointer[0] = const_cast<unsigned char*>(image(row++, 0));
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }
        
        // Similar to read file, clean up after we're done compressing
        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);
        fclose(outfile);

        return true;
    }
}
