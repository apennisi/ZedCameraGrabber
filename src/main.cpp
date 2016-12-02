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


#include <iostream>
#include "Grabber.h"
#include "ImageGrabber.h"
#include "ImageReader.h"
#include "mosaic.h"

bool grab = false;
bool readImg = false;
std::string directory;
std::shared_ptr<ZedGrabber::Grabber> grabber;
cv::Size displaySize(720, 404);
cv::Mat frame, depth;
sl::zed::ZEDResolution_mode resolution = sl::zed::HD720;
int confidenceIdx = 70;
int exposure = 40;

inline void help()
{
  std::cout << "HELP" << std::endl;
  std::cout << "\tFor recording: ./grabber -grab /path/to/the/directory/" << std::endl;
  std::cout << "\tFor reading: ./grabber -read /path/to/the/directory/" << std::endl;
  std::cout << "\tOptionally you can add:\n\t\t -res HD720 (HD1080, HD2K, VGA) to specify the resolution. The default value is HD720." << std::endl;
  std::cout << "\t\t -conf: 100 to specify the confidence of the depth. The default value is 70." << std::endl;
  std::cout << "\t\t -exp: 100 to specify the exposure. The default value is 40." << std::endl;
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}


inline void argAnalyzer(int argc, char **argv)
{
  if(argc == 2 || argc == 3 || argc == 5 || argc == 7 || argc == 9)
  {
    std::cout << "qui" << std::endl;
    bool resSpec = false;
    bool confidence = false;
    bool exp = false;
    for(uint i = 1; i < argc; ++i)
    {
      if(std::string(argv[i]).compare("-help") == 0)
      {
	help();
	exit(-1);
      }
      else if(std::string(argv[i]).compare("-grab") == 0)
      {
	grab = true;
	directory = std::string(argv[++i]);
	std::stringstream ss;
	ss << "mkdir " << directory << "/frames";
	system(ss.str().c_str());
	ss.str("");
	ss << "mkdir " << directory << "/depths";
	system(ss.str().c_str());
	std::cout << "Directory: " << directory << std::endl;
      }
      else if(std::string(argv[i]).compare("-read") == 0)
      {
	readImg = true;
	directory = std::string(argv[++i]);
	std::cout << "Directory: " << directory << std::endl;
      }
      else if(std::string(argv[i]).compare("-res") == 0)
      {
	std::string res = std::string(argv[++i]);
	switch(str2int(res.c_str()))
	{
	  case str2int("HD1080"):
	  {
	    resolution = sl::zed::HD1080;
	    std::cout << "Resolution: HD1080" << std::endl;
	    resSpec = true;
	    break;
	  }
	  case str2int("HD2K"):
	  {
	    resolution = sl::zed::HD2K;
	    std::cout << "Resolution: HD2K" << std::endl;
	    resSpec = true;
	    break;
	  }
	  case str2int("HD720"):
	  {
	    std::cout << "Resolution: HD720" << std::endl;
	    resolution = sl::zed::HD720;
	    resSpec = true;
	    break;
	  }
	  case str2int("VGA"):
	  {
	    std::cout << "Resolution: VGA" << std::endl;
	    resolution = sl::zed::VGA;
	    resSpec = true;
	    break;
	  }
	  default:
	  {
	    std::cout << "The resoluton does not exist!" << std::endl;
	    exit(-1);
	  }
	}
      }
      else if(std::string(argv[i]).compare("-conf") == 0)
      {
	confidenceIdx = atoi(argv[i++]);
	std::cout << "Confidence: " << confidenceIdx << std::endl;
	confidence = true;
	break;
      }
      else if(std::string(argv[i]).compare("-exp") == 0)
      {
	exposure = atoi(argv[i++]);
	exp = true;
	break;
      }
      else
      {
	std::cout << "No arguments detected!" << std::endl;
	help();
	exit(-1);
      }
      
    }
    
    if(!readImg)
    {
      if(!resSpec)
	std::cout << "Default resolution: HD720" << std::endl;
      
      if(!confidence)
	std::cout << "Defult Depth Confidence: " << confidenceIdx << std::endl;
      
      if(!exp)
	std::cout << "Default Eposure: " << exposure << std::endl;
    }
  }
  else
  {
    help();
    exit(-1);
  }
}


int main(int argc, char **argv)
{
    argAnalyzer(argc, argv);
    if(grab)
    {
      grabber = std::shared_ptr<ZedGrabber::Grabber>((new ZedGrabber::ImageGrabber(directory, resolution, confidenceIdx, exposure)));
    }
    else if(read)
    {
      grabber = std::shared_ptr<ZedGrabber::Grabber>((new ZedGrabber::ImageReader(directory)));
    }
    char key = ' ';
    
    while (key != 'q')
    {
      grabber->getData();
      cv::resize(grabber->frame(), frame, displaySize);
      cv::resize(grabber->depth16bit(), depth, displaySize);
      cv::imshow("RGB + Depth", ZedGrabber::Mosaic::instance()->mosaic(frame, depth));
      key = cv::waitKey(30);
    }
    grabber->finish();
    return 0;
}
