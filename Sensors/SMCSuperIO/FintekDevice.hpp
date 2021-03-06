//
//  FintekDevice.hpp
//
//  Sensors implementation for FINTEK SuperIO device
//
//  Based on https://github.com/kozlek/HWSensors/blob/master/SuperIOSensors/F718xxSensors.cpp
//  @author joedm
//

#ifndef _FINTEKDEVICE_HPP
#define _FINTEKDEVICE_HPP

#include "SuperIODevice.hpp"
#include "WinbondFamilyDevice.hpp"

namespace Fintek {
	static constexpr uint8_t FINTEK_MAX_VOLTAGE_COUNT = 9;
	static constexpr uint8_t FINTEK_MAX_TACHOMETER_COUNT = 4;
	static constexpr uint8_t FINTEK_ADDRESS_REGISTER_OFFSET = 0x05;
	static constexpr uint8_t FINTEK_DATA_REGISTER_OFFSET = 0x06;
	
	// Hardware Monitor Registers
	static constexpr uint8_t FINTEK_TEMPERATURE_CONFIG_REG = 0x69;
	static constexpr uint8_t FINTEK_TEMPERATURE_BASE_REG = 0x70;
	static constexpr uint8_t FINTEK_VOLTAGE_REG[FINTEK_MAX_VOLTAGE_COUNT] = { 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28 };
	static constexpr uint8_t FINTEK_FAN_TACHOMETER_REG[] = { 0xA0, 0xB0, 0xC0, 0xD0 };
	static constexpr uint8_t FINTEK_TEMPERATURE_EXT_REG[] = { 0x7A, 0x7B, 0x7C, 0x7E };

	class FintekDevice : public WindbondFamilyDevice {
	private:
		/**
		 *  Tachometers
		 */
		uint16_t tachometers[FINTEK_MAX_TACHOMETER_COUNT] = { 0 };
		/**
		 *  Voltages
		 */
		float voltages[FINTEK_MAX_VOLTAGE_COUNT] = { 0.0 };
	protected:
		/**
		 *  Implementations for tachometer reading.
		 */
		uint16_t tachometerRead(uint8_t);
		void setTachometerValue(uint8_t index, uint16_t value) override {
			if (index < getTachometerCount() && index < FINTEK_MAX_TACHOMETER_COUNT) {
				tachometers[index] = value;
			}
		}
		uint16_t getTachometerValue(uint8_t index) override {
			if (index < getTachometerCount() && index < FINTEK_MAX_TACHOMETER_COUNT) {
				return tachometers[index];
			}
			return 0;
		}
		/**
		 * Reads voltage data. Invoked from update() only.
		 */
		float voltageRead(uint8_t);
		float voltageRead71808E(uint8_t);
		void setVoltageValue(uint8_t index, float value) override {
			if (index < getVoltageCount() && index < FINTEK_MAX_VOLTAGE_COUNT) {
				voltages[index] = value;
			}
		}
		float getVoltageValue(uint8_t index) override {
			if (index < getVoltageCount() && index < FINTEK_MAX_VOLTAGE_COUNT) {
				return voltages[index];
			}
			return 0.0f;
		}
		/**
		 *  Device access
		 */
		uint8_t readByte(uint8_t reg);
	public:
	
		/**
		 *  Overrides
		 */
		void setupKeys(VirtualSMCAPI::Plugin &vsmcPlugin) override;
		
		/**
		 *  Ctors
		 */
		FintekDevice() = default;
	};
}

#endif // _FINTEKDEVICE_HPP
