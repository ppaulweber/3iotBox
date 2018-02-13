#
#   Copyright (C) 2018 Philipp Paulweber, Simon Tragatschnig, and Patrick Gaubatz
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 <https://github.com/ppaulweber/3iotBox>
#
#   This file is part of 3iotBox.
#
#   3iotBox is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   3iotBox is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with 3iotBox. If not, see <http://www.gnu.org/licenses/>.
#

TARGET = 3iotBox

UPDATE_ROOT  = lib/stdhl
UPDATE_PATH  = .
UPDATE_FILE  = .cmake/LibPackage.cmake
UPDATE_FILE += .cmake/config.mk

include .cmake/config.mk
