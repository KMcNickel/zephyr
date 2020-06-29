/*
 * Copyright (c) 2020 Laird Connectivity
 * Copyright (c) 2019 Electronut Labs
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT silabs_si7055

#include <drivers/sensor.h>
#include <kernel.h>
#include <device.h>
#include <logging/log.h>
#include <drivers/i2c.h>
#include "si7055.h"

LOG_MODULE_REGISTER(si7055, CONFIG_SENSOR_LOG_LEVEL);

struct si7055_data {
	struct device *i2c_dev;
	uint16_t temperature;
};

/**
 * @brief function to get temperature
 *
 * @return int 0 on success
 */
static int si7055_get_temperature(struct device *i2c_dev,
			       struct si7055_data *si_data)
{
	int retval;
	uint8_t temp[2];

	retval = i2c_burst_read(i2c_dev, DT_INST_REG_ADDR(0),
		SI7055_MEAS_TEMP_MASTER_MODE, temp, sizeof(temp));

	if (retval == 0) {
		si_data->temperature = (temp[0] << 8) | temp[1];
	} else {
		LOG_ERR("read register err");
	}

	return retval;
}

/**
 * @brief fetch a sample from the sensor
 *
 * @return 0
 */
static int si7055_sample_fetch(struct device *dev, enum sensor_channel chan)
{
	int retval;
	struct si7055_data *si_data = dev->driver_data;

	retval = si7055_get_temperature(si_data->i2c_dev, si_data);

	return retval;
}

/**
 * @brief sensor value get
 *
 * @return -ENOTSUP for unsupported channels
 */
static int si7055_channel_get(struct device *dev, enum sensor_channel chan,
			      struct sensor_value *val)
{
	struct si7055_data *si_data = dev->driver_data;

	if (chan == SENSOR_CHAN_AMBIENT_TEMP) {

		int32_t temp_ucelcius = (((17572 * (int32_t)si_data->temperature)
					/ 65536) - 4685) * 10000;

		val->val1 = temp_ucelcius / 1000000;
		val->val2 = temp_ucelcius % 1000000;

		LOG_DBG("temperature = val1:%d, val2:%d", val->val1, val->val2);

		return 0;
	} else {
		return -ENOTSUP;
	}
}

static const struct sensor_driver_api si7055_api = {
	.sample_fetch = &si7055_sample_fetch,
	.channel_get = &si7055_channel_get,
};

/**
 * @brief initiasize the sensor
 *
 * @return 0 for success
 */

static int si7055_init(struct device *dev)
{
	struct si7055_data *drv_data = dev->driver_data;

	drv_data->i2c_dev = device_get_binding(
		DT_INST_BUS_LABEL(0));

	if (!drv_data->i2c_dev) {
		LOG_ERR("i2c master not found.");
		return -EINVAL;
	}

	LOG_DBG("si7055 init ok");

	return 0;
}

static struct si7055_data si_data;

DEVICE_AND_API_INIT(si7055, DT_INST_LABEL(0), si7055_init,
	&si_data, NULL, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY, &si7055_api);
