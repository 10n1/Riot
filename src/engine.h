//
//  engine.h
//  Riot
//
//  Created by Kyle Weicht on 7/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "types.h"
#include "config.h"
#include "assert.h"

class Engine
{
public:
    Engine();
    ~Engine();
    
    void Init(void);
    void Frame(void);
    void Shutdown(void);
    
private:
    
};
