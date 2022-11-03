/*
 * This file was automatically generated using csnake v0.3.5.
 *
 * This file should not be edited directly, any changes will be
 * overwritten next time the script is run.
 *
 * Source code for csnake is available at:
 * https://gitlab.com/andrejr/csnake
 *
 * csnake is also available on PyPI, at :
 * https://pypi.org/project/csnake
 */
#ifndef DATA_DICTIONARY_H
#define DATA_DICTIONARY_H
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Command received on input, can be either ON (true) or OFF (false).
 */
typedef bool command_in_t;

/**
 * \brief Command sent on output, can be either ON (true) or OFF (false).
 */
typedef bool command_out_t;

/**
 * \brief Status of an indicator, can be either ON (true) or OFF (false).
 */
typedef bool indicator_t;

/**
 * \brief 8-bit CRC code.
 */
typedef uint8_t crc_8_t;

/**
 * \brief ID of a MUX frame.
 */
typedef uint8_t mux_id_t;

/**
 * \brief Mileage of the vehicle, in kilometers.
 */
typedef uint32_t frame_mileage_t;

/**
 * \brief Current speed of the vehicle, in kilometers per hour.
 */
typedef uint8_t frame_speed_t;

/**
 * \brief Flags for the frame defects : tire pressure on bit 0b01, brakes
 * failure on bit 0b10.
 */
typedef uint8_t frame_flags_t;

/**
 * \brief Flags for the motor defects : motor pressure on bit 0b001, cooling
 * liquid overheat on bit 0b010, oil overheat on bit 0b100.
 */
typedef uint8_t motor_flags_t;

/**
 * \brief Amount of remaining fuel in the tank.
 */
typedef uint8_t tank_level_t;

/**
 * \brief Current motor speed, in rotations per minute.
 */
typedef uint32_t motor_speed_t;

/**
 * \brief Flags for the battery defects: battery low on bit 0b01, battery
 * failure on bit 0b10.
 */
typedef uint8_t battery_flags_t;

/**
 * \brief Finite state machine for the lights systems.
 */
typedef int32_t fsm_lights_t;

/**
 * \brief Finite state machine for the blinkers systems.
 */
typedef int32_t fsm_blinkers_t;

/**
 * \brief Finite state machine for the wipers system.
 */
typedef int32_t fsm_wipers_t;

/**
 * \brief Timer for the finite state machines.
 */
typedef uint8_t fsm_timer_t;

/**
 * \brief Acknowledgement of a message sent.
 */
typedef bool acknowledgement_t;

/**
 * \brief Initializer function for the application data.
 */
void application_init(void);

/**
 * \brief Getter for application.warnings_in.
 * \return Input buffer for the warnings command.
 */
command_in_t get_warnings_in(void);

/**
 * \brief Setter for application.warnings_in.
 * \param[in] value Input buffer for the warnings command.
 */
void set_warnings_in(command_in_t value);

/**
 * \brief Getter for application.sidelights_in.
 * \return Input buffer for the sidelights command.
 */
command_in_t get_sidelights_in(void);

/**
 * \brief Setter for application.sidelights_in.
 * \param[in] value Input buffer for the sidelights command.
 */
void set_sidelights_in(command_in_t value);

/**
 * \brief Getter for application.sidelights_out.
 * \return Output buffer for the sidelights command.
 */
command_out_t get_sidelights_out(void);

/**
 * \brief Setter for application.sidelights_out.
 * \param[in] value Output buffer for the sidelights command.
 */
void set_sidelights_out(command_out_t value);

/**
 * \brief Getter for application.sidelights_acknowledgement.
 * \return Acknowledgement for the sidelights command message.
 */
acknowledgement_t get_sidelights_acknowledgement(void);

/**
 * \brief Setter for application.sidelights_acknowledgement.
 * \param[in] value Acknowledgement for the sidelights command message.
 */
void set_sidelights_acknowledgement(acknowledgement_t value);

/**
 * \brief Getter for application.headlights_in.
 * \return Input buffer for the headlights command.
 */
command_in_t get_headlights_in(void);

/**
 * \brief Setter for application.headlights_in.
 * \param[in] value Input buffer for the headlights command.
 */
void set_headlights_in(command_in_t value);

/**
 * \brief Getter for application.headlights_out.
 * \return Output buffer for the headlight command.
 */
command_out_t get_headlights_out(void);

