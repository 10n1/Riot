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

TEST(SystemInput)
{
    for(int ii=0; ii<System::Key::kMAX_KEYS; ++ii)
    {
        System::Key::Enum key = (System::Key::Enum)ii;
        CHECK(System::GetKeyState(key) == 0);
    }

    CHECK(System::GetMouseButtonState(System::Mouse::kLeft));
    CHECK(System::GetMouseButtonState(System::Mouse::kRight));
    CHECK(System::GetMouseButtonState(System::Mouse::kMiddle));

    int x, y;
    System::GetMousePosition(&x, &y);
    CHECK(x == 0);
    CHECK(y == 0);
}