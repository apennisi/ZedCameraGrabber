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

#ifndef _IMAGEGRABBER_H_
#define _IMAGEGRABBER_H_

#include "Grabber.h"

//ZED Includes
#include <zed/Camera.hpp>
#include <zed/utils/GlobalDefine.hpp>
// 
//C++
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <queue>
#include <fstream>
#include "mycvmat.h"

namespace ZedGrabber {
  
    using namespace std::chrono;

    class ImageGrabber : public Grabber
    {
        public:
            ImageGrabber (const std::string& name, const sl::zed::ZEDResolution_mode& resolution, const int& confidence, const int& exposure);
            ImageGrabber () { ; }
            virtual bool getData();
	    virtual bool finish();
        private:
            std::shared_ptr<sl::zed::Camera> zedCamera;
            std::string name_;
            sl::zed::Mat _depth;
            int ConfidenceIdx;
	    uint counter;
	    std::mutex mtx;
	    std::vector<ZedGrabber::MyCvMat> frames;
	    std::vector<ZedGrabber::MyCvMat> depths;
	    std::vector<ZedGrabber::MyCvMat> threadFrames;
	    std::vector<ZedGrabber::MyCvMat> threadDepths;
	    std::thread *writer;
	    bool launchWriter;
	    milliseconds start, end;
	    bool startV, endV;
	    bool done;
	private:
	  void launchConsumers(bool &done);
	  void saveFrames(int _counter, const std::vector<ZedGrabber::MyCvMat >& frames, const std::string& name);
    };
}

#endif
