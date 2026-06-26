// Minimal helper for your custom jammer
#include <furi.h>
#include <furi_hal_subghz.h>

void jammer_init_subghz(void) {
    furi_hal_subghz_reset();
    furi_hal_subghz_set_path(FuriHalSubGhzPathExt); // Flux Cap
}
