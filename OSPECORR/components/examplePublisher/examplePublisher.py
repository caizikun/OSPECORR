#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#
#   This file is part of OSPECOR².
#
#   OSPECOR² is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   OSPECOR² is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with OSPECOR².  If not, see <http://www.gnu.org/licenses/>.
#
#   Copyright 2011 Andre Puschmann <andre.puschmann@tu-ilmenau.de>
#

import scl
import random
import time
import phy_pb2

map = scl.generate_map("examplePublisher")
sock = map["fastSensingResult"]
print "continously sending sensing results .."
while True:
    result = phy_pb2.fastSensingResult()
    result.rssi = random.uniform(50, 80)
    print result.rssi
    string = result.SerializeToString()
    sock.send(string)
    time.sleep(0.2)
