/*
 * materials.h
 * game
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __game_materials_h__
#define __game_materials_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */

/*******************************************************************\
External Constants And types
\*******************************************************************/
enum material_type_e
{
    kNothing = 0,
    kDirt,
    kGrass,
    kRock,
    
    kMAX_MATERIAL_TYPES,
};

struct tixel_template_t
{
    uint8_t         color[4];
    material_type_e type;
    int             maxDurability;
};
struct tixel_t
{
    material_type_e type;
    int durability;
    
    tixel_t operator=(const tixel_template_t& temp)
    {
        tixel_t newTixel = *this;
        newTixel.type       = temp.type;
        newTixel.durability = temp.maxDurability;

        return newTixel;
    }
};

/*******************************************************************\
External variables
\*******************************************************************/
static const tixel_template_t kMaterialTemplates[kMAX_MATERIAL_TYPES] =
{
    /* Type          Color              Type        Durability */
    /* kNothing */ { {0,0,0,0},         kNothing,   0 },
    /* kDirt    */ { {148,100,12,255},  kDirt,      0 },
    /* kGrass   */ { {25,200,50,255},   kGrass,     0 },
    /* kRock    */ { {128,128,128,255}, kRock,      0 },
};

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
