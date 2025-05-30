# Copyright 2024 Pixar
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.
#
# Copyright Joel de Guzman 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Modified by Jeremy Retailleau.

'''

>>> import pointer_vector_ext
>>> d = pointer_vector_ext.DoesSomething()
>>> lst = d.returnList()
>>> lst[0].f();
'harru'

'''


def run(args = None):
    import sys
    import doctest

    if args is not None:
        sys.argv = args
    return doctest.testmod(sys.modules.get(__name__))

if __name__ == '__main__':
    print('running...')
    import sys
    status = run()[0]
    if (status == 0): print("Done.")
    sys.exit(status)



