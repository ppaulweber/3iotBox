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

set( PROJECT 3iotBox )

cmake_minimum_required( VERSION ${CMAKE_VERSION} )

project( ${PROJECT} LANGUAGES C CXX )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_VERSION 3.2 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_MODULE_PATH ${${PROJECT}_SOURCE_DIR}/.cmake )


#
# packages
#

include( LibPackage )

package_git( ${PROJECT} )
package_git_submodule( pawn master REQUIRED obj lib/pawn )


#
# includes
#

include( FeatureSummary )
include( CPack )

add_subdirectory( src )


#
# project
#

add_custom_target( ${PROJECT}
#  COMMAND make --no-print-directory -C lib/stdhl
  DEPENDS
    ${PROJECT}-app
  WORKING_DIRECTORY
    ${PROJECT_SOURCE_DIR}
)

#
# test
#

# add_custom_target( ${PROJECT}-check
#   COMMAND make --no-print-directory -C lib/pawn test
#   WORKING_DIRECTORY
#     ${PROJECT_SOURCE_DIR}
# )


#
# benchmark
#

# add_custom_target( ${PROJECT}-run
#   COMMAND make --no-print-directory -C lib/pawn benchmark
#   WORKING_DIRECTORY
#     ${PROJECT_SOURCE_DIR}
# )


#
# install
#

install(
  TARGETS
  EXPORT   ${PROJECT}Targets
  LIBRARY  DESTINATION lib
  ARCHIVE  DESTINATION lib
  RUNTIME  DESTINATION bin
  INCLUDES DESTINATION include
)


#
# cmake
#

# install(
#   FILES
#     .cmake/Find${PROJECT}.cmake
#   DESTINATION
#     "share/cmake/Module/${PROJECT}"
#   )


#
# build features and checks
#

feature_summary( WHAT
  PACKAGES_NOT_FOUND
  DISABLED_FEATURES
)