/**
 * \brief Setter for application.headlights_out.
 * \param[in] value Output buffer for the headlight command.
 */
void set_headlights_out(command_out_t value);

/**
 * \brief Getter for application.headlights_acknowledgement.
 * \return Acknowledgement for the headlights command message.
 */
acknowledgement_t get_headlights_acknowledgement(void);

/**
 * \brief Setter for application.headlights_acknowledgement.
 * \param[in] value Acknowledgement for the headlights command message.
 */
void set_headlights_acknowledgement(acknowledgement_t value);

/**
 * \brief Getter for application.redlights_in.
 * \return Input buffer for the redlights command.
 */
command_in_t get_redlights_in(void);

/**
 * \brief Setter for application.redlights_in.
 * \param[in] value Input buffer for the redlights command.
 */
void set_redlights_in(command_in_t value);

/**
 * \brief Getter for application.redlights_out.
 * \return Output buffer for the redlights command.
 */
command_out_t get_redlights_out(void);

/**
 * \brief Setter for application.redlights_out.
 * \param[in] value Output buffer for the redlights command.
 */
void set_redlights_out(command_out_t value);

/**
 * \brief Getter for application.redlights_acknowledgement.
 * \return Acknowledgement for the redlights command message.
 */
acknowledgement_t get_redlights_acknowledgement(void);

/**
 * \brief Setter for application.redlights_acknowledgement.
 * \param[in] value Acknowledgement for the redlights command message.
 */
void set_redlights_acknowledgement(acknowledgement_t value);

/**
 * \brief Getter for application.left_blinker_in.
 * \return Input buffer for the left blinker command.
 */
command_in_t get_left_blinker_in(void);

/**
 * \brief Setter for application.left_blinker_in.
 * \param[in] value Input buffer for the left blinker command.
 */
void set_left_blinker_in(command_in_t value);

/**
 * \brief Getter for application.left_blinker_out.
 * \return Output buffer for the left blinker command.
 */
command_out_t get_left_blinker_out(void);

/**
 * \brief Setter for application.left_blinker_out.
 * \param[in] value Output buffer for the left blinker command.
 */
void set_left_blinker_out(command_out_t value);

/**
 * \brief Getter for application.left_blinker_acknowledgement.
 * \return Acknowledgement for the left blinker command message.
 */
acknowledgement_t get_left_blinker_acknowledgement(void);

/**
 * \brief Setter for application.left_blinker_acknowledgement.
 * \param[in] value Acknowledgement for the left blinker command message.
 */
void set_left_blinker_acknowledgement(acknowledgement_t value);

/**
 * \brief Getter for application.right_blinker_in.
 * \return Input buffer for the right blinker command.
 */
command_in_t get_right_blinker_in(void);

/**
 * \brief Setter for application.right_blinker_in.
 * \param[in] value Input buffer for the right blinker command.
 */
void set_right_blinker_in(command_in_t value);

/**
 * \brief Getter for application.right_blinker_out.
 * \return Output buffer for the right blinker command.
 */
command_out_t get_right_blinker_out(void);

/**
 * \brief Setter for application.right_blinker_out.
 * \param[in] value Output buffer for the right blinker command.
 */
void set_right_blinker_out(command_out_t value);

/**
 * \brief Getter for application.right_blinker_acknowledgement.
 * \return Acknowledgement for the right blinker command message.
 */
acknowledgement_t get_right_blinker_acknowledgement(void);

/**
 * \brief Setter for application.right_blinker_acknowledgement.
 * \param[in] value Acknowledgement for the right blinker command message.
 */
void set_right_blinker_acknowledgement(acknowledgement_t value);

/**
 * \brief Getter for application.wipers_in.
 * \return Input buffer for the wipers command.
 */
command_in_t get_wipers_in(void);

/**
 * \brief Setter for application.wipers_in.
 * \param[in] value Input buffer for the wipers command.
 */
void set_wipers_in(command_in_t value);

/**
 * \brief Getter for application.wipers_out.
 * \return Output buffer for the wipers command.
 */
command_out_t get_wipers_out(void);

/**
 * \brief Setter for application.wipers_out.
 * \param[in] value Output buffer for the wipers command.
 */
void set_wipers_out(command_out_t value);

/**
 * \brief Getter for application.washer_fluid_in.
 * \return Input buffer for the washer fluid command.
 */
