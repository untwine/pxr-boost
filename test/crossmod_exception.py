# Copyright 2024 Pixar
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.
#
# Copyright (C) 2003 Rational Discovery LLC.  Distributed under the Boost
# Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
# at http://www.boost.org/LICENSE_1_0.txt)
#
# Modified by Jeremy Retailleau.

print("running...")

import crossmod_exception_a
import crossmod_exception_b

try:
  crossmod_exception_b.tossit()
except IndexError:
  pass
try:
  crossmod_exception_a.tossit()
except IndexError:
  pass

print("Done.")
