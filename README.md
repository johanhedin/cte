cte
====
[![Build](https://github.com/johanhedin/cte/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/johanhedin/cte/actions/workflows/c-cpp.yml)

`cte` is a small example for how to build a C executable with cmake. It include
config for how to link to another library in three different ways: in-tree, static
and shared. The cmake configuration works with cmake from 2.8.12 all the way up
to 3.28.

Download and build
----
Clone `cte` from GitHub with the needed submodules:

    $ git clone --recurse-submodules https://github.com/johanhedin/cte.git
    $ cd cte

and then build with cmake:

    $ mkdir build
    $ cd build
    $ cmake .. -DCMAKE_BUILD_TYPE=Release
    $ make

`cte`accepts the following build options on the command line (set with -D):

 * `CTE_LIBCTL_LINK_MODE={Submodule,Static,Shared}` Defaults to `Submodule` if not set
 * `CMAKE_BUILD_TYPE={Debug,Release,RelWithDebInfo}` Defaults to `Debug` if not set
 * `CMAKE_INSTALL_PREFIX=<path>` Defaults to `/usr/local` if not set

Keep up to date with changes
----
To keep up to date with changes and updates to `cte`, simply run:

    $ cd cte
    $ git pull --ff-only
    $ git submodule update --init --recursive
    $ cd build
    $ cmake ..
    $ make

Do not forget to run the `git submodule` part to keep in sync with submodule
changes.
