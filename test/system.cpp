/*
 * main.cpp
 * Riot
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
/* C++ headers */
/* External headers */
#include "UnitTest++.h"
/* Internal headers */
#include "system.h"

TEST(SystemCreateShutdown)
{
    CHECK(System::IsRunning() == 0);
    System::Init(0, 0, 0);
    CHECK(System::IsRunning() == 1);
    System::Shutdown();
    CHECK(System::IsRunning() == 0);
}

TEST(SystemGetWindow)
{
    CHECK(System::GetWindow() == NULL);
}

TEST(SystemPollEvents)
{
    System::PollEvents();
}

TEST(SystemWindowSize)
{
    int width, height;
    System::GetWindowSize(&width, &height);
    CHECK(width == 0);
    CHECK(height == 0);
}

TEST(SystemCreateWindow)
{   
    System::Init(1, 128, 128);
    CHECK(System::GetWindow());
    CHECK(System::IsRunning());

    int frame = 0;
    while(System::IsRunning())
    {
        if(++frame == 10)
        {
            System::Stop();
        }
    }

    System::Shutdown();
}

TEST(SystemInput)
{
    System::Init(0,0,0);
    for(int ii=0; ii<System::Key::kMAX_KEYS; ++ii)
    {
        System::Key::Enum key = (System::Key::Enum)ii;
        CHECK(System::GetKeyState(key) == 0);
    }

    CHECK(System::GetMouseButtonState(System::Mouse::kLeft) == 0);
    CHECK(System::GetMouseButtonState(System::Mouse::kRight) == 0);
    CHECK(System::GetMouseButtonState(System::Mouse::kMiddle) == 0);

    int x, y;
    System::GetMousePosition(&x, &y);
    CHECK(x == 0);
    CHECK(y == 0);

    System::Shutdown();
}
