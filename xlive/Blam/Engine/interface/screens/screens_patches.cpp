#include "stdafx.h"
#include "screens_patches.h"

#include "screen_esrb_warning.h"
#include "screen_xbox_live_task_progress_dialog.h"

void screens_apply_patches_on_map_load()
{
	c_xbox_live_task_progress_menu::apply_patches();
}

void screens_apply_patches()
{
	c_screen_esrb_warning::apply_patches();
}
