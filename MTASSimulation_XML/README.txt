# Installation:
# You may need to modify CMakeLists.txt (add below lines)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

How to run MTASSimulation_XML with different number of input files:

1) No input file (e.g. in Linux terminal "$ ./MTASSimulation_XML") - default macro file 'vis.mac' will be exectuted. 'vis.mac' contains visualization options.
2) One input file (e.g. 'input.mac') is treated as macro file instead of default 'vis.mac' - 'vis.mac' won't be executed. Use this option for energy specified gammas, betas etc. simulations. Example file 'input.mac' from the GitHub repository contains all necessary parameters for simple simulation.
3) Two input files. First one (e.g. 'input.mac') is treated as macro file, second one (e.g. 'Decay.xml') must be in .xml format and contain information about simulation. Use it to simulate isotopes decay. Second file should have a list of isotopes used in simulated process (NuclideFile elements), specified start and stop levels of the simulation (StartLevel and StopLevel elements). You may learn more about .xml files structure from examples provided in the GitHub repository or 'CreateXML' program.
