/*
 *  main.cpp
 *  Riot test
 *
 *  Created by Kyle Weicht on 12/16/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "gtest/gtest.h"

/*******************************************************************\
 External functions
\*******************************************************************/
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
