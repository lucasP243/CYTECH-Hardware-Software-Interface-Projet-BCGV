/**
 * \file app.c
 * \brief This file acts as the middleware of the BCGV application, and is its
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
 * \brief List of masks to decode the commands byte of the LNS frame received
 * from the commodos.
 */
typedef enum commodos_decode_masks_t {
  COMMODOS_MASK_WARNINGS = (1 << 7),
  COMMODOS_MASK_SIDELIGHTS = (1 << 6),
  COMMODOS_MASK_HEADLIGHTS = (1 << 5),
  COMMODOS_MASK_REDLIGHTS = (1 << 4),
  COMMODOS_MASK_RIGHT_BLINKER = (1 << 3),
  COMMODOS_MASK_LEFT_BLINKER = (1 << 2),
  COMMODOS_MASK_WIPERS = (1 << 1),
  COMMODOS_MASK_WASHERS = (1),
} commodos_decode_masks_t;

/**
 * \brief Decodes LNS frames from the commodos and sets application data
 * accordingly.
 *
 * \param[in] lns_frame_p The LNS frame.
 * \param[in] lns_frame_size_p The size of the frame.
 */
void decode_commodos(const uint8_t lns_frame_p[DRV_MAX_FRAME_SIZE],
                     size_t lns_frame_size_p);

/**
 * \brief Decodes UDP frames from the MUX and sets application data
 * accordingly.
 *
 * \param[in] data_p The UDP frame.
 */
void decode_mux(const uint8_t data_p[DRV_UDP_10MS_FRAME_SIZE]);

#define BGF_OUT_FRAME_COUNT 5
#define BGF_OUT_FRAME_SIZE 2

/**
 * \brief Creates and encodes the LNS frames for the BGF from application data.
 * \param[out] lns_frame_p Structure to fill with the LNS frames
 */
void encode_bgf(uint_t lns_frame_p[BGF_OUT_FRAME_COUNT][BGF_OUT_FRAME_SIZE]);

/**
 * \brief Creates and encodes the UDP frame for the MUX from application data.
 * \param[out] udp_frame_p Structure to fill with the UDP frame.
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

void decode_commodos(const uint8_t *lns_frame_p, size_t lns_frame_size_p) {

  if (lns_frame_size_p < 2) { // Only frames treated are 2B
    return;
  }
  // Little Endian : CRC8 is on first byte
  uint8_t crc_8_byte = lns_frame_p[0];
  uint8_t command_byte = lns_frame_p[1];

  set_commodos_crc_8(crc_8_byte);
  uint8_t computed_crc_8 = crc_8(&command_byte, 1);

  if (crc_8_byte != computed_crc_8) {
    // TODO print WARN: CRC8 checksum failed
  }

  // Extract commands from command_byte
  set_warnings_in(command_byte & COMMODOS_MASK_WARNINGS);
  set_sidelights_in(command_byte & COMMODOS_MASK_SIDELIGHTS);
  set_headlights_in(command_byte & COMMODOS_MASK_HEADLIGHTS);
  set_redlights_in(command_byte & COMMODOS_MASK_REDLIGHTS);
  set_left_blinker_in(command_byte & COMMODOS_MASK_LEFT_BLINKER);
  set_right_blinker_in(command_byte & COMMODOS_MASK_RIGHT_BLINKER);
  set_wipers_in(command_byte & COMMODOS_MASK_WIPERS);
  set_washer_fluid_in(command_byte & COMMODOS_MASK_WASHERS);
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
  set_frame_mileage(FILTER_FRAME_MILEAGE(data_p));
  set_frame_speed(FILTER_FRAME_SPEED(data_p));
  set_frame_flags(FILTER_FRAME_FLAGS(data_p));
  set_motor_flags(FILTER_MOTOR_FLAGS(data_p));
  set_tank_level(FILTER_TANK_LEVEL(data_p));
  set_motor_speed(FILTER_MOTOR_SPEED(data_p));
  set_battery_flags_in(FILTER_BATTERY_FLAGS(data_p));
}

/**
 * \brief Constants used for encoding the BGF out frame.
 */
