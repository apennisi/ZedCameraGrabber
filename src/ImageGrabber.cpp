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

#include "ImageGrabber.h"

using namespace ZedGrabber;

ImageGrabber::ImageGrabber(const std::string& name, const sl::zed::ZEDResolution_mode& resolution, const int& confidence, const int& exposure)
    : name_(name), ConfidenceIdx(confidence)
{
    zedCamera = std::shared_ptr<sl::zed::Camera>(new sl::zed::Camera(resolution, 15));
    // define a struct of parameters for the initialization
    sl::zed::InitParams params;
    params.mode = sl::zed::QUALITY;
    params.verbose = true;
   
    sl::zed::ERRCODE err = zedCamera->init(params);

    if (err != sl::zed::SUCCESS) {
        exit(-1);
    }


    width = zedCamera->getImageSize().width;
    height = zedCamera->getImageSize().height;

    _depth = zedCamera->retrieveMeasure(sl::zed::MEASURE::DEPTH); // Get the pointer

    // the depth is limited to 20. METERS as define in zed::init()
    zedCamera->setDepthClampValue(10000);

    //Jetson only. Execute the calling thread on core 2
    //sl::zed::Camera::sticktoCPUCore(4);

    sl::zed::ZED_SELF_CALIBRATION_STATUS old_self_calibration_status = sl::zed::SELF_CALIBRATION_NOT_CALLED;
    zedCamera->setCameraSettingsValue(sl::zed::ZED_EXPOSURE, exposure);

    m_frame = cv::Mat::zeros(height, width, CV_8UC4);
    m_depth = cv::Mat::zeros(height, width, CV_8UC4);
    
    counter = 0;
    
    startV = false;
    endV = false;
   
    m_init = false;
    
    launchWriter = true;
    writer = NULL;
    
    done = false;
    
    double cx, cy, fx, fy;
    switch(resolution)
    {
      case sl::zed::HD720:
      {
	cx = 652.636;
	cy = 338.7;
	fx = 699.2;
	fy = 699.2;
	break;
      }
      case sl::zed::HD1080:
      {
	cx = 988.273;
	cy = 500.399;
	fx = 1398.4;
	fy = 1398.4;
	break;
      }
      case sl::zed::HD2K:
      {
	cx = 1132.27;
	cy = 581.399;
	fx = 1398.4;
	fy = 1398.4;
	break;
      }
      case sl::zed::VGA:
      {
	cx = 341.818;
	cy = 176.85;
	fx = 349.6;
	fy = 349.6;
      }
      default:
      {
	std::cout << "No resolution recognized!" << std::endl;
	exit(-1);
      }
    }
    
     
    std::ofstream file;
    file.open(name + "/parameters.xml");
    file << "<parameters>" << std::endl;
    file << "\t<cx>" << cx << "</cx>" << std::endl;
    file << "\t<cy>" << cy << "</cy>" << std::endl;
    file << "\t<fx>" << fx << "</fx>" << std::endl;
    file << "\t<fy>" << fx << "</fy>" << std::endl;
    file << "\t<tx>0.</tx>" << std::endl;
    file << "\t<ty>0.</ty>" << std::endl;
    file << "\t<theta>0.</theta>" << std::endl;
    file << "\t<min_height>1.0</min_height>" << std::endl;
    file << "\t<max_height>2.0</max_height>" << std::endl;
    file << "\t<max_distance>10.0</max_distance>" << std::endl;
    file << "\t<ground_threshold>0.10</ground_threshold>" << std::endl;
    file << "\t<min_cluster_points>100</min_cluster_points>" << std::endl;
	file << "\t<max_cluster_points>10000</max_cluster_points>" << std::endl;
	file << "\t<voxel_size>0.06</voxel_size>" << std::endl;
    file << "</parameters>" << std::endl;
    file.close();
    std::cout << "CAMERA PARAMETERS SAVED TO: " << name + "/parameters.xml" << std::endl;
    std::cout << "!!!IMPORTANT: Edit your file and set the other parameters!" << std::endl;
}

bool ImageGrabber::finish()
{
  mtx.lock();
  const bool& doneCopy = done;
  mtx.unlock();
  if(!doneCopy)
  {
    writer->join();
    delete writer;
    writer = NULL;
  }
}

void ImageGrabber::saveFrames(int _counter, const std::vector< ZedGrabber::MyCvMat >& frames, const std::string& name)
{
  std::ofstream frameWriter;
  std::stringstream ss;
  for(uint i = 0; i < frames.size(); ++i)
  {
    const cv::Mat& frame = frames[i].getMat();
    ss.str("");
    ss << name << "_" << _counter++ << ".png";
    cv::imwrite(ss.str(), frame);
  }
}

void ImageGrabber::launchConsumers(bool& done)
{
  std::cout << "Writing " << threadDepths.size() << " images" << std::endl;
  
  std::thread frameThread(std::bind(&ImageGrabber::saveFrames, this, counter, threadFrames, name_+"/frames/frame"));
  std::thread depthThread(std::bind(&ImageGrabber::saveFrames, this, counter, threadDepths, name_+"/depths/depth"));
  
  frameThread.join();
  depthThread.join();
  
  counter += threadDepths.size();
  threadDepths.clear();
  threadFrames.clear();
  
  std::cout << "End writing." << std::endl;
  std::cout << "Total number of images:  " << counter << std::endl;
  
  mtx.lock();
  done = true;
  mtx.unlock();
}



bool ImageGrabber::getData()
{

    // DisparityMap filtering
    zedCamera->setConfidenceThreshold(ConfidenceIdx);

    //bool res = zedCamera->grab(dm_type);
	  
    if(/*!res*/!zedCamera->grab(sl::zed::SENSING_MODE::STANDARD))
    {
	if(!startV) { start = duration_cast< milliseconds >(system_clock::now().time_since_epoch()); startV = true; }
	_depth = zedCamera->retrieveMeasure(sl::zed::MEASURE::DEPTH); // Get the pointer
	slMat2cvMat(zedCamera->retrieveImage(static_cast<sl::zed::SIDE> (0))).copyTo(m_frame);
	
	slMat2cvMat(zedCamera->normalizeMeasure(sl::zed::MEASURE::DEPTH)).copyTo(m_depth);
	cv::Mat depth32bit = cv::Mat(height, width, CV_32FC1, _depth.data);
	
	depth32bit.convertTo(m_depth16bit, CV_16UC1);
	end = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
	
	frames.push_back(ZedGrabber::MyCvMat(m_frame));
	depths.push_back(ZedGrabber::MyCvMat(m_depth16bit));
	
	
	if(end.count() - start.count() >= 3000) //3 seconds;
	{
	  mtx.lock();
	  if (done || launchWriter) 
	  {
	    if(!launchWriter)
	    {
	      writer->join();
	      delete writer;
	      writer = NULL;
	    }
	    
	    std::swap(threadFrames, frames);
	    std::swap(threadDepths, depths);
	    
	    /*for(uint i = 0; i < frames.size(); ++i)
	    {
	      threadFrames.push_back(frames.at(i).clone());
	      threadDepths.push_back(depths.at(i).clone());
	    }*/
	    
	    done = false;
	    writer = new std::thread(std::bind(&ImageGrabber::launchConsumers, this, std::ref(done)));
	    launchWriter = false;
	    startV = false;
	    frames.clear();
	    depths.clear();
	    
	  }
	  mtx.unlock();
	}
    }
    return true;
}
