/**
 * TODO doxygen
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "lib/drv_api.h"

__attribute__((noreturn))
int main(void) {

    int32_t driver_fd = DRV_ERROR;
    uint8_t out_udp_frame[DRV_UDP_10MS_FRAME_SIZE];

    driver_fd = drv_open();

    if (driver_fd == DRV_ERROR) {
        perror("Unable to open connection to driver");
    }

    while (1) {

        drv_read_udp_10ms(driver_fd, out_udp_frame);

#pragma unroll
        for (size_t i = 0; i < DRV_UDP_10MS_FRAME_SIZE; i++) {
            (void) fprintf(stdout, "%08" PRIX32, out_udp_frame[i]);
        }
        (void) fputs("\n", stdout);
    }

    // Generates warning that function declared 'noreturn' has a 'return' statement somehow??
    // Who cares, it's temporary code and the endless loop will be removed soon enough
}