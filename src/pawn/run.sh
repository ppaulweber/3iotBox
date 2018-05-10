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

out=logic.m2m

cat logic.header.m2m                                                             > $out

for i in {0..983}; do
    echo "    <field>"                                                          >> $out
    echo "        name       = field$(expr $i + 2)"	                        >> $out
    echo "        alias      = data$i"			                        >> $out
    echo "        title      = Receipt (Beleg) Data $i"	                        >> $out
    echo "        type       = u32"			                        >> $out
    echo "        decpl      = 0"                                               >> $out
    echo "        byteofs    = $(expr $i \* 4 + 64)"	                        >> $out
    echo "        default    = 0"			                        >> $out
    echo "        edit       = 5"			                        >> $out
    echo "    </field>"					                        >> $out
done
echo "</table>"                                                                 >> $out
echo ""                                                                         >> $out

# for c in {3..9}; do
#     echo "<table>"                                                                  >> $out
#     echo "    name  = config$c"                                                     >> $out
#     echo "    title = Electronic Receipt (Elektronischer Beleg) [https://obono.at]" >> $out
#     for i in {0..999}; do
# 	echo "    <field>"                                                          >> $out
# 	echo "        name       = field$i"					    >> $out
# 	echo "        alias      = data$i"              			    >> $out
# 	echo "        title      = Receipt (Beleg) Data $i"                         >> $out
# 	echo "        type       = u32"						    >> $out
# 	echo "        byteofs    = $(expr $i \* 4)"				    >> $out
# 	echo "        decpl      = 0"                                               >> $out
# 	echo "        default    = 0"						    >> $out
# 	echo "        edit       = 5"						    >> $out
# 	echo "    </field>"							    >> $out
#     done
#     echo "</table>"                                                                 >> $out
#     echo ""                                                                         >> $out
# done

cat logic.footer.m2m                                                            >> $out
