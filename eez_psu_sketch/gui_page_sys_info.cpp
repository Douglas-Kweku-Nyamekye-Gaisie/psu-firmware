/*
 * EEZ PSU Firmware
 * Copyright (C) 2016-present, Envox d.o.o.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "psu.h"

#if OPTION_DISPLAY

#include "fan.h"
#include "temperature.h"

#include "gui_page_sys_info.h"

namespace eez {
namespace psu {
namespace gui {

data::Value SysInfoPage::getData(const data::Cursor &cursor, uint8_t id) {
	if (id == DATA_ID_SYS_ON_TIME_TOTAL) {
		return data::Value((uint32_t)g_powerOnTimeCounter.getTotalTime(), VALUE_TYPE_ON_TIME_COUNTER);
	}

	if (id == DATA_ID_SYS_ON_TIME_LAST) {
		return data::Value((uint32_t)g_powerOnTimeCounter.getLastTime(), VALUE_TYPE_ON_TIME_COUNTER);
	}

	if (id == DATA_ID_SYS_INFO_FIRMWARE_VER) {
		return data::Value(FIRMWARE);
	}

	if (id == DATA_ID_SYS_INFO_SERIAL_NO) {
		return data::Value(persist_conf::devConf.serialNumber);
	}

	if (id == DATA_ID_SYS_INFO_CPU) {
		return data::Value(getCpuModel());
	}

	if (cursor.i >= 0) {
		int iChannel = cursor.i >= 0 ? cursor.i : g_channel->index - 1;

		if (id == DATA_ID_CHANNEL_BOARD_INFO_LABEL) {
			return data::Value(iChannel + 1, VALUE_TYPE_CHANNEL_BOARD_INFO_LABEL);
		}

		if (id == DATA_ID_CHANNEL_BOARD_INFO_REVISION) {
			if (iChannel < CH_NUM) {
				return data::Value(Channel::get(iChannel).getBoardRevisionName());
			}
		}
	}

	if (id == DATA_ID_SYS_INFO_SCPI_VER) {
		return data::Value(SCPI_STD_VERSION_REVISION);
	}

	if (id == DATA_ID_SYS_INFO_ETHERNET) {
		return data::Value(getCpuEthernetType());
	}


#if EEZ_PSU_SELECTED_REVISION == EEZ_PSU_REVISION_R1B9 || !OPTION_FAN
	if (id == DATA_ID_SYS_INFO_FAN_STATUS) {
		return data::Value(3);
	}
#elif EEZ_PSU_SELECTED_REVISION == EEZ_PSU_REVISION_R3B4 || EEZ_PSU_SELECTED_REVISION == EEZ_PSU_REVISION_R5B12
	if (fan::g_testResult == TEST_FAILED || fan::g_testResult == TEST_WARNING) {
	    if (id == DATA_ID_SYS_INFO_FAN_STATUS) {
		    return data::Value(0);
	    }
	} else if (fan::g_testResult == TEST_OK) {
#if FAN_OPTION_RPM_MEASUREMENT
	    if (id == DATA_ID_SYS_INFO_FAN_STATUS) {
		    return data::Value(1);
	    }
	    if (id == DATA_ID_SYS_INFO_FAN_SPEED) {
		    return data::Value((float)fan::g_rpm, VALUE_TYPE_FLOAT_RPM);
	    }
#else
	    if (id == DATA_ID_SYS_INFO_FAN_STATUS) {
		    return data::Value(2);
	    }
#endif
	} else {
	    if (id == DATA_ID_SYS_INFO_FAN_STATUS) {
		    return data::Value(3);
	    }
	}
#endif

	return data::Value();
}

}
}
} // namespace eez::psu::gui

#endif