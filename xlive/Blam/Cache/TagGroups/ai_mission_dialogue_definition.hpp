#pragma once
#include "Blam/Cache/DataTypes/BlamDataTypes.h"

/*********************************************************************
* name: ai_mission_dialogue
* group_tag : mdlg
* header size : 8
* *********************************************************************/

#pragma pack(push,1)
struct s_ai_mission_dialogue_group_definition
{
	struct s_lines_block
	{
		string_id name;//0x0
		struct s_variants_block
		{
			string_id variant_designation;//0x0
			tag_reference sound;//0x4
			string_id sound_effect;//0xC
		};
		ASSERT_STRUCT_SIZE(s_variants_block, 0x10);
		tag_block<s_variants_block> variants;//0x4
		string_id default_sound_effect;//0xC
	};
	ASSERT_STRUCT_SIZE(s_lines_block, 0x10);
	tag_block<s_lines_block> lines;//0x0
};
#pragma pack(pop)
ASSERT_STRUCT_SIZE(s_ai_mission_dialogue_group_definition, 0x8);

