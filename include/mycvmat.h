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

#ifndef _MYCVMAT_H_
#define _MYCVMAT_H_

#include "opencv2/opencv.hpp"

namespace ZedGrabber
{
  class MyCvMat
  {
    public:
      MyCvMat(const cv::Mat & _in) { myMat = _in.clone(); }
      inline const cv::Mat getMat() const
      {
	return myMat;
      }
      MyCvMat& operator=(const MyCvMat& _in)
      {
	myMat = _in.myMat.clone();
      }
      MyCvMat& operator=(const cv::Mat& _in)
      {
 	myMat = _in.clone();
      }
      
    private:
      cv::Mat myMat;
  };
}

#endif
