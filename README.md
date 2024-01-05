cte
====

`cte` is a simple template for how to build a C executable with cmake. It include
config for how to link to another library in three different ways: in-tree, static
and shared.

Download and build
----
Clone `cte` from GitHub with the needed submodules:

    $ git clone --recurse-submodules https://github.com/johanhedin/ctl.git
    $ cd cte

and then build with cmake:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

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
