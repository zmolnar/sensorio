//
//  This file is part of Sensorio.
//
//  Sensorio is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Sensorio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Sensorio.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef SHARP_LCD_HPP
#define SHARP_LCD_HPP

#define SHARP_LCD_VCOM_CHANGE_PERIOD_IN_MS 1000

void SharpLcdInit(void);

void SharpLcdRegisterDriver(void);

void SharpLcdToggleVcom(void);

void SharpLcdSendVcomIfNeeded(void);

#endif
