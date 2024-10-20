#pragma once
#include "math/color_math.h"


// max: 1
struct render_lighting
{
	real_rgb_color ambient;
	real_vector3d shadow_direction;
	real32 lighting_accuracy;
	real32 shadow_opacity;
	real_rgb_color primary_direction_color;
	real_vector3d primary_direction;
	real_rgb_color secondary_direction_color;
	real_vector3d secondary_direction;
	uint16 sh_index;
	int16 pad;
};
ASSERT_STRUCT_SIZE(render_lighting, 84);


void __cdecl render_lights(void);

void __cdecl render_light_clear_data(void);
