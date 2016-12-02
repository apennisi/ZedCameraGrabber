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

#ifndef _MOSAIC_H_
#define _MOSAIC_H_

#include <iostream>
#include "opencv2/opencv.hpp"

namespace ZedGrabber
{
  class Mosaic 
  {
    private:
      static Mosaic *m_instance;
      Mosaic() {;}
    public:
      static Mosaic *instance() 
      {
	if(!m_instance)
	  m_instance = new Mosaic;
	return m_instance;
      }
      
      cv::Mat mosaic(const cv::Mat& img1, const cv::Mat& img2);
  };
}

#endif
