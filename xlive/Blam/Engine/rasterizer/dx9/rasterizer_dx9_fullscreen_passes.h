#pragma once
#include "rasterizer_dx9_targets.h"

/* enums */

enum e_vertex_output_type : int32
{
    _vertex_output_type_position = 0,
    _vertex_output_type_texcoord = 1,
    _vertex_output_type_color = 2
};


/* public code */

void rasterizer_dx9_fullscreen_passes_apply_patches(void);

// ### TOOD rename
void __cdecl rasterizer_dx9_render_fullscreen_overlay_geometry(
    real_rectangle2d* a1,
    unsigned __int8(__cdecl* a2)(int32),
    bool(__cdecl* a3)(
        e_vertex_output_type output_type,
        real_rectangle2d* bounds,
        real_vector4d* location,
        void* output,
        void* ctx),
    int32(__cdecl* a4)(int32),
    int32 a5,
    int16 a6,
    bool a7);

void __cdecl rasterizer_dx9_apply_gamma_and_brightness(e_rasterizer_target rasterizer_target);

bool __cdecl rasterizer_fullscreen_effects_build_vertex_buffer_cb(
    e_vertex_output_type output_type,
    real_rectangle2d* bounds,
    real_vector4d* location,
    void* output,
    void* ctx
);
