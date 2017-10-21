3D Parallel N-Body Simulator in OpenGL
Simulations of dynamical systems of particles are often used in physics to predict behavior of planets, stars, galaxies, gas particles... The interaction between particles is described by physically sound equations and ”integrated” in time to predict the outcome of the simulation.

###Features

Uses the Barnes-Hut algorithm and the Naive N-Body algorithm to simulate particle gravity interactions in C++ 11
Parallelization using Intel Thread Building Blocks
OpenGL for the 3D visualisation.

Documentation

Documentation comparing the speed-up between the serial and parallel versions: https://onedrive.live.com/redir?resid=F3C315EB7F683B03!16208&authkey=!ABgFWP56pvq2rCs&ithint=file%2cpdf

###References

Intel Developer zone - n-bodies: a parallel TBB solution: computing accelerations? or forces? - https://software.intel.com/enus/blogs/2009/09/22/n-bodies-a-parallel-tbb-solution-computingaccelerations-or-forces
Wikipedia: Fast Inverse Square Root - https://en.wikipedia.org/wiki/Fastinversesquareroot
Wikipedia: Barnes-Hut simulation - https://en.wikipedia.org/wiki/BarnesHutsimulation
Tutorial: Scalable Memory Allocator - https://www.threadingbuildingblocks.org/tutorial-intel-tbb-scalablememory-allocator