typedef enum bgf_encoding_constants_t {
  BGF_FIRST_FRAME_INDEX = 0,
  BGF_SECOND_FRAME_INDEX = 1,
  BGF_THIRD_FRAME_INDEX = 2,
  BGF_FOURTH_FRAME_INDEX = 3,
  BGF_FIFTH_FRAME_INDEX = 4,

  BGF_FIRST_FRAME_ID = 0x01,
  BGF_SECOND_FRAME_ID = 0x02,
  BGF_THIRD_FRAME_ID = 0x03,
  BGF_FOURTH_FRAME_ID = 0x04,
  BGF_FIFTH_FRAME_ID = 0x05,

  BGF_FRAME_ID_INDEX = 0,
  BGF_FRAME_VALUE_INDEX = 1,

  BGF_VALUE_OFF = 0x0,
  BGF_VALUE_ON = 0x1,
} bgf_encoding_constants_t;

void encode_bgf(uint_t lns_frame_p[BGF_OUT_FRAME_COUNT][BGF_OUT_FRAME_SIZE]) {

  lns_frame_p[BGF_FIRST_FRAME_INDEX][BGF_FRAME_ID_INDEX] = BGF_FIRST_FRAME_ID;
  lns_frame_p[BGF_FIRST_FRAME_INDEX][BGF_FRAME_VALUE_INDEX] =
      (get_sidelights_out() ? BGF_VALUE_ON : BGF_VALUE_OFF);

  lns_frame_p[BGF_SECOND_FRAME_INDEX][BGF_FRAME_ID_INDEX] = BGF_SECOND_FRAME_ID;
  lns_frame_p[BGF_SECOND_FRAME_INDEX][BGF_FRAME_VALUE_INDEX] =
      (get_headlights_out() ? BGF_VALUE_ON : BGF_VALUE_OFF);

  lns_frame_p[BGF_THIRD_FRAME_INDEX][BGF_FRAME_ID_INDEX] = BGF_THIRD_FRAME_ID;
  lns_frame_p[BGF_THIRD_FRAME_INDEX][BGF_FRAME_VALUE_INDEX] =
      (get_headlights_out() ? BGF_VALUE_ON : BGF_VALUE_OFF);

  lns_frame_p[BGF_FOURTH_FRAME_INDEX][BGF_FRAME_ID_INDEX] = BGF_FOURTH_FRAME_ID;
  lns_frame_p[BGF_FOURTH_FRAME_INDEX][BGF_FRAME_VALUE_INDEX] =
      (get_headlights_out() ? BGF_VALUE_ON : BGF_VALUE_OFF);

  lns_frame_p[BGF_FIFTH_FRAME_INDEX][BGF_FRAME_ID_INDEX] = BGF_FIFTH_FRAME_ID;
  lns_frame_p[BGF_FIFTH_FRAME_INDEX][BGF_FRAME_VALUE_INDEX] =
      (get_headlights_out() ? BGF_VALUE_ON : BGF_VALUE_OFF);
}

/**
 * \brief Constants used for encoding the MUX out frame.
 */
typedef enum mux_encoding_constants_t {
  MUX_OUT_FRAME_FIRST_BYTE = 0,
  MUX_OUT_FRAME_SECOND_BYTE = 1,
  MUX_OUT_FRAME_THIRD_BYTE = 2,
  MUX_OUT_FRAME_FOURTH_BYTE = 3,
  MUX_OUT_FRAME_FIFTH_BYTE = 4,
  MUX_OUT_FRAME_SIXTH_BYTE = 5,
  MUX_OUT_FRAME_SEVENTH_BYTE = 6,
  MUX_OUT_FRAME_EIGHTH_BYTE = 7,
  MUX_OUT_FRAME_NINTH_BYTE = 8,

  MUX_OUT_OFFSET_SIDELIGHTS = 7,
  MUX_OUT_OFFSET_HEADLIGHTS = 6,
  MUX_OUT_OFFSET_REDLIGHTS = 5,
  MUX_OUT_OFFSET_LOW_FUEL = 4,
  MUX_OUT_OFFSET_MOTOR_FAILURE = 3,
  MUX_OUT_OFFSET_TIRE_PRESSURE = 2,
  MUX_OUT_OFFSET_PADS_FAILURE = 1,
  MUX_OUT_OFFSET_BATTERY_LOW = 0,

  MUX_OUT_OFFSET_WARNINGS = 7,
  MUX_OUT_OFFSET_BATTERY_FAILURE = 6,
  MUX_OUT_OFFSET_COOLANT_OVERHEAT = 5,
  MUX_OUT_OFFSET_MOTOR_PRESSURE = 4,
  MUX_OUT_OFFSET_OIL_OVERHEAT = 3,
  MUX_OUT_OFFSET_BRAKE_FAILURE = 2,
  MUX_OUT_OFFSET_WIPERS_OUT = 1,
  MUX_OUT_OFFSET_WASHER_FLUID_OUT = 0,
} mux_encoding_constants_t;

