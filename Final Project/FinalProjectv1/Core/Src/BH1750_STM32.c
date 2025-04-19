#include "BH1750_STM32.h"
#include <stdbool.h>

// Initialize sensor struct
void BH1750_Init(BH1750 *sensor, uint8_t addr, I2C_HandleTypeDef *hi2c) {
  sensor->addr = addr;
  sensor->hi2c = hi2c;
  sensor->mtreg = BH1750_DEFAULT_MTREG;
  sensor->mode = BH1750_UNCONFIGURED;
  sensor->convFactor = 1.2f;
}

// Begin communication
uint8_t BH1750_Begin(BH1750 *sensor, BH1750_Mode mode) {
  return (BH1750_Configure(sensor, mode) && BH1750_SetMTreg(sensor, BH1750_DEFAULT_MTREG));
}

// Configure mode
uint8_t BH1750_Configure(BH1750 *sensor, BH1750_Mode mode) {
  uint8_t cmd = (uint8_t)mode;
  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(sensor->hi2c, sensor->addr << 1, &cmd, 1, HAL_MAX_DELAY);

  if (status != HAL_OK) return false;

  HAL_Delay(10);
  sensor->mode = mode;
  sensor->lastReadTimestamp = HAL_GetTick();
  return true;
}

// Set MT register
uint8_t BH1750_SetMTreg(BH1750 *sensor, uint8_t mtreg) {
  if (mtreg < BH1750_MTREG_MIN || mtreg > BH1750_MTREG_MAX) return false;

  uint8_t cmd1 = (0b01000 << 3) | (mtreg >> 5);
  uint8_t cmd2 = (0b011 << 5) | (mtreg & 0b11111);

  if (HAL_I2C_Master_Transmit(sensor->hi2c, sensor->addr << 1, &cmd1, 1, HAL_MAX_DELAY) != HAL_OK) return false;
  if (HAL_I2C_Master_Transmit(sensor->hi2c, sensor->addr << 1, &cmd2, 1, HAL_MAX_DELAY) != HAL_OK) return false;
  if (HAL_I2C_Master_Transmit(sensor->hi2c, sensor->addr << 1, (uint8_t*)&sensor->mode, 1, HAL_MAX_DELAY) != HAL_OK) return false;

  HAL_Delay(10);
  sensor->mtreg = mtreg;
  return true;
}

// Check if measurement is ready
uint8_t BH1750_MeasurementReady(BH1750 *sensor, uint8_t maxWait) {
  uint32_t delayTime = 0;
  switch (sensor->mode) {
    case BH1750_CONTINUOUS_HIGH_RES_MODE:
    case BH1750_CONTINUOUS_HIGH_RES_MODE_2:
    case BH1750_ONE_TIME_HIGH_RES_MODE:
    case BH1750_ONE_TIME_HIGH_RES_MODE_2:
      delayTime = maxWait ? 180 * sensor->mtreg / BH1750_DEFAULT_MTREG : 120 * sensor->mtreg / BH1750_DEFAULT_MTREG;
      break;
    case BH1750_CONTINUOUS_LOW_RES_MODE:
    case BH1750_ONE_TIME_LOW_RES_MODE:
      delayTime = maxWait ? 24 * sensor->mtreg / BH1750_DEFAULT_MTREG : 16 * sensor->mtreg / BH1750_DEFAULT_MTREG;
      break;
    default: return false;
  }
  return (HAL_GetTick() - sensor->lastReadTimestamp) >= delayTime;
}

// Read light level
float BH1750_ReadLightLevel(BH1750 *sensor) {
  if (sensor->mode == BH1750_UNCONFIGURED) return -2.0f;

  uint8_t data[2];
  if (HAL_I2C_Master_Receive(sensor->hi2c, sensor->addr << 1, data, 2, HAL_MAX_DELAY) != HAL_OK) return -1.0f;

  uint16_t raw = (data[0] << 8) | data[1];
  float level = raw;

  if (sensor->mtreg != BH1750_DEFAULT_MTREG) {
    level *= (BH1750_DEFAULT_MTREG / (float)sensor->mtreg);
  }

  if (sensor->mode == BH1750_ONE_TIME_HIGH_RES_MODE_2 || sensor->mode == BH1750_CONTINUOUS_HIGH_RES_MODE_2) {
    level /= 2;
  }

  level /= sensor->convFactor;
  sensor->lastReadTimestamp = HAL_GetTick();
  return level;
}
