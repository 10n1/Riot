/*
 * GraphicsDevice.cpp
 * RiotTest
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
/* C++ headers */
/* External headers */
#include "UnitTest++.h"
/* Internal headers */
#include "graphicsDevice.h"

TEST(GraphicsDeviceCreate)
{
    GraphicsDevice* device = GraphicsDevice::Create(GraphicsAPI::kNull, NULL);
    CHECK(device);
    CHECK(device->GetAPI() == GraphicsAPI::kNull);
    device->Destroy();
}
