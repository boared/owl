/**
 * This class contains methods for several operations on images.
 * 
 * Licensed under the MIT License (MIT)
 * Copyright (c) 2015 Eder de Almeida Perez
 * 
 * @author: Eder A. Perez.
 */


#ifndef IMAGE_OPERATOR_H
#define IMAGE_OPERATOR_H

#include "Image.h"


namespace owl
{
    class ImageOperator
    {
        public:

            /**
             * Compute the addition of two images. The output image and the
             * input image can be the same and all images must have the same
             * dimension.
             * 
             * If image channel type is BYTE, an overflow will be truncated to
             * 255 and an underflow to 0.
             * @param outputImage The resulting of imageA + imageB.
             * @param imageA An input image.
             * @param imageB An input image.
             */
            static void template<typename Channel> add( Image<Channel>& outputImage, const Image<Channel>& imageA, const Image<Channel>& imageB );
            
            /**
             * Compute the difference of two images. The output image and the
             * input image can be the same and all images must have the same
             * dimension.
             * 
             * If image channel type is BYTE, an overflow will be truncated to
             * 255 and an underflow to 0.
             * @param outputImage The resulting of imageA - imageB.
             * @param imageA An input image.
             * @param imageB An input image.
             */
            static void template<typename Channel> subtract( Image<Channel>& outputImage, const Image<Channel>& imageA, const Image<Channel>& imageB );
            
            /**
             * Multiply an image by a scalar value. The output image and the
             * input image can be the same and both images must have the same
             * dimension.
             * 
             * If image channel type is BYTE, an overflow will be truncated to
             * 255 and an underflow to 0.
             * @param outputImage The resulting of inputImage * scalar.
             * @param inputImage An input image.
             * @param scalar A scalar value.
             */
            static void template<typename Channel, typename S> multiply( Image<Channel>& outputImage, const Image<Channel>& inputImage, const S scalar );
            
            /**
             * Multiply two images pixel by pixel. The output image can not be
             * either @imageA or @imageB and all images must have the same
             * dimension.
             * 
             * If image channel type is BYTE, an overflow will be truncated to
             * 255 and an underflow to 0.
             * @param outputImage The resulting of imageA * imageB.
             * @param imageA An input image.
             * @param imageB An input image.
             */
            static void template<typename Channel> multiply( Image<Channel>& outputImage, const Image<Channel>& imageA, const Image<Channel>& imageB );
            
            /**
             * Compute a grayscale image from an RGB image using the following
             * luminance operator:
             *
             * g(x, y) = 0.2126*R(x, y) + 0.7152*G(x, y) + 0.0722*B(x, y)
             * 
             * Both images must have the same dimension. The input image must
             * be an RGB image. Its not mandatory the output image color space
             * be grayscale. However, if it is not only the first channel will
             * hold the resulting computation.
             * @param outputImage The grayscale resulting image.
             * @param inputImage A RGB image.
             */
            static void template<typename Channel> luminance( Image<Channel>& outputImage, const Image<Channel>& inputImage );
            
        private:
            
            /**
             * Check if two images have the same color space and dimensions.
             * @param imageA Input image.
             * @param imageB Input image.
             * @return True if they have equal color space.
             */
            static bool template<typename Channel> areCompatible( const Image<Channel>& imageA, const Image<Channel>& imageB );
    };
    
    
    template<typename Channel>
    void ImageOperator<Channel>::add( Image<Channel>& outputImage, const Image<Channel>& imageA, const Image<Channel>& imageB )
    {
        if ( !areCompatible( imageA, imageB ) )
        {
            return;
        }
        else if ( !areCompatible( outputImage, imageA ) )
        {
            outputImage.create( imageA.getWidth(), imageA.getHeight(), imageA.getColorSpace() );
        }
        
        // @TODO: Implement addition.
    }
    

    template<typename Channel>
    bool ImageOperator<Channel>::areCompatible( const Image<Channel>& imageA, const Image<Channel>& imageB )
    {
        return imageA.getWidth() == imageB.getWidth() &&
               imageA.getHeight() == imageB.getHeight() &&
               imageA.getColorSpace() == imageB.getColorSpace();
    }
}

#endif // IMAGE_OPERATOR_H
