#pragma once
#include "Blam/Cache/DataTypes/BlamDataTypes.h"

#include "tag_files/tag_block.h"



/*********************************************************************
* name: camera_track
* group_tag : trak
* header size : 12
* *********************************************************************/

#pragma pack(push,1)
struct s_camera_track_group_definition
{
	enum class e_flags : __int32
	{
	};
	e_flags flags;//0x0
	struct s_control_points_block
	{
		real_vector3d position;//0x0
		real_vector3d orientation;//0xC
		float orientation_w;//0x18
	};
	ASSERT_STRUCT_SIZE(s_control_points_block, 0x1C);
	tag_block<s_control_points_block> control_points;//0x4
};
ASSERT_STRUCT_SIZE(s_camera_track_group_definition, 0xC);
#pragma pack(pop)


