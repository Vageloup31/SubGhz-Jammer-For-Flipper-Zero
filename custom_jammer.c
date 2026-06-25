#include <furi.h>
#include <gui/gui.h>
#include <furi_hal_subghz.h>

static void draw_callback(Canvas* canvas, void* ctx) {
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 5, 20, "Custom Jammer");
    canvas_draw_str(canvas, 5, 40, "Flux Cap ON");
    canvas_draw_str(canvas, 5, 60, "Full Auto Hop");
}

int32_t custom_jammer_app(void* p) {
    UNUSED(p);

    furi_hal_subghz_reset();
    furi_hal_subghz_set_path(FuriHalSubGhzPathExt);

    ViewPort* viewport = view_port_alloc();
    view_port_draw_callback_set(viewport, draw_callback, NULL);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    while(1) {
        // Band 1
        for(uint32_t f = 300000000; f <= 348000000; f += 500000) {
            furi_hal_subghz_set_frequency(f);
            furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
            furi_delay_ms(25);
            furi_hal_subghz_tx_stop();
            furi_delay_ms(10);
        }
        // Band 2
        for(uint32_t f = 387000000; f <= 464000000; f += 400000) {
            furi_hal_subghz_set_frequency(f);
            furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
            furi_delay_ms(25);
            furi_hal_subghz_tx_stop();
            furi_delay_ms(10);
        }
        // Band 3
        for(uint32_t f = 779000000; f <= 928000000; f += 500000) {
            furi_hal_subghz_set_frequency(f);
            furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
            furi_delay_ms(25);
            furi_hal_subghz_tx_stop();
            furi_delay_ms(10);
        }
    }

    furi_hal_subghz_reset();
    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_record_close(RECORD_GUI);
    return 0;
}
