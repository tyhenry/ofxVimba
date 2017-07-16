#pragma once

#include "ofxVimba/ofxVimbaCam.h"

namespace ofxVimba
{

// call this function upon application exit to prevent app crashes
static void exit() 
{
	ofxVimbaSystem::getInstance().shutdown();
}

}