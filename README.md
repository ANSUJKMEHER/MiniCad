# MiniCAD Web Engine

A full-stack, high-performance 2D CAD Geometry Engine with a modern web interface, powered entirely by a C++ REST API Backend. 

This project was built to demonstrate proficiency in **C++**, **Backend Architecture**, **Object-Oriented Programming (OOP)**, and **Vector Mathematics**.

## ✨ Features
- **Pure C++ Backend**: Uses a lightweight, single-header C++ web server (`cpp-httplib`) to process geometric payloads.
- **RESTful Architecture**: The C++ engine exposes an API endpoint (`POST /api/geometry`) that consumes and parses JSON using `nlohmann/json`.
- **Advanced Geometry Math**: Capable of calculating distances and algebraic intersections (e.g., quadratic line-circle intersections, determinant-based line-line intersections).
- **SVG Generation Engine**: The backend mathematically plots the intersections and dynamically serializes the C++ objects into an SVG string.
- **Premium Frontend UI**: A clean, "glassmorphic" interface built with pure Vanilla HTML/CSS/JS (no heavy frameworks), showcasing core web knowledge.

## 🛠️ Technology Stack
- **Backend**: C++17, `cpp-httplib`, `nlohmann/json`
- **Frontend**: Vanilla JavaScript, HTML5, CSS3
- **Build System**: CMake

## 🚀 How to Run Locally

### Prerequisites
- A modern C++17 capable compiler (GCC, Clang, or MSVC).
- CMake (version 3.10+).

### 1. Build and Start the C++ Backend
```bash
# Clone the repository
git clone https://github.com/ANSUJKMEHER/MiniCad.git
cd MiniCad

# Build the C++ server
mkdir build && cd build
cmake ..
cmake --build .

# Run the backend server (starts on http://localhost:8080)
./MiniCADServer.exe
```

### 2. Start the Frontend
In a new terminal window, navigate to the `frontend` directory and start a lightweight web server:
```bash
cd frontend
python -m http.server 3000
# Alternatively, you can use: npx serve
```

### 3. Test the App
Open your web browser and navigate to `http://localhost:3000`. Add geometry via the sidebar, click "Render", and watch the C++ backend calculate intersections and return the generated SVG instantly!

## 📐 Architecture
The geometry logic uses strict Object-Oriented principles.
- `Shape` (Abstract Base Class) defines polymorphism for `translate`, `rotate`, `scale`, and `toSVG()`.
- `GeometryMath` namespace segregates the algebraic intersection and distance algorithms away from the data models.
- `SVGExporter` aggregates standard library smart pointers (`std::shared_ptr`) of geometry and manages the document structure.
