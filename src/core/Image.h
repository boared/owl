/**
 * This template class is a data structure to hold and access images. The
 * Channel type represents the data type of a pixel.
 * 
 * The origin of the coordinate system is top-left.
 * 
 *  (0,0)* ---------------------> j
 *       | XXXXXXXXXXXXXXXXXX p
 *       | XXXXXXXXXXXXXXXXXX p
 *       | XXXXXXXXXXXXXXXXXX p
 *       | XXXXXXXXXXXXXXXXXX p
 *       | XXXXXXXXXXXXXXXXXX p
 *       |
 *       v
 *       i
 * 
 * X = pixel value
 * p = padding (0 or more bytes for 32-bits alignment)
 *
 * 
 * 
 * Licensed under the MIT License (MIT)
 * Copyright (c) 2015 Eder de Almeida Perez
 * 
 * @author: Eder A. Perez.
 */


#ifndef IMAGE_H
#define IMAGE_H

#include <cstring>
#include <type_traits>
#include "Types.h"


namespace owl
{
    template<typename Channel> class Image;

    /**
     * Pre-defined image objects.
     */
    typedef Image<BYTE> ImageByte;
    typedef Image<float> ImageFloat;
    typedef Image<double> ImageDouble;

    template<typename Channel>
    class Image
    {
        public:

            /**
             * Check for wrong declaration in the type of a owl::Image channel.
             * A owl::Image channel type must be one of BYTE, float or double
             * types.
             */
            static_assert( std::is_same< BYTE, Channel >::value ||
                           std::is_same< float, Channel >::value ||
                           std::is_same< double, Channel >::value,
                           "owl::Image assertion: Invalid type for color channel." );

            /**
             * Instantiates an empty image.
             */
            Image();

            /**
             * Instantiates an image.
             * @param width Image width.
             * @param height Image height.
             * @param colorSpace (Optional) Image color space. Default is GRAY.
             * @param data Array with image pixels. Must be the same type and
             * padding as the image being created.
             */
            Image( unsigned int width, unsigned int height, ColorSpace::Type colorSpace = ColorSpace::Type::GRAYSCALE, const Channel* data = nullptr );
            
            /**
             * Copy constructor.
             */
            Image( const Image& image );
            
            /**
             * Image destructor.
             */
            ~Image();
            
            /**
             * Releases all resources and sets everything to zero.
             */
            void destroy();
            
            /**
             * Creates an new image. All previous data will be destroyed.
             * @param width Image width.
             * @param height Image height.
             * @param colorSpace Image color space.
             * @param data (Optional) Array with image pixels. Must be the same type and
             * padding as the image being created.
             */
            void create( unsigned int width, unsigned int height, ColorSpace::Type colorSpace, const Channel* data = nullptr );
    
            /**
             * Gets the image width.
             * @return Image width.
             */
            unsigned int getWidth() const;
            
            /**
             * Gets the image height.
             * @return Image height.
             */
            unsigned int getHeight() const;
            
            /**
             * Gets the length of a scanline in bytes (including padding, if it
             * exists).
             * @return Length of a scanline.
             */
            unsigned int getRowSize() const;
            
            /**
             * Gets the image color space.
             * @return Image color space.
             */
            ColorSpace::Type getColorSpace() const;
            
            /**
             * Gets the number of channels per pixel.
             * @return Image number of channels per pixel.
             */
            int getNumberOfChannels() const;
            
            /**
             * Gets the image's pixels array.
             * @return A array with pixels. The lines of the image might be
             * padded for memory alignment.
             */
            Channel* getData();
            const Channel* getData() const;
            
            /**
             * Pixel access.
             * @param row Image row coordinate. Must be in [0, image width).
             * @param column Image column coordinate. Must be in [0, image height).
             * @return A reference to the pixel in (row, column).
             */
            Channel* operator()( unsigned int row, unsigned int column );
            const Channel* operator()( unsigned int row, unsigned int column ) const;
            
            /**
             * Attribution operator.
             * @param image Input image.
             * @return The input image is copied to the output image.
             */
            Image* operator=(const Image& image);


        private:
            
            /**
             * Calculate the bits per pixel given a color space and the data
             * type of an image's pixel.
             * @param colorSpace A color space.
             */
            static int calculateBpp( ColorSpace::Type colorSpace );
            
            /**
             * Calculate the size of a scanline in bytes including padding.
             * @param width Image width.
             * @param bpp Bits per pixel.
             * @return The size of a scanline in bytes including padding.
             */
            static unsigned int calculateRowSize(unsigned int width, int bpp);

            /**
             * Color space.
             */
            ColorSpace::Type mColorSpace;

            /**
             * Image's bits per pixel.
             */
            int mBpp;

            /**
             * Image width and height in pixels.
             */
            unsigned int mWidth;
            unsigned int mHeight;

            /**
             * Length of a scanline in bytes (including padding, if it exists)
             */
            unsigned int mRowSize;
            
