# Copyright 2024 Pixar
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.
#
# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Modified by Jeremy Retailleau.

r'''>>> import pickle2_ext
    >>> import pickle
    >>> pickle2_ext.world.__module__
    'pickle2_ext'
    >>> pickle2_ext.world.__safe_for_unpickling__
    1
    >>> pickle2_ext.world.__name__
    'world'
    >>> pickle2_ext.world('Hello').__reduce__()
    (<class 'pickle2_ext.world'>, ('Hello',), (0,))
    >>> for number in (24, 42):
    ...   wd = pickle2_ext.world('California')
    ...   wd.set_secret_number(number)
    ...   pstr = pickle.dumps(wd)
    ...   wl = pickle.loads(pstr)
    ...   print(wd.greet(), wd.get_secret_number())
    ...   print(wl.greet(), wl.get_secret_number())
    Hello from California! 24
    Hello from California! 24
    Hello from California! 42
    Hello from California! 0

# Now show that the __dict__ is not taken care of.
    >>> wd = pickle2_ext.world('California')
    >>> wd.x = 1
    >>> wd.__dict__
    {'x': 1}
    >>> try: pstr = pickle.dumps(wd)
    ... except RuntimeError as err: print(err)
    ...
    Incomplete pickle support (__getstate_manages_dict__ not set)
'''

from __future__ import print_function

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
