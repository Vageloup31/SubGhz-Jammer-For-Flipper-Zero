#include <furi.h>
#include <gui/gui.h>
#include <subghz/subghz_tx_rx_worker.h>
#include <furi_hal_subghz.h>

typedef enum {
    ModeHopping,
    ModeSawtooth,
    ModeWhiteNoise,
    ModeBruteForce,
} JammerMode;

typedef struct {
    JammerMode mode;
    uint32_t start_freq;
    uint32_t end_freq;
    uint32_t step;
    uint32_t delay_ms;
    uint8_t range_preset; // 0=Low ~10m, 1=Med ~30m, 2=High ~70m, 3=Max ~150m+
    bool running;
} AppContext;

static void draw_callback(Canvas* canvas, void* ctx) {
    AppContext* app = ctx;
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 5, 15, "Custom SubGHz Jammer");
    canvas_draw_str(canvas, 5, 35, "Flux Cap: ON");
    if (app->running) {
        canvas_draw_str(canvas, 5, 55, "JAMMING ACTIVE");
    } else {
        canvas_draw_str(canvas, 5, 55, "Press OK to Start");
    }
}

int32_t custom_jammer_app(void* p) {
    AppContext app = {0};
    app.start_freq = 433000000;
    app.end_freq = 434000000;
    app.step = 250000;
    app.delay_ms = 15;
    app.range_preset = 2;
    app.mode = ModeHopping;
    app.running = true;

    furi_hal_subghz_reset();
    furi_hal_subghz_set_path(FuriHalSubGhzPathExt); // Flux Capacitor

    ViewPort* viewport = view_port_alloc();
    view_port_draw_callback_set(viewport, draw_callback, &app);
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    while (app.running) {
        uint32_t burst_time = 20 + (app.range_preset * 25); // Range control

        // Full band hopping / sweeping
        if (app.mode == ModeHopping || app.mode == ModeSawtooth) {
            // Band 1
            for (uint32_t f = 300000000; f <= 348000000; f += app.step) {
                furi_hal_subghz_set_frequency(f);
                furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
                furi_delay_ms(burst_time);
                furi_hal_subghz_tx_stop();
                furi_delay_ms(app.delay_ms);
            }
            // Band 2 (most active)
            for (uint32_t f = 387000000; f <= 464000000; f += app.step) {
                furi_hal_subghz_set_frequency(f);
                furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
                furi_delay_ms(burst_time);
                furi_hal_subghz_tx_stop();
                furi_delay_ms(app.delay_ms);
            }
            // Band 3
            for (uint32_t f = 779000000; f <= 928000000; f += app.step) {
                furi_hal_subghz_set_frequency(f);
                furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
                furi_delay_ms(burst_time);
                furi_hal_subghz_tx_stop();
                furi_delay_ms(app.delay_ms);
            }
        } else {
            // White Noise / Brute Force fallback
            furi_hal_subghz_set_frequency(433920000);
            furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
            furi_delay_ms(400);
            furi_hal_subghz_tx_stop();
        }
    }

    furi_hal_subghz_reset();
    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_record_close(RECORD_GUI);

    return 0;
}