// Macros for encoding the mileage bytes (big endian)
#define ENCODE_MILEAGE_FIRST_BYTE(v_int32) (0xFFFF & (v_int32 >> 24))
#define ENCODE_MILEAGE_SECOND_BYTE(v_int32) (0xFFFF & (v_int32 >> 16))
#define ENCODE_MILEAGE_THIRD_BYTE(v_int32) (0xFFFF & (v_int32 >> 8))
#define ENCODE_MILEAGE_FOURTH_BYTE(v_int32) (0xFFFF & v_int32)

void encode_mux(uint8_t udp_frame_p[DRV_UDP_20MS_FRAME_SIZE]) {

  udp_frame_p[MUX_OUT_FRAME_FIRST_BYTE] =
      (get_indicator_sidelights() << MUX_OUT_OFFSET_SIDELIGHTS) +
      (get_indicator_headlights() << MUX_OUT_OFFSET_HEADLIGHTS) +
      (get_indicator_redlights() << MUX_OUT_OFFSET_REDLIGHTS) +
      (get_indicator_low_fuel() << MUX_OUT_OFFSET_LOW_FUEL) +
      (get_indicator_motor_failure() << MUX_OUT_OFFSET_MOTOR_FAILURE) +
      (get_indicator_tire_pressure() << MUX_OUT_OFFSET_TIRE_PRESSURE) +
      (get_indicator_pads_failure() << MUX_OUT_OFFSET_PADS_FAILURE) +
      (get_indicator_battery_low() << MUX_OUT_OFFSET_BATTERY_LOW);

  udp_frame_p[MUX_OUT_FRAME_SECOND_BYTE] =
      (get_indicator_warnings() << MUX_OUT_OFFSET_WARNINGS) +
      (get_indicator_battery_failure() << MUX_OUT_OFFSET_BATTERY_FAILURE) +
      (get_indicator_coolant_overheat() << MUX_OUT_OFFSET_COOLANT_OVERHEAT) +
      (get_indicator_motor_pressure() << MUX_OUT_OFFSET_MOTOR_PRESSURE) +
      (get_indicator_oil_overheat() << MUX_OUT_OFFSET_OIL_OVERHEAT) +
      (get_indicator_brake_failure() << MUX_OUT_OFFSET_BRAKE_FAILURE) +
      (get_wipers_out() << MUX_OUT_OFFSET_WIPERS_OUT) +
      (get_washer_fluid_out() << MUX_OUT_OFFSET_WASHER_FLUID_OUT);

  frame_mileage_t mileage = get_frame_mileage();

  udp_frame_p[MUX_OUT_FRAME_THIRD_BYTE] = ENCODE_MILEAGE_FIRST_BYTE(mileage);
  udp_frame_p[MUX_OUT_FRAME_FOURTH_BYTE] = ENCODE_MILEAGE_SECOND_BYTE(mileage);
  udp_frame_p[MUX_OUT_FRAME_FIFTH_BYTE] = ENCODE_MILEAGE_THIRD_BYTE(mileage);
  udp_frame_p[MUX_OUT_FRAME_SIXTH_BYTE] = ENCODE_MILEAGE_FOURTH_BYTE(mileage);

  udp_frame_p[MUX_OUT_FRAME_SEVENTH_BYTE] = get_frame_speed();
  udp_frame_p[MUX_OUT_FRAME_EIGHTH_BYTE] = get_tank_level();
  udp_frame_p[MUX_OUT_FRAME_NINTH_BYTE] = get_motor_speed();
}