            /**
             * Color's number of channels to represent a pixel.
             */
            int mNumberOfChannels;

            /**
             * The image data
             */
            Channel* mData;
    };
    
    
    template<typename Channel>
    Image<Channel>::Image() :
        mColorSpace( ColorSpace::Type::RGB ),
        mBpp( 0 ),
        mWidth( 0 ),
        mHeight( 0 ),
        mRowSize( 0 ),
        mNumberOfChannels( 0 ),
        mData( nullptr )
    {
    }
    
    template<typename Channel>
    Image<Channel>::Image( unsigned int width, unsigned int height, ColorSpace::Type colorSpace, const Channel* data ) :
        mColorSpace( colorSpace ),
        mBpp( calculateBpp( colorSpace ) ),
        mWidth( width ),
        mHeight( height ),
        mRowSize( calculateRowSize( mWidth, mBpp ) ),
        mNumberOfChannels( ColorSpace::calculateNumberOfChannels( colorSpace ) ),
        mData( new Channel[mRowSize * mHeight] )
    {
        if ( data != nullptr )
        {
            std::memcpy(mData, data, sizeof(BYTE) * mRowSize * mHeight);
        }
    }

    template<typename Channel>
    Image<Channel>::Image( const Image& image ) :
        Image(image.getWidth(), image.getHeight(), image.getColorSpace(), image.getData())
    {
    }

    template<typename Channel>
    Image<Channel>::~Image()
    {
        delete[] mData;
    }
    
    template<typename Channel>
    void Image<Channel>::destroy()
    {
        mColorSpace = ColorSpace::Type::RGB;
        mBpp = 0;

        mWidth = 0;
        mHeight = 0;
        mRowSize = 0;
        mNumberOfChannels = 0;

        delete[] mData;
        mData = nullptr;
    }
    
    template<typename Channel>
    void Image<Channel>::create( unsigned int width, unsigned int height, ColorSpace::Type colorSpace, const Channel* data )
    {
        destroy();
        
        mColorSpace = colorSpace;
        mBpp = calculateBpp( colorSpace );

        mWidth = width;
        mHeight = height;
        mRowSize = calculateRowSize( mWidth, mBpp );
        mNumberOfChannels = ColorSpace::calculateNumberOfChannels( colorSpace );

        mData = new BYTE[mRowSize * mHeight];
        
        if ( data != nullptr )
        {
            memcpy( mData, data, sizeof(BYTE) * mRowSize * mHeight );
        }
    }
    
    template<typename Channel>
    unsigned int Image<Channel>::getWidth() const
    {
        return mWidth;
    }

    template<typename Channel>
    unsigned int Image<Channel>::getHeight() const
    {
        return mHeight;
    }
    
    template<typename Channel>
    unsigned int Image<Channel>::getRowSize() const
    {
        return mRowSize;
    }
    
    template<typename Channel>
    ColorSpace::Type Image<Channel>::getColorSpace() const
    {
        return mColorSpace;
    }

    template<typename Channel>
    int Image<Channel>::getNumberOfChannels() const
    {
        return mNumberOfChannels;
    }

    template<typename Channel>
    Channel* Image<Channel>::getData()
    {
        return mData;
    }
    
    template<typename Channel>
    const Channel* Image<Channel>::getData() const
    {
        return mData;
    }
    
    template<typename Channel>
    Channel* Image<Channel>::operator()( unsigned int row, unsigned int column )
    {
        return mData + (row * mRowSize + column * mNumberOfChannels);
    }
    template<typename Channel>
    const Channel* Image<Channel>::operator()( unsigned int row, unsigned int column ) const
    {
        return mData + (row * mRowSize + column * mNumberOfChannels);
    }
    
    template<typename Channel>
    Image<Channel>* Image<Channel>::operator=(const Image<Channel>& image)
    {
        mWidth = image.mWidth;
        mHeight = image.mHeight;
        mRowSize = image.mRowSize;
        mNumberOfChannels = image.mNumberOfChannels;
        mColorSpace = image.mColorSpace;
        mBpp = image.mBpp;

        unsigned int sourceSize = image.mRowSize * image.mHeight;
        unsigned int destSize = mRowSize * mHeight;

        if ( sourceSize != destSize )
        {
            delete[] mData;
            mData = new Channel[ sourceSize ];
        }

        memcpy( mData, image.mData, sourceSize );

        return this;
    }
    
    template<typename Channel>
    int Image<Channel>::calculateBpp( ColorSpace::Type colorSpace )
    {
        return ColorSpace::calculateNumberOfChannels( colorSpace ) * sizeof(Channel) * 8;
    }
    
    template<typename Channel>
    unsigned int Image<Channel>::calculateRowSize(unsigned int width, int bpp)
    {
        return ( (width * bpp + 31) & ~31 ) >> 3;
    }
}

#endif // IMAGE_H
