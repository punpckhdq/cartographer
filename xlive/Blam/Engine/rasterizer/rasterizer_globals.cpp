#include "stdafx.h"
#include "rasterizer_globals.h"

/* public code */

s_rasterizer_globals* rasterizer_globals_get(void)
{
	return Memory::GetAddress<s_rasterizer_globals*>(0xA3E400);
}

s_game_globals_rasterizer_data* rasterizer_globals_get_data(void)
{
	return *Memory::GetAddress<s_game_globals_rasterizer_data**>(0xA3DA3C);
}

void __cdecl rasterizer_get_screen_bounds(rectangle2d* screen_bounds)
{
	*screen_bounds = rasterizer_globals_get()->screen_bounds;
	return;
}

void __cdecl rasterizer_get_frame_bounds(rectangle2d* frame_bounds)
{
	*frame_bounds = rasterizer_globals_get()->frame_bounds;
	return;
}
