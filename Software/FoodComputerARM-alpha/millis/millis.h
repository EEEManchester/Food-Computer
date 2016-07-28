/*******************************************************************************
 * This file is part of the millis library.                                    *
 *                                                                             *
 * millis is free software: you can redistribute it and/or                     *
 * modify it under the terms of the GNU General Public License as              *
 * published by the Free Software Foundation, either version 3 of              *
 * the License, or any later version.                                          *
 *                                                                             *
 * millis is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * millis is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public            *
 * License along with millis. If not, see                                      *
 * <http://www.gnu.org/licenses/>.                                             *
 ******************************************************************************/

/*
 *  Copyright:  DFRobot
 *  name:       millis
 *  version:    1.0
 *  Author:     lisper (lisper.li@dfrobot.com)
 *  Date:       2014-10-30
 *  Description:    millis library for mbed
 */

#include "mbed.h"

void millisTicker ();

uint32_t millis ();

void startMillis ();

void stopMillis ();