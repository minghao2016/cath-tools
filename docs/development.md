
Development
===========

This page contains extra information that may be useful for anyone working on the development of cath-tools. For information on how to set up a standard build of the code, see [Build](build).


Doxygen Code Documentation
--------------------------

Much of the code is documented inline with Doxygen. To view it, install doxygen, run `doxygen` in the cath-tools root directory and then view `doxygen_documentation/html/index.html` in a browser.

<!-- TODO: Set up Doxygen build? -->


Compiling Multiple Versions (release/relwithdebinfo/debug)
----------------------------------------------------------

To compile multiple versions, you can put them in individual directories. First make sure that you don't have any
build files in the root directory and then run something like:


~~~~~no-highlight
$ mkdir release
$ cd release
$ cmake -DCMAKE_BUILD_TYPE=RELEASE ..
$ cd ..
$ make -C release
~~~~~

Available types are: release, relwithdebinfo and debug

NOTE: the debug build requires a debug version of Boost built with `_GLIBCXX_DEBUG` enabled otherwise the resulting code will have all sorts of
horrible, non-obvious errors. If you really want to build a debug version without doing this, you can work around this issue by removing all
mentions of `-D_GLIBCXX_DEBUG` from CMakeLists.txt.

Example CMake Commands
----------------------

**clang_debug**

~~~~~no-highlight
/usr/bin/cmake -DCMAKE_BUILD_TYPE=DEBUG          -DBUILD_SHARED_LIBS=ON -DBOOST_ROOT=/opt/boost_1_58_0_clang_build -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_CXX_FLAGS="-stdlib=libc++" ..
~~~~~

**clang_release**

~~~~~no-highlight
/usr/bin/cmake -DCMAKE_BUILD_TYPE=RELEASE                               -DBOOST_ROOT=/opt/boost_1_58_0_clang_build -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_CXX_FLAGS="-stdlib=libc++" ..
~~~~~

**clang_relwithdebinfo**

~~~~~no-highlight
/usr/bin/cmake -DCMAKE_BUILD_TYPE=RELWITHDEBINFO                        -DBOOST_ROOT=/opt/boost_1_58_0_clang_build -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_CXX_FLAGS="-stdlib=libc++" ..
~~~~~

**gcc_debug**

~~~~~no-highlight
/usr/bin/cmake -DCMAKE_BUILD_TYPE=DEBUG          -DBUILD_SHARED_LIBS=ON -DBOOST_ROOT=/opt/boost_1_58_0_gcc_build                                                                                                              ..
~~~~~

**gcc_release**

~~~~~no-highlight
/usr/bin/cmake -DCMAKE_BUILD_TYPE=RELEASE                               -DBOOST_ROOT=/opt/boost_1_58_0_gcc_build                                                                                                              ..
~~~~~

**gcc_relwithdebinfo**

~~~~~no-highlight
/usr/bin/cmake -DCMAKE_BUILD_TYPE=RELWITHDEBINFO                        -DBOOST_ROOT=/opt/boost_1_58_0_gcc_build                                                                                                              ..
~~~~~

Consider using ninja instead of make
------------------------------------

If you're developing, consider using ninja instead of make. It's a drop-in replacement that's much quicker to get started, which can make a big difference on incremental builds. To use ninja, install a suitable package and add `-GNinja` to your CMake commands.

Shared Versus Static
----------------

At present, the builds are all completely static. This should be made more configurable in the future so that (at least debug) builds can be run in static mode using the CMake flag `-DBUILD_SHARED_LIBS=ON`.

Clang Static Analzyer
--------------------------------------

Want to search for bugs in the code? These instructions aim to help you run Clang-based static analysis. (TODO: Get this running on a CI server, such as Travis-CI.)

Ensure you have clang installed. Find the analyzer programs `ccc-analyzer` and `c++-analyzer`. For example, on Ubuntu you can do something like:

~~~~~no-highlight
dpkg -l | grep clang | awk '{print $2}' | xargs dpkg -L | grep analyzer
~~~~~

Then substitute their locations into the following commands and then run the commands, starting in the root of the cath-tools project (with bioplib already built if you don't want errors from static analysis of bioplib):


~~~~~no-highlight
mkdir build-analyze && cd build-analyze
setenv CCC_CC  clang
setenv CCC_CXX clang++
# For Clang 3.6
/usr/bin/cmake -DBOOST_ROOT=/opt/boost_1_58_0_clang_build -DCMAKE_C_COMPILER="/usr/share/clang/scan-build-3.6/ccc-analyzer"         -DCMAKE_CXX_COMPILER="/usr/share/clang/scan-build-3.6/c++-analyzer"         -DCMAKE_CXX_FLAGS="-stdlib=libc++" ..
# For Clang 3.8
/usr/bin/cmake -DBOOST_ROOT=/opt/boost_1_58_0_clang_build -DCMAKE_C_COMPILER="/usr/share/clang/scan-build-3.8/libexec/ccc-analyzer" -DCMAKE_CXX_COMPILER="/usr/share/clang/scan-build-3.8/libexec/c++-analyzer" -DCMAKE_CXX_FLAGS="-stdlib=libc++" ..
scan-build make
~~~~~

To get parallel compilation, you can append ` -j #` to the `scan-build make` (where `#` is the number of threads).



Checking headers compile independently
--------------------------------------

~~~~~no-highlight
find source -iname '*.h' | grep third_party_code -v | xargs -P 4 -I VAR clang++ -x c++ -DBOOST_LOG -std=c++1y -stdlib=libc++ -W -Wall -Werror -Wextra -Wno-unused-const-variable -Wno-unused-local-typedef -Wsign-compare -Wcast-qual -Wconversion -Wnon-virtual-dtor -pedantic -ftemplate-backtrace-limit=0 -c -o /tmp/.comp_clang.dummy.header.o -isystem /opt/boost_1_58_0_clang_build/include -I source VAR
~~~~~



Fixing trailing namespace comments
----------------------------------

~~~~~no-highlight
find source -iname '*.h' | sort | grep third_party_code -v | xargs -P 4 -I VAR clang-tidy -fix -checks=llvm-namespace-comment VAR -- -x c++ -std=c++1y -isystem /opt/boost_1_58_0_clang_build/include -I source
~~~~~



Fixing header guards
--------------------

~~~~~no-highlight
ln -s source src
find src/   -iname '*.h' | sort | grep third_party_code -v | xargs -P 4 -I VAR clang-tidy -fix -checks=llvm-header-guard      VAR -- -x c++ -std=c++1y -isystem /opt/boost_1_58_0_clang_build/include -I source
rm -f src
~~~~~
