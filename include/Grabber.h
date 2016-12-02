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

#ifndef _GRABBER_H
#define _GRABBER_H

#include <opencv2/opencv.hpp>
#include <ctime>
#include <fstream>

namespace ZedGrabber
{
    class Grabber
    {
        public:
            Grabber(){;}
            virtual ~Grabber(){;}
            virtual bool getData() = 0;
            inline const cv::Mat frame() { return  m_frame; } 
            inline const cv::Mat depth() { return m_depth; }
            inline const cv::Mat depth16bit() { return m_depth16bit; }
            virtual bool finish() = 0;
        protected:
            cv::Mat m_frame;
            cv::Mat m_depth;
            std::string sequenceName;
            int frameCounter;
            int width, height;
            cv::Mat m_depth16bit;
            float m_fx;
            float m_fy;
            float m_cx;
            float m_cy;
            float inv_fx;
            float inv_fy;
            float m_max_height;
	    bool m_init;

    };
}

#endif // KINECT_H
