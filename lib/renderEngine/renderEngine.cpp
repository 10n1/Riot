/*
 * renderEngine.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/4/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "renderEngine.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "graphicsDevice.h"
#include "system.h"
#include "utility.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
template<class T, int kMaxCount>
class ResourceList
{
/* Methods */
public:

/* Members */
private:
    T           _items[kMaxCount];
    StringHash  _names[kMaxCount];
};

/*******************************************************************\
Internal variables
\*******************************************************************/
GraphicsDevice* _graphicsDevice = NULL;

/*******************************************************************\
Internal functions
\*******************************************************************/

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
namespace RenderEngine
{

void Init(const render_engine_params_t& params)
{
    _graphicsDevice = GraphicsDevice::Create((GraphicsAPI::Enum)params.graphicsApi, System::GetWindow());
    
    StringHash myHash("Lolcakes!");
}

void Shutdown(void)
{
    _graphicsDevice->Destroy();
}

} // namespace RenderEngine
