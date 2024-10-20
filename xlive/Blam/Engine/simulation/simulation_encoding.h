#pragma once
#include "machine_id.h"
#include "simulation_players.h"

#include "memory/bitstream.h"
#include "Networking/Session/NetworkSession.h"

struct simulation_machine_update
{
	uint32 machine_valid_mask;
	s_machine_identifier identifiers[NETWORK_SESSION_PEERS_MAX];
};

void __cdecl simulation_player_update_encode(c_bitstream* stream, const simulation_player_update* player_update);
bool __cdecl simulation_player_update_decode(c_bitstream* stream, simulation_player_update* player_update);
