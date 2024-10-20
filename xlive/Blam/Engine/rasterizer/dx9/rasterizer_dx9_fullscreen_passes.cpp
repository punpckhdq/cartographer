#include "stdafx.h"
#include "rasterizer_dx9_fullscreen_passes.h"

#include "rasterizer_dx9.h"
#include "rasterizer_dx9_main.h"
#include "rasterizer_dx9_shader_submit.h"

#include "rasterizer/rasterizer_globals.h"
#include "main/main_screenshot.h"

/* prototypes */

real32 rasterizer_dx9_fullscreen_effect_calculate_position_z(real32 z_far, int32 a2);
void rasterizer_dx9_fullscreen_calculate_position(const real_vector4d* location, real32 z_far, real_vector4d* output);

/* public code */

void rasterizer_dx9_fullscreen_passes_apply_patches(void)
{
    PatchCall(Memory::GetAddress(0x262684), rasterizer_dx9_apply_gamma_and_brightness);
    return;
}

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
    bool a7)
{
    INVOKE(0x27D4EF, 0x0, rasterizer_dx9_render_fullscreen_overlay_geometry, a1, a2, a3, a4, a5, a6, a7);
    return;
}

void __cdecl rasterizer_dx9_apply_gamma_and_brightness(e_rasterizer_target rasterizer_target)
{
    IDirect3DSurface9* backbuffer = NULL;
    const s_frame* global_window_parameters = global_window_parameters_get();
    s_rasterizer_dx9_main_globals* dx9_globals = rasterizer_dx9_main_globals_get();

    IDirect3DDevice9Ex* global_d3d_device = rasterizer_dx9_device_get_interface();

    if (!global_window_parameters->is_texture_camera || *hs_texture_camera_view_get())
    {
        if (screenshot_in_progress())
        {
            backbuffer = dx9_globals->global_d3d_surface_screenshot;
            backbuffer->AddRef();
        }
        else
        {
            global_d3d_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
        }

        const s_rasterizer_globals* rasterizer_globals = rasterizer_globals_get();
        if (rasterizer_globals->fullscreen_parameters.brightness == 0.f && rasterizer_globals->fullscreen_parameters.gamma == 1.f)
        {
            IDirect3DSurface9* target_surface = rasterizer_dx9_get_render_target_surface(rasterizer_target, 0);

            int16 width = rectangle2d_width(&global_window_parameters->camera.viewport_bounds);
            int16 height = rectangle2d_height(&global_window_parameters->camera.viewport_bounds);
            int16 left = global_window_parameters->camera.viewport_bounds.left;
            int16 top = global_window_parameters->camera.viewport_bounds.top;

            const RECT rect =
            {
                left,
                top,
                width + left,
                height + top
            };
            rasterizer_set_render_target_internal_hook_set_viewport(backbuffer, (IDirect3DSurface9*)NONE, true);
            global_d3d_device->StretchRect(target_surface, &rect, backbuffer, &rect, D3DTEXF_NONE);
        }
        else
        {
            rasterizer_dx9_set_sampler_state(0, D3DSAMP_ADDRESSU, 3);
            rasterizer_dx9_set_sampler_state(0, D3DSAMP_ADDRESSV, 3);
            rasterizer_dx9_set_sampler_state(0, D3DSAMP_MAGFILTER, 1);
            rasterizer_dx9_set_sampler_state(0, D3DSAMP_MINFILTER, 1);
            rasterizer_dx9_set_sampler_state(0, D3DSAMP_MIPFILTER, 1);
            rasterizer_dx9_set_sampler_state(0, D3DSAMP_MAXANISOTROPY, 1);
            rasterizer_dx9_set_sampler_state(0, D3DSAMP_MIPMAPLODBIAS, 0);
            rasterizer_dx9_set_sampler_state(0, D3DSAMP_MAXMIPLEVEL, 0);
            rasterizer_dx9_set_render_state(D3DRS_COLORWRITEENABLE, D3DBLEND_INVBLENDFACTOR);
            rasterizer_dx9_set_render_state(D3DRS_ZENABLE, 0);
            rasterizer_dx9_set_render_state(D3DRS_DEPTHBIAS, 0);
            rasterizer_dx9_set_render_state(D3DRS_ALPHATESTENABLE, 0);
            rasterizer_dx9_set_render_state(D3DRS_CULLMODE, D3DBLEND_ZERO);
            if (!*rasterizer_dx9_disable_stencil_get())
            {
                rasterizer_dx9_set_render_state(D3DRS_STENCILENABLE, 0);
            }

            rasterizer_dx9_set_blend_render_state(_framebuffer_blend_function_none);
            rasterizer_set_render_target_internal_hook_set_viewport(backbuffer, (IDirect3DSurface9*)NONE, true);
            rasterizer_dx9_set_target_as_texture(0, rasterizer_target);

            c_rasterizer_constant_4f_cache<32>* main_pixel_shader_constants = rasterizer_get_main_pixel_shader_cache();
            if (main_pixel_shader_constants->test_cache(0, (real_vector4d*)&rasterizer_globals->fullscreen_parameters, 1))
            {
                global_d3d_device->SetPixelShaderConstantF(0, (real32*)&rasterizer_globals->fullscreen_parameters, 1);
            }

            rasterizer_dx9_set_screen_effect_pixel_shader(3);
            rasterizer_dx9_render_fullscreen_overlay_geometry(0, 0, rasterizer_fullscreen_effects_build_vertex_buffer_cb, 0, 0, 1, 1);
        }
        if (backbuffer)
        {
            backbuffer->Release();
        }
    }
    else
    {
        e_rasterizer_target rasterizer_target_dst = rasterizer_dx9_get_overlay_destination_target();
        if (rasterizer_target_dst != NONE)
        {
            int16 width = rectangle2d_width(&global_window_parameters->camera.viewport_bounds);
            int16 height = rectangle2d_height(&global_window_parameters->camera.viewport_bounds);
            int16 left = global_window_parameters->camera.viewport_bounds.left;
            int16 top = global_window_parameters->camera.viewport_bounds.top;

            const RECT rect =
            {
                left,
                top,
                left + width,
                top + height
            };

            IDirect3DSurface9* dst_surface = rasterizer_dx9_get_render_target_surface(rasterizer_target_dst, 0);
            IDirect3DSurface9* src_surface = rasterizer_dx9_get_render_target_surface(rasterizer_target, 0);
            global_d3d_device->StretchRect(src_surface, &rect, dst_surface, NULL, D3DTEXF_POINT);
        }
    }
    return;
}

