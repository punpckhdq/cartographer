#include "stdafx.h"
#include "Christmas.h"

#include "../SpecialEventHelpers.h"

#include "items/weapon_definitions.h"
#include "game/game_globals.h"
#include "models/models.h"
#include "scenario/scenario.h"
#include "tag_files/tag_loader/tag_injection.h"
#include "units/biped_definitions.h"

#include "H2MOD/Tags/MetaExtender.h"

void christmas_event_map_load(void)
{
	// Halo 2 tags
	datum sword_weapon_datum = tag_loaded(_tag_group_weapon, "objects\\weapons\\melee\\energy_blade\\energy_blade");
	datum ghost_datum = tag_loaded(_tag_group_vehicle, "objects\\vehicles\\ghost\\ghost");
	datum frag_model_datum = tag_loaded(_tag_group_model, "objects\\weapons\\grenade\\frag_grenade\\frag_grenade_projectile");
	datum plasma_model_datum = tag_loaded(_tag_group_model, "objects\\weapons\\grenade\\plasma_grenade\\plasma_grenade");
	datum ball_weapon_datum = tag_loaded(_tag_group_weapon, "objects\\weapons\\multiplayer\\ball\\ball");
	datum bomb_weapon_datum = tag_loaded(_tag_group_weapon, "objects\\weapons\\multiplayer\\assault_bomb\\assault_bomb");

	// Carto Shared tags
	tag_injection_set_active_map(k_events_map);

	if (!tag_injection_active_map_verified())
		return;

	datum santa_hat_datum = tag_injection_load(_tag_group_scenery, "scenarios\\objects\\multi\\christmas_hat_map\\hat\\hat", true);
	datum beard_datum = tag_injection_load(_tag_group_scenery, "objects\\multi\\stpat_hat\\beard\\santa_beard", true);
	datum snow_datum = tag_injection_load(_tag_group_weather_system, "scenarios\\multi\\lockout\\lockout_big", true);
	datum candy_cane_datum = tag_injection_load(_tag_group_render_model, "scenarios\\objects\\multi\\carto_shared\\candy_cane\\candy_cane", true);
	datum deer_datum = tag_injection_load(_tag_group_render_model, "scenarios\\objects\\multi\\carto_shared\\reindeer_ghost\\reindeer_ghost", true);
	datum ornament_datum = tag_injection_load(_tag_group_render_model, "scenarios\\objects\\multi\\carto_shared\\ornament\\ornament", true);
	datum present_datum = tag_injection_load(_tag_group_render_model, "scenarios\\objects\\multi\\carto_shared\\present\\present", true);
	datum fp_present_datum = tag_injection_load(_tag_group_render_model, "scenarios\\objects\\multi\\carto_shared\\present\\fp_present", true);


	if (santa_hat_datum != NONE &&
		beard_datum != NONE &&
		snow_datum != NONE &&
		candy_cane_datum != NONE &&
		sword_weapon_datum != NONE &&
		deer_datum != NONE &&
		ghost_datum != NONE &&
		ornament_datum != NONE &&
		frag_model_datum != NONE &&
		plasma_model_datum != NONE &&
		present_datum != NONE &&
		fp_present_datum != NONE &&
		ball_weapon_datum != NONE &&
		bomb_weapon_datum != NONE)
	{
		tag_injection_inject();

		// Give Santa Hat and Beard to Chief & Friends
		if (datum hlmt_chief_datum = tag_loaded(_tag_group_model, "objects\\characters\\masterchief\\masterchief");
			hlmt_chief_datum != NONE)
		{
			add_hat_and_beard_to_model(hlmt_chief_datum, santa_hat_datum, beard_datum);
		}
		if (datum hlmt_chief_mp_datum = tag_loaded(_tag_group_model, "objects\\characters\\masterchief\\masterchief_mp");
			hlmt_chief_mp_datum != NONE)
		{
			add_hat_and_beard_to_model(hlmt_chief_mp_datum, santa_hat_datum, beard_datum);
		}
		if (datum hlmt_elite_datum = tag_loaded(_tag_group_model, "objects\\characters\\elite\\elite_mp");
			hlmt_elite_datum != NONE)
		{
			add_hat_and_beard_to_model(hlmt_elite_datum, santa_hat_datum, beard_datum, true);
		}

		if (datum flood_datum = game_globals_get_representation(_character_type_flood)->third_person_unit.index;
			flood_datum != NONE)
		{
			_biped_definition* flood_biped = (_biped_definition*)tag_get_fast(flood_datum);
			add_hat_and_beard_to_model(flood_biped->unit.object.model.index, santa_hat_datum, beard_datum, false);
		}

		// Change/Add weather system to bsp
		structure_bsp* bsp_definition = (structure_bsp*)tag_get_fast(get_global_scenario()->structure_bsps[0]->structure_bsp.index);
		structure_weather_palette_entry* weat_block = MetaExtender::add_tag_block2<structure_weather_palette_entry>((unsigned long)std::addressof(bsp_definition->weather_palette));
		weat_block->name.set("snow_cs");
		weat_block->weather_system.group.group = _tag_group_weather_system;
		weat_block->weather_system.index = snow_datum;

		for (auto& cluster : bsp_definition->clusters)
		{
			cluster.weather_index = (short)bsp_definition->weather_palette.count - 1;
		}

		// Change sword model to candy cane
		_weapon_definition* sword_weapon = (_weapon_definition*)tag_get_fast(sword_weapon_datum);

		datum sword_model_datum = sword_weapon->item.object.model.index;
		s_model_definition* sword_model = (s_model_definition*)tag_get_fast(sword_model_datum);

		sword_model->render_model.index = candy_cane_datum;

		for (auto& first_person : sword_weapon->player_interface.first_person)
			first_person.model.index = candy_cane_datum;

		for (auto& attachment : sword_weapon->item.object.attachments)
		{
			attachment.type.index = NONE;
			attachment.type.group = { (e_tag_group)NONE };
			attachment.marker = 0;
			attachment.primary_scale = 0;
		}

		// Change ghost to reindeer
		_unit_definition* ghost_vehicle = (_unit_definition*)tag_get_fast(ghost_datum);
		ghost_vehicle->object.attachments.data = 0;
		ghost_vehicle->object.attachments.count = 0;

		datum ghost_model_datum = ghost_vehicle->object.model.index;
		s_model_definition* ghost_model = (s_model_definition*)tag_get_fast(ghost_model_datum);
		ghost_model->render_model.index = deer_datum;

		s_model_definition* frag_model = (s_model_definition*)tag_get_fast(frag_model_datum);
		frag_model->render_model.index = ornament_datum;

		s_model_definition* plasma_model = (s_model_definition*)tag_get_fast(plasma_model_datum);
		plasma_model->render_model.index = ornament_datum;

		// Change the models for objectives to presents
		replace_fp_and_3p_models_from_weapon(ball_weapon_datum, fp_present_datum, present_datum);
		replace_fp_and_3p_models_from_weapon(bomb_weapon_datum, fp_present_datum, present_datum);
	}
	
	return;
}
