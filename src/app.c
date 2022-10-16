/**
 * \file app.c
 * \brief This file acts as the middleware of the application, and is its
 * entrypoint.
 *
 * \details Up to the current revision, this program only reads UDP frames from
 * the driver and writes those raw (in hexadecimal) to the standard output.
 *
 * \author Pinard Lucas
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "lib/drv_api.h"

__attribute__((noreturn)) int main(void) {

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
      (void)fprintf(stdout, "%08" PRIX32, out_udp_frame[i]);
    }
    (void)fputs("\n", stdout);
  }
}