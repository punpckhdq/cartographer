#include "stdafx.h"
#include "motion_sensor.h"

#include "rasterizer/dx9/rasterizer_dx9_main.h"
#include "rasterizer/dx9/shaders/compiled/motion_sensor_sweep.h"

#include "hud_definitions.h"
#include "game/game_time.h"
#include "game/players.h"
#include "Networking/Session/NetworkSession.h"


#define k_radar_size_multiplier 1.f
#define k_radar_distance_multiplier 1.5f

real32 g_motion_sensor_observation_accumulator = 0.f;
real32 g_motion_sensor_sample_accumulator = 0.f;
real32 g_motion_sensor_sweeper_accumulator = 0.f;

IDirect3DPixelShader9* motion_sensor_sweep_shader;

void __cdecl motion_sensor_update_examine_nearby_players()
{
	INVOKE(0x22BAE6, 0, motion_sensor_update_examine_nearby_players);
}

void __cdecl motion_sensor_update_sort_examined_objects()
{
	INVOKE(0x22BE3A, 0, motion_sensor_update_sort_examined_objects);
}

void __cdecl motion_sensor_update_examine_near_by_units()
{
	INVOKE(0x22BEAD, 0, motion_sensor_update_examine_near_by_units);
}

void __cdecl motion_sensor_update_move_examined_objects_onto_sample_array(int local_user_index)
{
	INVOKE(0x22C11C, 0, motion_sensor_update_move_examined_objects_onto_sample_array, local_user_index);
}

void c_local_player_motion_sensor_data::sort()
{
	typedef void(__thiscall* sort_function_t)(c_local_player_motion_sensor_data*);
	auto function = Memory::GetAddress<sort_function_t>(0x22B459);
	function(this);
}

s_motion_sensor_globals* get_motion_sensor_globals()
{
	return *Memory::GetAddress< s_motion_sensor_globals**>(0x977110, 0);
}

real32* get_motion_sensor_doppler_scale()
{
	return Memory::GetAddress<real32*>(0x97710C, 0);
}

void set_motion_sensor_doppler_scale(real32 value)
{
	*Memory::GetAddress<real32*>(0x97710C, 0) = value;
}

void __cdecl motion_sensor_update()
{
	INVOKE(0x22C882, 0, motion_sensor_update);
}

void motion_sensor_update_with_delta(real32 delta)
{
	if (delta > 0.f)
	{
		s_motion_sensor_globals* motion_sensor_globals = get_motion_sensor_globals();

		g_motion_sensor_sweeper_accumulator += delta;

		// Originally would use game_ticks as seconds to calculate the sweeper result
		real32 sweeper_accum_ = fmodf(g_motion_sensor_sweeper_accumulator, 2.099999904632568f);

		if (sweeper_accum_ >= 2.0374999f)
		{
			set_motion_sensor_doppler_scale(0.40000001f);
			g_motion_sensor_sweeper_accumulator = 0.0f;
		}
		else
		{
			real32 scaled_tick_time = 1.0f / (sweeper_accum_ + 0.0625f) * 1.1f;
			set_motion_sensor_doppler_scale(scaled_tick_time);
		}

		// Original game functionality would only sort and examine units every 0.5 seconds, not sure why it seems unnecessary
		// Functions perfectly fine just letting them all execute to get the latest viable targets every frame
		motion_sensor_update_sort_examined_objects();
		motion_sensor_update_examine_nearby_players();
		motion_sensor_update_examine_near_by_units();

		// Added accumulation to meet the length of a game tick to preserve the trailing effect of fast moving objects on the radar
		g_motion_sensor_sample_accumulator += delta;
		if (g_motion_sensor_sample_accumulator >= game_tick_length())
		{
			if (motion_sensor_globals->current_sample_index)
			{
				motion_sensor_globals->current_sample_index--;
			}
			else
			{
				motion_sensor_globals->current_sample_index = 9;
			}
			g_motion_sensor_sample_accumulator = 0.f;
		}

		for (int i = 0; i < k_number_of_users; i++)
		{
			if (players_user_is_active(i))
				motion_sensor_update_move_examined_objects_onto_sample_array(i);
		}
	}
}

void __cdecl rasterizer_dx9_create_motion_sensor_shader()
{
	INVOKE(0x28458C, 0x0, rasterizer_dx9_create_motion_sensor_shader);

	rasterizer_dx9_device_get_interface()->CreatePixelShader((const DWORD*)motion_sensor_sweep_pixel_shader_bytecode, &motion_sensor_sweep_shader);
	return;
}

void radar_patch()
{
	PatchCall(Memory::GetAddress(0x263563), rasterizer_dx9_create_motion_sensor_shader);
	WritePointer(Memory::GetAddress(0x2849EA) + 2, &motion_sensor_sweep_shader);
}

void motion_sensor_apply_patches()
{
	radar_patch();
	motion_sensor_fix_size();

	// Remove hud motion sensor update from game tick
	NopFill(Memory::GetAddress(0x220c8f), 5);
}

void motion_sensor_fix_size()
{
	float og_res = *Memory::GetAddress<float*>(0x3E6A9C);
	WriteValue<float>(Memory::GetAddress(0x3E6A9C), og_res * k_radar_size_multiplier);
}