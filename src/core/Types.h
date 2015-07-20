/** 
 * This file contains types definitions and related functions.
 * 
 * Licensed under the MIT License (MIT)
 * Copyright (c) 2015 Eder Perez
 * 
 * @author: Eder Perez.
 */

#ifndef TYPES_H
#define TYPES_H

#include <cstdint>


namespace owl
{
    /**
     *  8-bits data type for color channel representation.
     */
    typedef uint8_t BYTE;
    
    namespace ColorSpace
    {
        /**
         * Definition of color space types.
         */
        enum class Type
        {
            UNKNOWN,
            GRAYSCALE,
            RGB,
            RGBA
        };
        
        /**
         * Calculate the color's number of channels given a color space.
         * @param colorSpace Color space.
         * @return The color's number of channels.
         */
        static int calculateNumberOfChannels(Type colorSpace)
        {
            switch (colorSpace)
            {
                case Type::RGB:
                    return 3;

                case Type::RGBA:
                    return 4;

                case Type::GRAYSCALE:
                    return 1;

                default:
                    return 0;
            }
        }
    }
}

#endif // TYPES_H
