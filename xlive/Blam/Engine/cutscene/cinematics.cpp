#include "stdafx.h"
#include "cinematics.h"

#include "game/game.h"


s_cinematic_globals* get_cinematic_globals(void)
{
	return *Memory::GetAddress<s_cinematic_globals**>(0x48227C, 0x4F3DB0);
}

bool cinematic_is_running()
{
	bool result = false;

	if (game_in_progress() && game_is_campaign()) 
	{ 
		s_cinematic_globals* cinematic_globals = get_cinematic_globals();
		if (cinematic_globals)
		{
			result = cinematic_globals->flags.cinematic_in_progress;
		}
	}

	return result;
}

// Same as above but hook used to disable framerate cap when playing cinematics
bool __cdecl cinematics_in_progress_disable_framerate_cap_hook(void)
{
	bool result = false;
	// don't limit the game framerate if we're single player and playing cinematics
	if (game_is_campaign())
	{
		result = false;
	}
	else
	{
		result = cinematic_is_running();
	}

	return result;
}

bool cinematic_in_progress(void)
{
	bool result = false;
	s_cinematic_globals* cinematic_globals = get_cinematic_globals();

	if (cinematic_globals)
	{
		result = cinematic_globals->flags.cinematic_in_progress;
	}

	return result;
}

void cinematics_draw_line(rectangle2d* points, pixel32 rect_color)
{
	INVOKE(0x3B101, 0x0, cinematics_draw_line, points, rect_color);
	return;
}

void cinematics_apply_patches(void)
{
	// allow cinematics to run at 60 fps
	// Don't run these since interpolation does not currently work in cutscenes
	// PatchCall(Memory::GetAddress(0x97774), cinematics_in_progress_disable_framerate_cap_hook);
	// PatchCall(Memory::GetAddress(0x7C378), cinematics_in_progress_disable_framerate_cap_hook);
	return;
}