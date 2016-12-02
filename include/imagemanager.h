/*
 *  Zed Camera Grabber (ZCG)
 *  Copyright 2016 Andrea Pennisi
 *
 *  This file is part of ZCG and it is distributed under the terms of the
 *  GNU Lesser General Public License (Lesser GPL)
 *
 *
 *
 *  ZCG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AT is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with ZCG.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  ZCG has been written by Andrea Pennisi
 *
 *  Please, report suggestions/comments/bugs to
 *  andrea.pennisi@gmail.com
 *
 */

/**
 * \file imagemanager.h
 *
 * \class ImageManager
 *
 * \brief Class for managing a set of images
 *
 **/

#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <dirent.h>
#include <stdio.h>

class ImageManager
{
    public:
	ImageManager() { ; }
        /**
        * \brief Create a new object ImageManager
        *
        * \param d: directory path
        *
        */
        ImageManager(const std::string &d);
        /**
         * \brief ImageManager Destructor
         *
         */
        virtual ~ImageManager();
        /**
         * \brief Return the number of analysed images
         *
         */
        inline int getCount() const
        {
            return count;
        }
        /**
         * \brief Return the end of the image set
         *
         */
        inline int getEnd() const
        {
            return end;
        }
        /**
         * \brief Return the previous image
         *
         * \return return the previous image
         *
         */
        std::string next(const int &speed);
        /**
         * \brief Return current frame number
         *
         * \return return current frame number
         *
         */
        std::string prev(const int &speed);
    private:
        /**
         * \brief Sort the filenames according to the natural sort algorithm
         *
         * \param data: the vector containing the names of the files
         *
         */
        void sorting(std::vector<std::string>& data);
        int count, end;
        std::vector<std::string> filename;
        std::string dir_name;
};

#endif // IMAGEMANAGER_H