command_in_t get_washer_fluid_in(void);

/**
 * \brief Setter for application.washer_fluid_in.
 * \param[in] value Input buffer for the washer fluid command.
 */
void set_washer_fluid_in(command_in_t value);

/**
 * \brief Getter for application.washer_fluid_out.
 * \return Output buffer for the washer fluid command.
 */
command_out_t get_washer_fluid_out(void);

/**
 * \brief Setter for application.washer_fluid_out.
 * \param[in] value Output buffer for the washer fluid command.
 */
void set_washer_fluid_out(command_out_t value);

/**
 * \brief Getter for application.commodos_crc_8.
 * \return Input buffer for the commodos' sent CRC8 code.
 */
crc_8_t get_commodos_crc_8(void);

/**
 * \brief Setter for application.commodos_crc_8.
 * \param[in] value Input buffer for the commodos' sent CRC8 code.
 */
void set_commodos_crc_8(crc_8_t value);

/**
 * \brief Getter for application.mux_frame_id.
 * \return Input buffer for the MUX frame ID.
 */
mux_id_t get_mux_frame_id(void);

/**
 * \brief Setter for application.mux_frame_id.
 * \param[in] value Input buffer for the MUX frame ID.
 */
void set_mux_frame_id(mux_id_t value);

/**
 * \brief Getter for application.frame_mileage.
 * \return Input buffer for the vehicle’s mileage.
 */
frame_mileage_t get_frame_mileage(void);

/**
 * \brief Setter for application.frame_mileage.
 * \param[in] value Input buffer for the vehicle’s mileage.
 */
void set_frame_mileage(frame_mileage_t value);

/**
 * \brief Getter for application.frame_speed.
 * \return Input buffer for the vehicle’s speed.
 */
frame_speed_t get_frame_speed(void);

/**
 * \brief Setter for application.frame_speed.
 * \param[in] value Input buffer for the vehicle’s speed.
 */
void set_frame_speed(frame_speed_t value);

/**
 * \brief Getter for application.frame_flags.
 * \return Input buffer for the vehicle’s frame status information.
 */
frame_flags_t get_frame_flags(void);

/**
 * \brief Setter for application.frame_flags.
 * \param[in] value Input buffer for the vehicle’s frame status information.
 */
void set_frame_flags(frame_flags_t value);

/**
 * \brief Getter for application.motor_flags.
 * \return Input buffer for the vehicle’s motor status information.
 */
motor_flags_t get_motor_flags(void);

/**
 * \brief Setter for application.motor_flags.
 * \param[in] value Input buffer for the vehicle’s motor status information.
 */
void set_motor_flags(motor_flags_t value);

/**
 * \brief Getter for application.tank_level.
 * \return Input buffer for the vehicle’s tank fuel level.
 */
tank_level_t get_tank_level(void);

/**
 * \brief Setter for application.tank_level.
 * \param[in] value Input buffer for the vehicle’s tank fuel level.
 */
void set_tank_level(tank_level_t value);

/**
 * \brief Getter for application.motor_speed.
 * \return Input buffer for the vehicle’s motor speed.
 */
motor_speed_t get_motor_speed(void);

/**
 * \brief Setter for application.motor_speed.
 * \param[in] value Input buffer for the vehicle’s motor speed.
 */
void set_motor_speed(motor_speed_t value);

/**
 * \brief Getter for application.battery_flags_in.
 * \return Input buffer for the vehicle’s battery status information.
 */
battery_flags_t get_battery_flags_in(void);

/**
 * \brief Setter for application.battery_flags_in.
 * \param[in] value Input buffer for the vehicle’s battery status information.
 */
void set_battery_flags_in(battery_flags_t value);

/**
 * \brief Getter for application.indicator_sidelights.
 * \return Output buffer for the sidelights indicator.
 */
indicator_t get_indicator_sidelights(void);

/**
 * \brief Setter for application.indicator_sidelights.
 * \param[in] value Output buffer for the sidelights indicator.
 */
void set_indicator_sidelights(indicator_t value);

/**
 * \brief Getter for application.indicator_headlights.
 * \return Output buffer for the headlight indicator.
 */
indicator_t get_indicator_headlights(void);

/**
 * \brief Setter for application.indicator_headlights.
 * \param[in] value Output buffer for the headlight indicator.
 */
void set_indicator_headlights(indicator_t value);

