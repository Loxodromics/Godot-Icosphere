# Icosphere Generator for Godot

## Overview

This project provides a custom `Icosphere` generator as a module for the Godot 4 Engine. It allows for the creation of icospheres with adjustable radius and subdivision levels directly within Godot, complete with UV mapping and vertex colors support. It is written in C++ as a Godot Module.

## Features

- **Customizable Radius**: Adjust the size of the icosphere to fit your scene.
- **Subdivision Levels**: Control the detail level of the icosphere with subdivisions.
- **UV Mapping**: Automatic UV mapping for easy texture application.
- **Vertex Colors**: Supports vertex coloring for additional visual effects.
- **Dynamic Mesh Generation**: Generate and update icosphere meshes at runtime.

## Usage

1. Add an `Icosphere` node to your scene.
2. Adjust the `Radius` and `Subdivisions` properties in the inspector to customize your icosphere.
3. Optionally, apply a material to the `Icosphere` for texturing or further visual customization.
