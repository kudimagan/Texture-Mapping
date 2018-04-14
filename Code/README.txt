Running the code:

There are two .cpp files.
textureMappingInbuiltCheckers.cpp will map a white and black checked pattern onto the object of your choice.
textureMappingWithUserTex.cpp will map a user input texture file onto the object of your choice.

Directions to run:

1) Place the object file in the folder Objects.
2) Place the texture file in the folder Textures.
3) Navigate to the folder Code by running cd Code.
4) For textureMappingInbuiltCheckers.cpp, run the commands
       1)g++ -std=c++14 textureMappingInbuiltCheckers.cpp to generate a.out file.
       2)./a.out <width of image - integer> <height of image - integer> <name of obj file> <uv mapping mode - 1 or 2 or 3> <imageName.ppm
> <extra scaling factor - double(adjust as required)>
5) For textureMappingWithUserTex.cpp, run the commands
       1)g++ -std=c++14 textureMappingWithUserTex.cpp to generate a.out file.
       2)./a.out <width of image - integer> <height of image - integer> <name of obj file> <uv mapping mode - 1 or 2 or 3> <textureName.ppm> <imageName.ppm> <extra scaling factor - double(adjust as required)>