/**
 * \brief Getter for application.indicator_redlights.
 * \return Output buffer for the redlights indicator.
 */
indicator_t get_indicator_redlights(void);

/**
 * \brief Setter for application.indicator_redlights.
 * \param[in] value Output buffer for the redlights indicator.
 */
void set_indicator_redlights(indicator_t value);

/**
 * \brief Getter for application.indicator_low_fuel.
 * \return Output buffer for the low fuel indicator.
 */
indicator_t get_indicator_low_fuel(void);

/**
 * \brief Setter for application.indicator_low_fuel.
 * \param[in] value Output buffer for the low fuel indicator.
 */
void set_indicator_low_fuel(indicator_t value);

/**
 * \brief Getter for application.indicator_motor_failure.
 * \return Output buffer for the motor failure indicator.
 */
indicator_t get_indicator_motor_failure(void);

/**
 * \brief Setter for application.indicator_motor_failure.
 * \param[in] value Output buffer for the motor failure indicator.
 */
void set_indicator_motor_failure(indicator_t value);

/**
 * \brief Getter for application.indicator_tire_pressure.
 * \return Output buffer for the low tire pressure indicator.
 */
indicator_t get_indicator_tire_pressure(void);

/**
 * \brief Setter for application.indicator_tire_pressure.
 * \param[in] value Output buffer for the low tire pressure indicator.
 */
void set_indicator_tire_pressure(indicator_t value);

/**
 * \brief Getter for application.indicator_pads_failure.
 * \return Output buffer for the brake pads failure indicator.
 */
indicator_t get_indicator_pads_failure(void);

/**
 * \brief Setter for application.indicator_pads_failure.
 * \param[in] value Output buffer for the brake pads failure indicator.
 */
void set_indicator_pads_failure(indicator_t value);

/**
 * \brief Getter for application.indicator_battery_low.
 * \return Output buffer for the battery low indicator.
 */
indicator_t get_indicator_battery_low(void);

/**
 * \brief Setter for application.indicator_battery_low.
 * \param[in] value Output buffer for the battery low indicator.
 */
void set_indicator_battery_low(indicator_t value);

/**
 * \brief Getter for application.indicator_warnings.
 * \return Output buffer for the warnings indicator.
 */
indicator_t get_indicator_warnings(void);

/**
 * \brief Setter for application.indicator_warnings.
 * \param[in] value Output buffer for the warnings indicator.
 */
void set_indicator_warnings(indicator_t value);

/**
 * \brief Getter for application.indicator_battery_failure.
 * \return Output buffer for the battery failure indicator.
 */
indicator_t get_indicator_battery_failure(void);

/**
 * \brief Setter for application.indicator_battery_failure.
 * \param[in] value Output buffer for the battery failure indicator.
 */
void set_indicator_battery_failure(indicator_t value);

/**
 * \brief Getter for application.indicator_coolant_overheat.
 * \return Output buffer for the coolant overheat indicator.
 */
indicator_t get_indicator_coolant_overheat(void);

/**
 * \brief Setter for application.indicator_coolant_overheat.
 * \param[in] value Output buffer for the coolant overheat indicator.
 */
void set_indicator_coolant_overheat(indicator_t value);

/**
 * \brief Getter for application.indicator_motor_pressure.
 * \return Output buffer for the motor pressure indicator.
 */
indicator_t get_indicator_motor_pressure(void);

/**
 * \brief Setter for application.indicator_motor_pressure.
 * \param[in] value Output buffer for the motor pressure indicator.
 */
void set_indicator_motor_pressure(indicator_t value);

/**
 * \brief Getter for application.indicator_oil_overheat.
 * \return Output buffer for the oil overhead indicator.
 */
indicator_t get_indicator_oil_overheat(void);

/**
 * \brief Setter for application.indicator_oil_overheat.
 * \param[in] value Output buffer for the oil overhead indicator.
 */
void set_indicator_oil_overheat(indicator_t value);

/**
 * \brief Getter for application.indicator_brake_failure.
 * \return Output buffer for the brake failure indicator.
 */
indicator_t get_indicator_brake_failure(void);

/**
 * \brief Setter for application.indicator_brake_failure.
 * \param[in] value Output buffer for the brake failure indicator.
 */
void set_indicator_brake_failure(indicator_t value);

/**
 * \brief Getter for application.fsm_sidelights.
 * \return Current state of the FSM for the sidelights.
 */
