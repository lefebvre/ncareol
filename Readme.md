# Requirements
* C compiler. gcc 4.8 tested
* Git
* CMake. 2.8.12.2 tested.

# Getting Started
* Clone ncareol
`git clone https://github.com/lefebvre/ncareol ncareol
* Change directory into ncareol `cd ~/ncareol`
* Clone TriBITS
`git clone https://github.com/lefebvre/TriBITS.git TriBITS` [TriBITS documentation](https://tribits.org/doc/TribitsDevelopersGuide.html)
* Clone extra repos
`./TriBITS/tribits/ci_support/clone_extra_repos.py`
* Create a build directory
`mkdir -p ~/build/ncareol`
* Change into the build `cd ~/build/ncareol`
* Create a configuration script in ~/build/. Lets call is ../configure.sh(linux), ..\configure.bat(windows)

```
#!/bin/bash
# Linux bash file example
rm -rf CMake*
cmake \
 -D CMAKE_BUILD_TYPE:STRING=RELEASE \
 -D ncareol_ENABLE_ALL_PACKAGES:BOOL=ON \
 -D ncareol_ENABLE_TESTS:BOOL=ON \
 -G "Unix Makefiles" \
 ~/ncareol
```
```
rem Windows MinGW bat file example
del /S /Q CMake*
cmake ^
 -D CMAKE_BUILD_TYPE:STRING=RELEASE ^
 -D ncareol_ENABLE_ALL_PACKAGES:BOOL=ON ^
 -D ncareol_ENABLE_TESTS:BOOL=ON ^
 -G "MinGW Makefiles" ^
 ~/ncareol
```

* Invoke configure script in the build directory.
`../configure.sh or ..\configure.bat`
  * I place the configure script in the build directory as opposed to the build/ncareol directory because it allows me to delete the build/ncareol
directory without removing my script.

# SkewT
Enable with '-Dncareol_ENABLE_skewt:BOOL=ON'
 SkewT provides a generic implementation of the aerological diagram
 which utilizes a skewed temperature scale for the horizontal axis and
 an inverted logrithmic pressure scale for the vertical axis. For a
 good reference on aerological diagrams, see:

 *J. V. Iribarne and W. L. Godson, <i>Atmospheric Thermodynamics</i>,
 D. Reidel, Boston (1973)*

SkewT formulates the diagram in a virtual coordinate space, and then
calls a class derived from SkewTAdapter, to render the diagram
for a particular graphics environment. Example enviroments might be
Trolltech Qt, or a Microsoft Windows graphing component.

The only data required by SkewT are pressure, temperature,
relative humidity, wind speed and wind direction.

The functionality of SkewT is somewhat dependent upon the SkewTAdapter,
which is correspondingly dependent on the graphics subsystem that it is
designed for. In some cases, all of the graph data must be provided at
construction time, and a static graph will be created. In other cases,
a "real-time" update mode is possible, where data points may be incrementally
added to the graph.
