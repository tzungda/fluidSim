# fluidSim
Fluid grid-based and hybrid solvers with a simple OpenGL viewport to display it

- The fluid solvers are mostly based on this book: [Fluid Engine Development - Doyub Kim](https://www.amazon.ca/Fluid-Engine-Development-Doyub-Kim/dp/1498719929/ref=tmm_hrd_swatch_0?_encoding=UTF8&qid=1662332845&sr=1-1)

- The OpenGL viewport is modified from the sample code(Chap10) of [OpenGL 4 Shading Language Cookbook - David Wolff](https://www.amazon.ca/OpenGL-Shading-Language-Cookbook-high-quality-ebook/dp/B07HXYJ9VT)

- Visual Studio 2017

## Third-party librarie 
- [oneTBB](https://github.com/oneapi-src/oneTBB)
- [GLFW](https://www.glfw.org/)
- [glm](https://github.com/g-truc/glm)

## Build steps:
1. Unzip the libs([GLFW](https://www.glfw.org/) and [glm](https://github.com/g-truc/glm)) in /particleDisplay/external
2. In /fluidSimLib/CMakeLists.txt modify the tbb lib paths to the ones match to the Visual Studio version. 
3. In /particleDisplay/CMakeLists.txt modify the glfw lib path according to the Visual Studio version, eg. ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-4.3.8.bin.WIN64/lib-vc2022.
5. Open a Command Prompt and go to the /particleDisplay/external then run the cmake command, eg. cmake -G "Visual Studio 17 2022" -A x64, to generate the VS projects and solutions.
6. Open the particleDisplay solution and compile.

## Next
Rewrite part of the computation to CUDA.

## Images
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0045.png)
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0051.png)

## History
- 2022/09/21: Improved performance with OpenMP
- 2022/09/26: Parallelized the loops in gridFractionalSinglePhasePressureSolver3 and fdmLinearSystem3 with [oneTBB](https://github.com/oneapi-src/oneTBB).
