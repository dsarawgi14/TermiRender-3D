# ASCII 3D Scene Framework

This C++ framework allows you to create, render, and animate 3D scenes in the terminal using ASCII art. It supports various 3D objects like cubes, spheres, and donuts and can render them using ray marching techniques. The framework is highly extensible, allowing users to add custom objects and scenes.

## Features

- Render 3D objects like cubes, spheres, and donuts using ASCII art in the terminal.
- Support for ray marching techniques to render complex 3D objects.
- Create custom 3D scenes by combining multiple objects.
- Multithreading support for efficient rendering.
- Animate objects in scenes to create dynamic visual effects.
- Extensible object system that allows for easy integration of new shapes and scenes.

## Directory Structure

The project is organized into the following structure:

```
.
├── build
│   ├── Makefile               # Makefile for building the project
│   └── bin                    # Executable will be placed here after build
├── client
│   ├── rayMarchingDonut.cpp    # Example client code to render a 3D donut
│   ├── rayMarchingDonut.o      # Object file for the donut example
│   └── ignore                 # Ignore folder (intermediate files)
├── include                    # Header files for the framework
│   ├── BringBackCursor.h
│   ├── CameraObject.h
│   ├── CubeObj.h
│   ├── DonutObj.h
│   ├── ObjectCollection.h
│   ├── SDFMixer.h
│   ├── Scene.h
│   ├── SphereObj.h
│   ├── objectClass.h
│   ├── rayMarcher.h
│   ├── renderableObject.h
│   ├── vec.h
│   ├── vecBundle.h
│   └── vecFunctions.h
└── src
    ├── rayMarching            # Source files for ray marching components
    └── vec                    # Vector operations source files
```

## Installation and Setup

1. **Clone the Repository**:

   ```bash
   git clone <repository-url>
   ```

2. **Place Your Client Code**:
   
   - Add your custom scene and object creation logic in the `client/` folder.
   - The provided `rayMarchingDonut.cpp` serves as an example to get started.

3. **Build the Project**:
   
   - Navigate to the `build/` directory:
   
     ```bash
     cd build
     ```
   
   - Run the `make` command to compile the client code:

     ```bash
     make
     ```

   - The executable will be created in the `build/bin/` directory.

4. **Clean the Build**:
   
   - To clean up the generated executables and dependency files, run:

     ```bash
     make clean
     ```

## Usage

Once the project is built, you can run the executable from the `build/bin` directory:

```bash
./bin/<executable-name>
```

### Customizing the Scene

To create your custom scenes:

- Add or modify object creation code in the client folder.
- The framework supports objects like `CubeObj`, `SphereObj`, and `DonutObj`—feel free to extend these or create new objects.
- Use the provided header files in the `include/` folder to leverage pre-built components like `CameraObject`, `Scene`, and `SDFMixer`.

## Example Client Code: `rayMarchingDonut.cpp`

This example demonstrates how to create a dynamic 3D scene featuring a rotating donut and sphere in the terminal using ASCII art.

```cpp
#include "vecBundle.h"
#include "rayMarcher.h"

using namespace std;

// Step function to update object positions and rotations over time
void stepFunc(double increment, double currentTime, ObjectCollection& collection) {
    // Rotate the donut
    collection.donut["Donut 1"].rotX(4 * sin(currentTime) * increment);
    collection.donut["Donut 1"].rotY(2 * cos(currentTime) * increment);
    
    // Move the donut and sphere along the X-axis
    collection.donut["Donut 1"].setPos(vec3(-2 * sin(currentTime * 1.2), 0, 0));
    collection.sphere["Sphere 1"].setPos(vec3(2 * sin(currentTime * 1.2), 0, 0));
}

int main() {
    // Create an object collection with a camera and light source
    ObjectCollection collection;
    collection.camera.setPos({0, 0, 5});
    collection.camera.setU({0, 0, -1});
    collection.camera.setV({-1, 0, 0});
    collection.lightSource = vec3(3, 3, 3);

    // Add a sphere and two donuts to the scene
    collection.sphere["Sphere 1"].setRadius(0.7);
    collection.donut["Donut 1"];
    collection.donut["Donut 2"];

    // Use Smooth Union to combine the objects for rendering
    SDFMixer SphereNDonut(&collection.donut["Donut 1"], &collection.sphere["Sphere 1"]);
    SphereNDonut.setSmoothUnion();
    SDFMixer SphereNDonut2(&collection.donut["Donut 2"], &SphereNDonut);
    SphereNDonut2.setSmoothUnion();

    // Create and initialize the scene with the combined objects
    Scene scene(&SphereNDonut, collection);
    scene.setStepFunction(stepFunc); // Set the step function to animate the scene
    scene.clear();
    scene.startFrameTimer(); // Optional: Start the frame timer to measure frame rates

    // Main render loop
    while (true) {
        scene.render(); // Render the scene in each frame
    }
    return 0;
}
```

This code sets up a scene with two rotating donuts and a sphere, and uses ray marching to render them in the terminal. The `stepFunc` function continuously updates the position and rotation of the objects, creating an animated scene.