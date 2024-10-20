#pragma once
#include "game/aim_assist.h"


class c_player_input
{
public:
	real_vector2d throttle;
	real32 trigger;
	real32 secondary_trigger;
	real32 yaw_change;
	real32 pitch_change;
	uint32 field_18;
	uint32 flags_1C;
	uint32 flags_20;
	uint32 flags_24;
	real_euler_angles2d desired_angles;
	s_aim_assist_targeting_result targeting;
	int8 field_50[4];
};

struct s_player_action_weapons
{
	int8 primary_weapon_index;
	int8 secondary_weapon_index;
};

struct player_action
{
	real32 control_flag0;
	real32 control_flag1;
	real_euler_angles2d facing;
	real_point2d throttle;
	real32 trigger;
	real32 secondary_trigger;
	uint32 action_flags;
	uint16 weapon_set_identifier;
	s_player_action_weapons weapon_indexes;
	uint16 grenade_index;
	uint16 zoom_level;
	int32 interaction_type;
	int32 interaction_object;
	int32 melee_target_unit;
	s_aim_assist_targeting_result aim_assist_data;
	int32 unk;
};
ASSERT_STRUCT_SIZE(player_action, 0x60);

struct s_player_control
{
	datum unit_datum_index;
	int32 control_flag;
	int32 field_8;
	int32 field_C;
	int32 field_10;
	int32 field_14;
	player_action actions;
	int8 gap_78[4];
	uint32 action_context;
	int8 gap_80[25];
	bool zoom_input_held;
	int8 gap_9A[2];
	int16 zoom_input_updates_held_count;
	int8 gap_9E;
	int8 field_9F;
	int8 gap_A0[12];
	int32 field_AC;
};
ASSERT_STRUCT_SIZE(s_player_control, 0xB0);	

struct s_player_control_globals
{
	bool initialized;
	int8 pad_1[3];
	int32 field_4;
	int32 field_8;
	int32 field_C;
	bool disable_camera;
	int8 pad[7];
	s_player_control local_players[4];
};
ASSERT_STRUCT_SIZE(s_player_control_globals, 728);

void player_control_disable_local_camera(bool state);

s_player_control_globals* player_control_globals_get(void);
s_player_control* player_control_get(int32 user_index);

void player_control_update_dt(real32 dt);
void player_control_apply_patches();
