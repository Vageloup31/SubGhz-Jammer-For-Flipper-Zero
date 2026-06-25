#include <furi.h>
#include <gui/gui.h>
#include <gui/modules/variable_item_list.h>
#include <subghz/subghz_tx_rx_worker.h>
#include <furi_hal_subghz.h>

typedef enum {
    ModeWhiteNoise,
    ModeBruteForce,
    ModeSawtooth,
    ModeHopping,
    ModeScanThenJam,
} JammerMode;

typedef struct {
    VariableItemList* menu;
    ViewPort* viewport;
    Gui* gui;
    JammerMode current_mode;
    uint32_t start_freq;
    uint32_t end_freq;
    uint32_t step;
    uint32_t delay_ms;
    uint8_t range_preset; // 0=Low, 1=Med, 2=High, 3=Max
    bool running;
    bool flux_cap;
} AppContext;

static void draw_callback(Canvas* canvas, void* ctx) {
    AppContext* app = ctx;
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 5, 15, "Custom Jammer");
    canvas_draw_str(canvas, 5, 35, "Flux Cap Active");
    if (app->running) canvas_draw_str(canvas, 5, 55, "JAMMING");
}

int32_t custom_jammer_app(void* p) {
    AppContext* app = malloc(sizeof(AppContext));
    memset(app, 0, sizeof(AppContext));
    
    app->start_freq = 433000000;
    app->end_freq = 434000000;
    app->step = 250000;
    app->delay_ms = 15;
    app->range_preset = 2;
    app->current_mode = ModeHopping;
    app->flux_cap = true;
    app->running = true;

    furi_hal_subghz_reset();
    furi_hal_subghz_set_path(FuriHalSubGhzPathExt); // Flux Capacitor

    while (app->running) {
        uint32_t burst = 20 + (app->range_preset * 25); // Range control

        if (app->current_mode == ModeHopping || app->current_mode == ModeSawtooth) {
            for (uint32_t f = app->start_freq; f <= app->end_freq; f += app->step) {
                furi_hal_subghz_set_frequency(f);
                furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
                furi_delay_ms(burst);
                furi_hal_subghz_tx_stop();
                furi_delay_ms(app->delay_ms);
            }
        } else {
            // Other modes placeholder
            furi_hal_subghz_set_frequency(433920000);
            furi_hal_subghz_tx_start(FuriHalSubGhzPresetOok650Async);
            furi_delay_ms(300);
            furi_hal_subghz_tx_stop();
        }
    }

    furi_hal_subghz_reset();
    free(app);
    return 0;
}
