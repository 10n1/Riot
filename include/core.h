/*
 * core.h
 * RiotLib
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_core_h__
#define __RiotLib_core_h__

/* C headers */
#include <string.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "cJSON.h"
#include "graphicsDevice.h"

#define INT_FROM_JSON(json, objectName)                         \
    {                                                           \
        cJSON* _json_ = cJSON_GetObjectItem(json, #objectName); \
        if(_json_)                                              \
            objectName = _json_->valueint;                      \
    }


/*******************************************************************\
External Constants And types
\*******************************************************************/
class GraphicsDevice;

struct engine_params_t
{
    int                 createWindow;
    int                 windowWidth;
    int                 windowHeight;
    GraphicsAPI::Enum   graphicsApi;

    engine_params_t(cJSON* json = NULL)
        : createWindow(0)
        , windowWidth(0)
        , windowHeight(0)
        , graphicsApi(GraphicsAPI::kNull)
    {
        if(json == NULL)
            return;

        INT_FROM_JSON(json, createWindow);
        INT_FROM_JSON(json, windowWidth);
        INT_FROM_JSON(json, windowHeight);
        
        cJSON* jsonGraphicsApi = cJSON_GetObjectItem(json, "graphicsApi");

        if(jsonGraphicsApi)
        {
            if(stricmp("directx", jsonGraphicsApi->string))
                graphicsApi = GraphicsAPI::kDirectX;
            else if(stricmp("opengl", jsonGraphicsApi->string))
                graphicsApi = GraphicsAPI::kOpenGL;
        }
    }
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
class Core
{
/* Methods */
public:
    void Init(const engine_params_t& params);
    int Frame(void);
    void Shutdown(void);

    int frameNumber(void) const { return _frameNumber; }
/* Members */
private:
    GraphicsDevice* _graphicsDevice;
    int             _frameNumber;
};

#endif /* include guard */
