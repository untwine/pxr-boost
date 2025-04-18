# Copyright 2024 Pixar
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.
#
# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Modified by Jeremy Retailleau.

'''
>>> from keywords import *
>>> f = Foo()
>>> f.a(), f.b(), f.n()
(0, 0.0, '')
>>> f = Foo(1)
>>> f.a(), f.b(), f.n()
(1, 0.0, '')
>>> f = Foo(1,1.0)
>>> f.a(), f.b(), f.n()
(1, 1.0, '')
>>> f = Foo(1,1.0,"1")
>>> f.a(), f.b(), f.n()
(1, 1.0, '1')
>>> f = Foo(a=1)
>>> f.a(), f.b(), f.n()
(1, 0.0, '')
>>> f = Foo(b=1)
>>> f.a(), f.b(), f.n()
(0, 1.0, '')
>>> f = Foo(n="1")
>>> f.a(), f.b(), f.n()
(0, 0.0, '1')
>>> f = Foo(1,n="1")
>>> f.a(), f.b(), f.n()
(1, 0.0, '1')
>>> f.set()
>>> f.a(), f.b(), f.n()
(0, 0.0, '')
>>> f.set(1)
>>> f.a(), f.b(), f.n()
(1, 0.0, '')
>>> f.set(1,1.0)
>>> f.a(), f.b(), f.n()
(1, 1.0, '')
>>> f.set(1,1.0,"1")
>>> f.a(), f.b(), f.n()
(1, 1.0, '1')
>>> f.set(a=1)
>>> f.a(), f.b(), f.n()
(1, 0.0, '')
>>> f.set(b=1)
>>> f.a(), f.b(), f.n()
(0, 1.0, '')
>>> f.set(n="1")
>>> f.a(), f.b(), f.n()
(0, 0.0, '1')
>>> f.set(1,n="1")
>>> f.a(), f.b(), f.n()
(1, 0.0, '1')
>>> f.set2(b=2.0,n="2",a=2)
>>> f.a(), f.b(), f.n()
(2, 2.0, '2')

# lets see how badly we've broken the 'regular' functions
>>> f = Bar()
>>> f.a(), f.b(), f.n()
(0, 0.0, '')
>>> f = Bar(1)
>>> f.a(), f.b(), f.n()
(1, 0.0, '')
>>> f = Bar(1,1.0)
>>> f.a(), f.b(), f.n()
(1, 1.0, '')
>>> f = Bar(1,1.0,"1")
>>> f.a(), f.b(), f.n()
(1, 1.0, '1')
>>> f.set()
>>> f.a(), f.b(), f.n()
(0, 0.0, '')
>>> f.set(1)
>>> f.a(), f.b(), f.n()
(1, 0.0, '')
>>> f.set(1,1.0)
>>> f.a(), f.b(), f.n()
(1, 1.0, '')
>>> f.set(1,1.0,"1")
>>> f.a(), f.b(), f.n()
(1, 1.0, '1')
>>> f.set2.__doc__.splitlines()[1]
'set2( (Bar)arg1 [, (int)arg2 [, (float)arg3 [, (str)arg4]]]) -> None :'
>>> f.set2.__doc__.splitlines()[2]
"    set2's docstring"

# Test overloads with different arity and keyword args.

# Calls f1 since f2 must be called with at least two args.
>>> func(0)
(0, 2)

# Even though we supply the exact number of args to match f1, this calls
# f2 because the number of args also matches that function, and overloads
# are resolved in reverse registration order.
>>> func(0, 1)
(0, 1, 2)

# This calls f1 because the keyword arg matches.
>>> func(0, a1=2)
(0, 2)
'''




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

