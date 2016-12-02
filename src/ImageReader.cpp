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

#include "ImageReader.h"

using namespace ZedGrabber;

ImageReader::ImageReader(const std::string& name)
{
  frameName = name + "/frames/";
  depthName = name + "/depths/";
 
  frameReader = std::shared_ptr<ImageManager>(new ImageManager(frameName));
  depthReader = std::shared_ptr<ImageManager>(new ImageManager(depthName));
  
  end = frameReader->getEnd();
  
  i = 0;
}

bool ImageReader::finish()
{

}

cv::Mat ImageReader::convertImage(const std::string& name)
{
  std::ifstream frameReader (name, std::ios::in | std::ios::binary);
  if (!frameReader.is_open()) 
  {
    std::cout << "No file found: " << name << std::endl;
    exit(-1);
  }
  int width, height, channels;
  frameReader >> width;
  frameReader >> height;
  frameReader >> channels;
  std::cout << "W: " << width << " H: " << height << " C: " << channels << std::endl;
//  frameReader.read((char *)&width, sizeof(int));
//  frameReader.read((char *)&height, sizeof(int));
//  frameReader.read((char *)&channels, sizeof(int));
  cv::Mat frame(cv::Size(width, height), CV_8UC4);
  std::cout << "TOTAL: " << frame.total() << std::endl;
  frameReader.read((char*)(frame.data), sizeof(uchar)*frame.total()*channels);
  cv::imshow("frame", frame);
  cv::waitKey(0);
  return frame;
}


bool ImageReader::getData()
{
  if(i < end)
  {
    const std::string& nameF = frameReader->next(1);
    const std::string& nameD = depthReader->next(1);
    
    m_frame = cv::imread(nameF, CV_LOAD_IMAGE_COLOR);
    m_depth = cv::imread(nameD, CV_LOAD_IMAGE_ANYDEPTH);
    m_depth16bit = m_depth.clone();
    ++i;
  }
}

