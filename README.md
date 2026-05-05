# 🎲 Spinning Cube - Terminal Animation

A mesmerizing 3D rotating cube animation rendered entirely in your terminal using ASCII characters. Watch a wireframe cube spin in 3D space with realistic perspective and lighting effects.

![Cube Preview](3dgridcube.png)

## ✨ Features

- **Real-time 3D Rotation**: Cube rotates smoothly on all three axes
- **Perspective Projection**: Proper 3D-to-2D projection with perspective scaling
- **Depth Buffering**: Z-buffer implementation ensures correct face visibility
- **Lighting System**: Dot product-based luminance calculation with brightness mapping
- **ASCII Art Rendering**: Different characters for each face with shading
- **Terminal Rendering**: Works in any standard terminal (160×40 characters)
- **Smooth Animation**: ~60 FPS animation loop

## 🚀 Quick Start

### Prerequisites
- GCC compiler
- POSIX-compliant terminal
- Linux/macOS (or Windows with WSL)

### Compilation

```bash
gcc main.c -lm -o spinning-cube
```

The `-lm` flag links the math library (required for trigonometric functions).

### Running

```bash
./spinning-cube
```

Press `Ctrl+C` to stop the animation.

## 🎨 How It Works

### Core Algorithm

1. **Rotation Matrix Computation**: Converts 3D points using Euler angles (A, B, C)
   - Angles increment each frame for smooth rotation
   - Precomputed sin/cos values for efficiency

2. **Projection**: Projects 3D coordinates to 2D screen space
   - Uses perspective division (1/Z scaling)
   - K1 and K2 are scaling constants for proper sizing

3. **Lighting**: Normal vector illumination
   - Dot product with light direction determines brightness
   - Maps intensity to ASCII character set: `.,-~:;=!*#$@`

4. **Z-Buffering**: Maintains depth information
   - Only renders closest surface at each pixel
   - Prevents overlapping face artifacts

### 3D Transformation

The code performs a series of 3D rotations:
- **Rotation matrices** for all three axes are combined
- **Normal vectors** are also rotated to maintain correct lighting
- **Depth scaling** creates the perspective effect

### ASCII Character Mapping

Each face uses different characters for visual distinction:
- **Front**: `@`
- **Back**: `$`
- **Left**: `~`
- **Right**: `#`
- **Top**: `;`
- **Bottom**: `+`

Brightness varies based on surface normal angle to the light source.

## 📐 Mathematical Concepts

### Projection Formula
```
x_screen = (x_rotated / z_rotated) * K1 * 2 + WIDTH/2
y_screen = (y_rotated / z_rotated) * K1 + HEIGHT/2
```

### Luminance (Lighting)
```
L = normal_x * light_x + normal_y * light_y + normal_z * light_z
```

### Rotation Transformations
Full 3D rotation matrix combining rotations around X, Y, and Z axes using precomputed sin/cos values.

## ⚙️ Configuration

You can customize the animation by modifying these constants in `main.c`:

- `WIDTH` (160): Terminal width in characters
- `HEIGHT` (40): Terminal height in characters
- `S` (30): Half the cube side length
- `STEP` (0.5): Point density on each face (smaller = denser)
- `K1` (100), `K2` (250): Scaling constants
- `A`, `B`, `C` increment values (lines 161-163): Rotation speed

## 📚 Technical Details

- **Language**: C
- **Dependencies**: Standard C library + math.h
- **Performance**: ~60 FPS on modern hardware
- **Memory**: ~66KB for buffers (160×40 = 6400 characters + 6400 floats)
- **Rendering Method**: Direct terminal manipulation with ANSI escape codes

## 🔧 Customization Ideas

- Change rotation speeds for faster/slower animation
- Modify the cube size by adjusting `S`
- Experiment with different light directions
- Add different ASCII character sets for artistic effects
- Implement keyboard controls for interactive rotation

## 📖 References

- 3D Graphics Projection: Perspective transformation
- Z-Buffer Algorithm: Depth testing
- Lighting Models: Phong/Gouraud shading basics
- Terminal Graphics: ANSI escape codes

## 🎓 Educational Value

This project demonstrates:
- 3D computer graphics fundamentals
- Matrix transformations and linear algebra
- Depth sorting and rendering pipelines
- Trigonometric rotations
- Terminal/ASCII art programming

## 📝 License

Open for learning and modification.

---

**Created by**: Kemas Nayar  
**Animation Speed**: ~60 FPS  
**Build**: `gcc main.c -lm -o spinning-cube && ./spinning-cube`
