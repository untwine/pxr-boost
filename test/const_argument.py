# Copyright 2024 Pixar
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.
#
# Copyright Jonathan Brandmeyer,  2004. Distributed under the Boost 
# Software License, Version 1.0 (See accompanying 
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Modified by Jeremy Retailleau.

"""
>>> from const_argument_ext import *
>>> accept_const_arg(1)
1
"""

def run(args = None):
    import sys
    import doctest

    if args is not None:
        sys.argv = args
    return doctest.testmod(sys.modules.get(__name__))
    
if __name__ == '__main__':
    print("running...")
    import sys
    status = run()[0]
    if (status == 0): print("Done.")
    sys.exit(status)
