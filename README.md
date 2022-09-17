# fluidSim
Fluid grid-based and hybrid solvers with a simple OpenGL viewport to display it

- The fluid solvers are mostly based on this book: Fluid Engine Development - Doyub Kim
[Fluid Engine Development](https://www.amazon.ca/Fluid-Engine-Development-Doyub-Kim/dp/1498719929/ref=tmm_hrd_swatch_0?_encoding=UTF8&qid=1662332845&sr=1-1)

- The OpenGL viewport is based on the sample code(Chap10) of OpenGL 4 Shading Language Cookbook - David Wolff
[OpenGL 4 Shading Language Cookbook](https://www.amazon.ca/OpenGL-Shading-Language-Cookbook-high-quality-ebook/dp/B07HXYJ9VT)

# Buil steps:
1. Unzip the libs in /particleDisplay/external
2. In /particleDisplay/external/CMakeLists.txt modify the glfw lib path according to the Visual Studio version, eg. ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.3.8.bin.WIN64/lib-vc2022.
3. Open a Command Prompt and go to the /particleDisplay/external then run the cmake command, eg. cmake -G "Visual Studio 17 2022" -A x64, to generate the VS projects and solutions.
4. Open the particleDisplay solution and compile.

# Images
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0006.png)
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0018.png)
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0028.png)
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0038.png)
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0045.png)
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0051.png)
![alt text](https://github.com/tzungda/fluidSim/blob/main/outputImages/outputImage.0067.png)
