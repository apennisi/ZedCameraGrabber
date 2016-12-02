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


#include "mosaic.h"

using namespace ZedGrabber;

Mosaic *Mosaic::m_instance = 0;

cv::Mat Mosaic::mosaic(const cv::Mat& img1, const cv::Mat& img2)
{
  const uint& width = img1.cols + img2.cols;
  const uint& height = img1.rows;
  
  cv::Mat mosImg = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
 
  
  cv::Mat colorDepth, colorRGB;

  if(img1.channels() == 3)
  {
    colorRGB = img1;
  }
  else
  {
    cv::cvtColor(img1, colorRGB, CV_BGRA2BGR);
  }
  
  if(img2.channels() == 4)
  {
    cv::cvtColor(img2, colorDepth, CV_BGRA2BGR);
  }
  else
  {
    double min;
    double max;
    cv::minMaxIdx(img2, &min, &max);
    cv::Mat adjMap;
    img2.convertTo(adjMap, CV_8UC1, 255 / (max-min), -min); 

    cv::applyColorMap(adjMap, colorDepth, cv::COLORMAP_JET);
  }
  
  colorRGB.copyTo(mosImg(cv::Rect(0, 0, img1.cols, img1.rows)));

  colorDepth.copyTo(mosImg(cv::Rect(img1.cols, 0, colorDepth.cols, colorDepth.rows)));
  
  return mosImg;
}
