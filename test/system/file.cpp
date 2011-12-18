/*
 *  File.cpp
 *  riot test
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */

#include "system/file.h"
#include "gtest/gtest.h"

TEST(File, Open)
{
    file_t file;
    int result = File::Open(&file, "__notafile__", kFileRead);
    
    EXPECT_FALSE(file.file);
    EXPECT_TRUE(result);
}

TEST(File, CreateWriteRead)
{
    char buffer[1024] = {0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00};
    file_t file;
    size_t bytes = 0;
    int result = 0;
    
    result = File::Open(&file, "testFile", kFileWrite);
    EXPECT_FALSE(result);
    
    result = File::Write(&file, sizeof(buffer), buffer, &bytes);
    EXPECT_EQ(sizeof(buffer), bytes);
    EXPECT_EQ(0, result);
    
    result = File::Write(&file, sizeof(buffer)/2, buffer, &bytes);
    EXPECT_EQ(sizeof(buffer)/2, bytes);
    EXPECT_EQ(0, result);
    
    File::Close(&file);
    EXPECT_EQ(NULL, file.file);
    
    result = File::Open(&file, "testFile", kFileRead);
    EXPECT_FALSE(result);
    
    result = File::Read(&file, sizeof(buffer)/2, buffer, &bytes);
    EXPECT_EQ(sizeof(buffer)/2, bytes);
    EXPECT_EQ(0, result);
    
    result = File::Read(&file, sizeof(buffer), buffer, &bytes);
    EXPECT_EQ(sizeof(buffer), bytes);
    EXPECT_EQ(0, result);
    
    result = File::Read(&file, sizeof(buffer), buffer, &bytes);
    EXPECT_EQ(0, bytes);
    EXPECT_EQ(0, result);
    
    File::Close(&file);
    EXPECT_EQ(NULL, file.file);
}
