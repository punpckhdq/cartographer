#pragma once

#include "objects/object_identifier.h"
#include "tag_files/tag_block.h"

#define k_maximum_machine_door_portal_associations 128

// max: k_maximum_machine_door_portal_associations
struct s_structure_portal_device_machine_association
{
	c_object_identifier device_id;
	uint16 first_portal_index;
	uint16 game_portal_count;
};
ASSERT_STRUCT_SIZE(s_structure_portal_device_machine_association, 12);

// max: 1
struct s_structure_portal_device_map
{
	tag_block<s_structure_portal_device_machine_association> device_portal_associations;

	// max: k_maximum_machine_door_portal_associations
	tag_block<int16> game_portal_to_portal_map;
};
ASSERT_STRUCT_SIZE(s_structure_portal_device_map, 16);