#pragma once

enum e_context_variant : uint32
{
	_context_variant_ctf = 0,
	_context_variant_slayer = 1,
	_context_variant_oddball = 2,
	_context_variant_koth = 3,
	_context_variant_juggernaut = 4,
	_context_variant_territories = 5,
	_context_variant_assault = 6
};

bool* global_set_live_invite_accepted_flag_get(void);

// Set small image for discord interface
void discord_interface_set_small_image(const char* small_image, const char* small_text);

// Set large image for discord interface
void discord_interface_set_large_image(const char* large_image, const char* large_text);

// Set details for discord interface
void discord_interface_set_details(const char* details);

// Set state for discord interface
void discord_interface_set_state(const char* state);

// Update map info for discord interface
void discord_interface_set_map_name(const utf8* scenario_name, const utf8* map_name);

// Update variant info for discord interface
void discord_interface_set_variant(e_context_variant variant, const utf8* variant_name);

// Update difficulty info for discord interface
void discord_interface_set_difficulty(int16 difficulty);

// Update player count for discord interface
void discord_interface_set_player_counts(void);

// Initialize discord interface
void discord_game_status_create(void);

// Deinitialize discord interface
void discord_game_status_dispose(void);