bool __cdecl rasterizer_fullscreen_effects_build_vertex_buffer_cb(
    e_vertex_output_type output_type,
    real_rectangle2d* bounds,
    real_vector4d* location,
    void* output,
    void* ctx
)
{
    bool result = true;
    real32 bounds_width;
    real32 bounds_height;
    const s_rasterizer_globals* rasterizer_globals = rasterizer_globals_get();

    ASSERT(output);

    switch (output_type)
    {
    case _vertex_output_type_position:
        // position normalized device coordinates
        // make sure to set the viewport, otherwise it'll represent the entire surface
        // which is not good during split-screen
        rasterizer_dx9_fullscreen_calculate_position(location, 1.f, (real_vector4d*)output);
        break;
    case _vertex_output_type_texcoord:
        ASSERT(bounds);
        ASSERT(location);

        // texcoords normalized device coordinates (origin in 0,0 bottom left, max 1,1 top right)
        // set the tex coords based on the texture set to be drawn on the screen
        bounds_width = bounds->x1 - bounds->x0;
        bounds_height = bounds->y1 - bounds->y0;

        // explanation: locations are per vertex, this draw call will take 4 vertices
        // this function will get called 4 times, for each single vertex
        // the location of the first vertex is at 0, 0. and according to texcoords normalized device coordinates, the origin is in bottom left corner of the texture surface
        // then it goes counter-clockwise until it forms a rectangle, so 0,0 -> 1,0 (right down) -> 1,1 (right top) -> 0,1 (left top) -> (end) 0,0 (left bottom)
        // if the location is 1, for each component we scale it with the bounds width/lenght and offset that with the verticies located on the left
        // if it is 0, then the vertex is either at the left of the screen, or at the bottom, depending on which component is 0
        ((real_vector4d*)output)->i = ((location->i * bounds_width) / rasterizer_globals->resolution_x) + (bounds->x0 / rasterizer_globals->resolution_x);
        ((real_vector4d*)output)->j = ((location->j * bounds_height) / rasterizer_globals->resolution_y) + (bounds->y0 / rasterizer_globals->resolution_y);
        ((real_vector4d*)output)->k = 0.f;
        ((real_vector4d*)output)->w = 1.f;
        break;
    case _vertex_output_type_color:
        *(pixel32*)output = global_white_pixel32;
        break;
    default:
        DISPLAY_ASSERT("unreachable");
        result = false;
    }

    return result;
}

/* private code */

real32 rasterizer_dx9_fullscreen_effect_calculate_position_z(real32 z_far, int32 a2)
{
    const s_rasterizer_globals* rasterizer_globals = rasterizer_globals_get();

    real32 z = (a2 != 2 ? z_far : rasterizer_globals->clipping_parameters.z_far);
    return PIN(z, 0.f, 1.f);
}

void rasterizer_dx9_fullscreen_calculate_position(const real_vector4d* location, real32 z_far, real_vector4d* output)
{
    ASSERT(location);

    real32 z = (z_far != 0.f ? rasterizer_dx9_fullscreen_effect_calculate_position_z(z_far, 1) : 1.f);
    
    output->i = location->i * 2.f - 1.f;    // x
    output->j = -(location->j * 2.f - 1.f); // y
    output->k = z;                          // z
    output->w = 1.f;                        // scale
    return;
}
