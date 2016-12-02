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

#ifndef _IMAGEREADER_H_
#define _IMAGEREADER_H_

#include "Grabber.h"
#include "imagemanager.h"
#include <iostream>
#include <future>
#include <fstream>
#include <opencv2/opencv.hpp>

namespace ZedGrabber
{
  class ImageReader : public Grabber
  {
    public:
      ImageReader(const std::string& name);
      ImageReader() { ; }
      virtual bool getData();
      virtual bool finish();
    private:
      std::string frameName;
      std::string depthName;
      std::shared_ptr<ImageManager> frameReader;
      std::shared_ptr<ImageManager> depthReader;
      uint end;
      uint i;
    private:
      cv::Mat convertImage(const std::string& name);
  };
}

#endif
