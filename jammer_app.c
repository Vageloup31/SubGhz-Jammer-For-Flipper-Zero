#include "jammer_app.h"
#include <gui/gui.h>
#include <furi_hal_subghz.h>

static void draw_callback(Canvas* canvas, void* ctx) {
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 5, 15, "MY Custom Jammer");
    canvas_draw_str(canvas, 5, 35, "Flux Cap: ON");
    canvas_draw_str(canvas, 5, 55, "Full Auto Hop");
}

int32_t custom_jammer_app(void* p) {
    UNUSED(p);

    furi_hal_subghz_reset();
    furi_hal_subghz_set_path(FuriHalSubGhzPathExt); // Flux Capacitor

    ViewPort* viewport = view_port_alloc();
    view_port_draw_callback_set(viewport, draw_callback, NULL);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    // Your full auto-hopping
    while(1) {
        for(uint32_t f = 300000000; f <= 928000000; f += 400000) {
            if((f > 348000000 && f < 387000000) || (f > 464000000 && f < 779000000)) continue;
            furi_hal_subghz_set_frequency(f);
            furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
            furi_delay_ms(20);
            furi_hal_subghz_tx_stop();
            furi_delay_ms(8);
        }
    }

    // Cleanup (never reached)
    furi_hal_subghz_reset();
    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_record_close(RECORD_GUI);
    return 0;
}
