This is the repository of the CubeCounter project. CubeCounter is an open source project for people to visualize and calculate the number of combinations on an NxNxN Rubik's Cube. As the number of combinations (even on the classic 3x3) may exceed the limit of a 64 bit integer by several orders of magnitude, special libraries are needed. CubeCounter not only uses a powerful library (GMP) to calculate massive numbers, but it also provides a visual representation of a Rubik's Cube of that size (which can be extremely hard to replicate physically). 



**IF YOU OWN A CPU THAT CONTAINS Zen 5 CORES, PLEASE READ BELOW:**

It is stated on the GMP webpage that Ryzen 9000 series processors may be affected by their library, and have reported that some of their processors have died. Please exercise caution (or even better, modify this project on your computer) to prevent damage. 

In the CMakePresets.json, you may modify the top preset named "vcpkg-main" to not use assembly instructions (which may have caused this problem). However, this may result in slower performance. THIS HAS NOT BEEN TESTED BY CUBECOUNTER, so it is highly recommended that you find workarounds (such as replacing the library) to safely execute the code on your processor. 

