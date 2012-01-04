/*
 *  application.cpp
 *  riot test
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */

/*
#include "application/application.h"
#include "gtest/gtest.h"

void TestFrameCallback(void)
{  
    EXPECT_EQ(application_status_e::kRunning, Application::GetStatus());
    Application::Shutdown();
}

TEST(Application, Application)
{
    EXPECT_EQ(application_status_e::kNotStarted, Application::GetStatus());

    Application::Initialize();
    EXPECT_EQ(application_status_e::kStarted, Application::GetStatus());

    Application::SetFrameCallback(TestFrameCallback);    
    Application::StartMainLoop();
    
    EXPECT_EQ(application_status_e::kStopped, Application::GetStatus());
}
*/
