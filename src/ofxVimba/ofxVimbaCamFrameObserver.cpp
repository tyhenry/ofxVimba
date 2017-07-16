#pragma once
#include "ofxVimbaCam.h"

using namespace AVT;
using namespace ofxVimba;

//
// This is our callback routine that will be executed on every received frame.
// Triggered by the API.
//
// Parameters:
//  [in]    pFrame          The frame returned from the API
//
void ofxVimbaCam::FrameObserver::FrameReceived( const VmbAPI::FramePtr pFrame )
{
    bool bQueueDirectly = true;
    VmbFrameStatusType eReceiveStatus;

    if( VmbErrorSuccess == pFrame->GetReceiveStatus( eReceiveStatus ) )
    {
        // Lock the frame queue
        m_FramesMutex.Lock();
        // We store the FramePtr
        m_Frames.push( pFrame );
        // Unlock frame queue
        m_FramesMutex.Unlock();

        // And notify the view about it
        // pMainWin->PostMessage( WM_FRAME_READY, eReceiveStatus );

        bQueueDirectly = false;
    }

    // If any error occurred we queue the frame without notification
    if( true == bQueueDirectly )
    {
        m_pCamera->QueueFrame( pFrame );
    }
}

//
// After the view has been notified about a new frame it can pick it up.
// It is then removed from the internal queue
//
// Returns:
//  A shared pointer to the latest frame
//
VmbAPI::FramePtr ofxVimbaCam::FrameObserver::GetFrame()
{
    // Lock frame queue
    m_FramesMutex.Lock();
    // Pop the frame from the queue
    VmbAPI::FramePtr res;
    if( ! m_Frames.empty() )
    {
        res = m_Frames.front();
        m_Frames.pop();
    }
    // Unlock the frame queue
    m_FramesMutex.Unlock();
    return res;
}

//
// Clears the internal (double buffering) frame queue
//
void ofxVimbaCam::FrameObserver::ClearNewFrames()
{
    // Lock the frame queue
    m_FramesMutex.Lock();
    // Clear the frame queue and release the memory
    std::queue<VmbAPI::FramePtr> empty;
    std::swap( m_Frames, empty );
    // Unlock the frame queue
    m_FramesMutex.Unlock();
}

bool ofxVimbaCam::FrameObserver::HasNewFrame()
{
    return GetNumNewFrames() > 0;
}

int ofxVimbaCam::FrameObserver::GetNumNewFrames()
{
    // Lock the frame queue
    m_FramesMutex.Lock();
    // Get number of frames in queue
	int n = m_Frames.size();
    // Unlock the frame queue
    m_FramesMutex.Unlock();
	return n;
}

VmbErrorType ofxVimbaCam::FrameObserver::QueueFrame(const VmbAPI::FramePtr pFrame)
{
	return m_pCamera->QueueFrame( pFrame );
}