fsm_lights_t get_fsm_sidelights(void);

/**
 * \brief Setter for application.fsm_sidelights.
 * \param[in] value Current state of the FSM for the sidelights.
 */
void set_fsm_sidelights(fsm_lights_t value);

/**
 * \brief Getter for application.fsm_sidelights_timer.
 * \return Timer for the sidelights FSM.
 */
fsm_timer_t get_fsm_sidelights_timer(void);

/**
 * \brief Setter for application.fsm_sidelights_timer.
 * \param[in] value Timer for the sidelights FSM.
 */
void set_fsm_sidelights_timer(fsm_timer_t value);

/**
 * \brief Getter for application.fsm_headlights.
 * \return Current state of the FSM for the headlights.
 */
fsm_lights_t get_fsm_headlights(void);

/**
 * \brief Setter for application.fsm_headlights.
 * \param[in] value Current state of the FSM for the headlights.
 */
void set_fsm_headlights(fsm_lights_t value);

/**
 * \brief Getter for application.fsm_headlights_timer.
 * \return Timer for the headlights FSM.
 */
fsm_timer_t get_fsm_headlights_timer(void);

/**
 * \brief Setter for application.fsm_headlights_timer.
 * \param[in] value Timer for the headlights FSM.
 */
void set_fsm_headlights_timer(fsm_timer_t value);

/**
 * \brief Getter for application.fsm_redlights.
 * \return Current state of the FSM for the redlights.
 */
fsm_lights_t get_fsm_redlights(void);

/**
 * \brief Setter for application.fsm_redlights.
 * \param[in] value Current state of the FSM for the redlights.
 */
void set_fsm_redlights(fsm_lights_t value);

/**
 * \brief Getter for application.fsm_redlights_timer.
 * \return Timer for the redlights FSM.
 */
fsm_timer_t get_fsm_redlights_timer(void);

/**
 * \brief Setter for application.fsm_redlights_timer.
 * \param[in] value Timer for the redlights FSM.
 */
void set_fsm_redlights_timer(fsm_timer_t value);

/**
 * \brief Getter for application.fsm_left_blinker.
 * \return Current state of the FSM for the left blinker.
 */
fsm_blinkers_t get_fsm_left_blinker(void);

/**
 * \brief Setter for application.fsm_left_blinker.
 * \param[in] value Current state of the FSM for the left blinker.
 */
void set_fsm_left_blinker(fsm_blinkers_t value);

/**
 * \brief Getter for application.fsm_left_blinker_timer.
 * \return Timer for the left blinker FSM.
 */
fsm_timer_t get_fsm_left_blinker_timer(void);

/**
 * \brief Setter for application.fsm_left_blinker_timer.
 * \param[in] value Timer for the left blinker FSM.
 */
void set_fsm_left_blinker_timer(fsm_timer_t value);

/**
 * \brief Getter for application.fsm_right_blinker.
 * \return Current state of the FSM for the right blinker.
 */
fsm_blinkers_t get_fsm_right_blinker(void);

/**
 * \brief Setter for application.fsm_right_blinker.
 * \param[in] value Current state of the FSM for the right blinker.
 */
void set_fsm_right_blinker(fsm_blinkers_t value);

/**
 * \brief Getter for application.fsm_right_blinker_timer.
 * \return Timer for the right blinker FSM.
 */
fsm_timer_t get_fsm_right_blinker_timer(void);

/**
 * \brief Setter for application.fsm_right_blinker_timer.
 * \param[in] value Timer for the right blinker FSM.
 */
void set_fsm_right_blinker_timer(fsm_timer_t value);

/**
 * \brief Getter for application.fsm_wipers.
 * \return Current state of the FSM for the wipers.
 */
fsm_wipers_t get_fsm_wipers(void);

/**
 * \brief Setter for application.fsm_wipers.
 * \param[in] value Current state of the FSM for the wipers.
 */
void set_fsm_wipers(fsm_wipers_t value);

/**
 * \brief Getter for application.fsm_wipers_timer.
 * \return Timer for the wipers FSM.
 */
fsm_timer_t get_fsm_wipers_timer(void);

/**
 * \brief Setter for application.fsm_wipers_timer.
 * \param[in] value Timer for the wipers FSM.
 */
void set_fsm_wipers_timer(fsm_timer_t value);
#endif /* DATA_DICTIONARY_H */