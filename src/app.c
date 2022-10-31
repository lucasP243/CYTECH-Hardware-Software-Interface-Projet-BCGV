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

#include "lib/checksum.h"
#include "lib/data_dictionary.h"
#include "lib/drv_api.h"

/**
 * \brief List of masks to decode the commands byte of the COMODO frame.
 */
typedef enum comodo_decode_masks_t {
  COMODO_MASK_WARNINGS = (1 << 7),
  COMODO_MASK_SIDELIGHTS = (1 << 6),
  COMODO_MASK_HEADLIGHTS = (1 << 5),
  COMODO_MASK_REDLIGHTS = (1 << 4),
  COMODO_MASK_RIGHT_BLINKER = (1 << 3),
  COMODO_MASK_LEFT_BLINKER = (1 << 2),
  COMODO_MASK_WIPERS = (1 << 1),
  COMODO_MASK_WASHERS = (1),
} comodo_decode_masks_t;

/**
 * \brief Decodes LNS frames from the COMODO and sets application data
 * accordingly.
 *
 * @param lns_frame_p The LNS frame.
 * @param lns_frame_size_p The size of the frame.
 */
void decode_comodo(const uint8_t lns_frame_p[DRV_MAX_FRAME_SIZE],
                   size_t lns_frame_size_p);

/**
 * \brief Decodes UDP frames from the MUX and sets application data
 * accordingly.
 *
 * @param data_p The UDP frame.
 */
void decode_mux(const uint8_t data_p[DRV_UDP_10MS_FRAME_SIZE]);

// TODO: encode_bgf[1-5]

/**
 * \brief Encodes the UDP frame for the MUX from application data.
 * @param[out] udp_frame_p Structure to fill with the UDP frame.
 */
void encode_mux(uint8_t udp_frame_p[DRV_UDP_20MS_FRAME_SIZE]);

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

void decode_comodo(const uint8_t *lns_frame_p, size_t lns_frame_size_p) {

  if (lns_frame_size_p < 2) { // Only frames treated are 2B
    return;
  }
  // Little Endian : CRC8 is on first byte
  uint8_t crc_8_byte = lns_frame_p[0];
  uint8_t command_byte = lns_frame_p[1];

  set_comodo_crc_8(crc_8_byte);
  uint8_t computed_crc_8 = crc_8(&command_byte, 1);

  if (crc_8_byte != computed_crc_8) {
    // TODO print WARN: CRC8 checksum failed
  }

  // Extract commands from command_byte
  set_warnings_in(command_byte & COMODO_MASK_WARNINGS);
  set_sidelights_in(command_byte & COMODO_MASK_SIDELIGHTS);
  set_headlights_in(command_byte & COMODO_MASK_HEADLIGHTS);
  set_redlights_in(command_byte & COMODO_MASK_REDLIGHTS);
  set_left_blinker_in(command_byte & COMODO_MASK_LEFT_BLINKER);
  set_right_blinker_in(command_byte & COMODO_MASK_RIGHT_BLINKER);
  set_wipers_in(command_byte & COMODO_MASK_WIPERS);
  set_washer_fluid_in(command_byte & COMODO_MASK_WASHERS);
}

// MUX decoding filters
#define FILTER_MUX_FRAME_ID(data) data[0]
#define FILTER_FRAME_MILEAGE(data)                                             \
  ((data_p[1] << 24) + (data_p[2] << 16) + (data_p[3] << 8) + data_p[4])
#define FILTER_FRAME_SPEED(data) data[5]
#define FILTER_FRAME_FLAGS(data) data[6]
#define FILTER_MOTOR_FLAGS(data) data[7]
#define FILTER_TANK_LEVEL(data) data[8]
#define FILTER_MOTOR_SPEED(data)                                               \
  ((data_p[9] << 24) + (data_p[10] << 16) + (data_p[11] << 8) + data_p[12])
#define FILTER_BATTERY_FLAGS(data) data[13]

void decode_mux(const uint8_t data_p[DRV_UDP_10MS_FRAME_SIZE]) {
  set_mux_frame_id(FILTER_MUX_FRAME_ID(data_p));
  set_frame_mileage_in(FILTER_FRAME_MILEAGE(data_p));
  set_frame_speed_in(FILTER_FRAME_SPEED(data_p));
  set_frame_flags_in(FILTER_FRAME_FLAGS(data_p));
  set_motor_flags_in(FILTER_MOTOR_FLAGS(data_p));
  set_tank_level_in(FILTER_TANK_LEVEL(data_p));
  set_motor_speed_in(FILTER_MOTOR_SPEED(data_p));
  set_battery_flags_in(FILTER_BATTERY_FLAGS(data_p));
}

void encode_mux(uint8_t udp_frame_p[DRV_UDP_20MS_FRAME_SIZE]) {

  udp_frame_p[0] =
      (get_indicator_sidelights() << 7) + (get_indicator_headlights() << 6) +
      (get_indicator_redlights() << 5) + (get_indicator_low_fuel() << 4) +
      (get_indicator_motor_failure() << 3) +
      (get_indicator_tire_pressure() << 2) +
      (get_indicator_brake_pads_failure() << 1) +
      (get_indicator_battery_low() << 0);

  udp_frame_p[1] = (get_indicator_warnings() << 7) +
                   (get_indicator_battery_failure() << 6) +
                   (get_indicator_cooling_liquid_overheat() << 5) +
                   (get_indicator_motor_pressure() << 4) +
                   (get_indicator_oil_overheat() << 3) +
                   (get_indicator_brake_failure() << 2) +
                   (get_wipers_out() << 1) + (get_washer_fluid_out() << 0);

  frame_mileage_t mileage = get_frame_mileage_out();

  udp_frame_p[2] = (0xFFFF & (mileage >> 24));
  udp_frame_p[3] = (0xFFFF & (mileage >> 16));
  udp_frame_p[4] = (0xFFFF & (mileage >> 8));
  udp_frame_p[5] = (0xFFFF & mileage);

  udp_frame_p[6] = get_frame_speed_out();
  udp_frame_p[7] = get_tank_level_out();
  udp_frame_p[8] = get_motor_speed_out();

  // TODO: fuse application variables in/out for other than commands
  // TODO: use defines to secure magic numbers
}
