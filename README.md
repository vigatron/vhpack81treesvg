# vhpack81treesvg

### Huffman Tree / TCode / Spectrum → SVG visualizer

`vhpack81treesvg` is a small C++17 experimental tool for building, transforming and visualizing binary coding trees.

The project was created as a development and analysis utility around the `VHPack81` / `VHPack` data representation and related tree structures.

The main idea is simple:

```text
             input
               |
       +-------+-------+
       |               |
     TCode          Spectrum
       |               |
       +-------+-------+
               |
          tree builder
               |
          Huffman tree
               |
       SCode / node map
               |
       layout / statistics
               |
             SVG
```

The program does not try to be a general-purpose graph drawing framework.

It is a specialized engineering tool where the **internal tree representation is more important than the final picture**.

---

## 1. Project status

This is an experimental / research-oriented project.

It is primarily intended for:

* compression research;
* inspection of coding trees;
* analysis of TCode structures;
* generation of tree diagrams;
* comparison of trees generated from different inputs;
* debugging of tree transformations;
* investigation of symbol distribution and coding depth;
* producing SVG documentation of internal data structures.

The code is intentionally relatively direct.

There is no GUI.

There is no external graph-layout engine.

There is no dependency on a large graphics library.

SVG is generated directly as text.

---

# 2. What is being visualized?

The important object in this project is a binary coding tree.

A coding tree can be represented in several ways.

For example, a conventional binary tree can be viewed as:

```text
                    ROOT
                   /    \
                 /        \
               N0          N1
              /  \        /  \
             A    B      C    D
```

The same tree can be represented by paths:

```text
A = 00
B = 01
C = 10
D = 11
```

or by a compact structural description.

The project works with several such representations.

The distinction between the representations is important.

The tree itself is one object.

`TCode`, `SCode`, symbol numbering and node numbering are different representations of that object.

The program therefore contains explicit conversion stages instead of treating the input string as the tree itself.

---

# 3. Main processing modes

The executable supports three principal generation modes.

### TCode mode

A TCode representation is decoded into the internal tree architecture.

```text
TCode
  |
  v
SCode
  |
  v
tree architecture
  |
  v
layout
  |
  v
SVG
```

### Spectrum mode

A frequency / spectrum array is used to construct a Huffman tree.

```text
symbol frequencies
       |
       v
Huffman tree
       |
       v
SCode
       |
       v
layout
       |
       v
SVG
```

### MIX mode

TCode and spectrum information are used together.

This mode is useful when the structural coding representation and the symbol distribution need to be inspected simultaneously.

The program selects the processing mode automatically from the available input parameters unless an explicit mixed mode is requested.

---

# 4. Internal architecture

The source tree is divided into several functional areas.

```text
src/
├── calc/
├── draw/
├── parser/
├── global/
├── generate/
├── mods/
│   ├── colors/
│   ├── layerarea/
│   ├── strtools/
│   ├── svg/
│   └── tree/
└── main.cpp
```

The separation is intentional.

The tree model should not depend on the drawing implementation.

The SVG generator should not need to know how a Huffman tree was produced.

The parser should not contain layout code.

The generator layer connects these parts.

---

# 5. `main.cpp`

The executable entry point is intentionally small.

The main processing sequence is approximately:

```text
Parse command line
      |
      v
Validate input
      |
      v
Select generation mode
      |
      +---- TCode
      |
      +---- Spectrum
      |
      +---- MIX
      |
      v
Generate tree / graphics
      |
      v
Save SVG
```

The run-mode selection is handled before generation.

If neither TCode nor spectrum data is available, processing stops with an input error.

After successful generation the resulting SVG is written to the requested output file.

This keeps the executable itself mostly as a dispatcher.

---

# 6. VHTree

The central class is:

```cpp
class VHTree
```

It is the main bridge between coding data and the internal tree architecture.

Two important entry points are:

```cpp
buildFromTCode(...)
```

and

```cpp
buildFromSpectrum(...)
```

The first reconstructs a tree from an encoded TCode representation.

The second creates a tree from symbol frequencies.

---

# 7. Building from TCode

The TCode path starts by parsing the TCode representation.

Conceptually:

```text
TCode
  |
  v
TCode parser
  |
  v
SCode nodes
  |
  v
automatic enumeration
  |
  v
tree links
  |
  v
depth calculation
  |
  v
layer calculation
```

The internal node representation contains:

```cpp
struct stnode
{
    int id;
    int tt;
};
```

where:

* `id` identifies the node;
* `tt` describes the node type.

The tree architecture is then populated using the node type.

Different node types describe different left/right traversal cases.

This is why the TCode representation can remain compact while the internal representation can contain explicit parent/child relationships.

---

# 8. Building from a spectrum

The spectrum path is different.

Instead of reconstructing an existing tree, the program creates a tree from symbol frequencies.

The input is essentially:

```text
symbol 0 -> frequency
symbol 1 -> frequency
symbol 2 -> frequency
...
symbol N -> frequency
```

The tree builder creates the corresponding Huffman architecture.

After that the tree is converted to the project's SCode representation.

This provides an important analysis capability:

```text
frequency distribution
        |
        v
   Huffman tree
        |
        v
 coding structure
        |
        v
 visual representation
```

Therefore the SVG output can be used to inspect how the frequency distribution affects the resulting coding tree.

---

# 9. Huffman tree representation

The internal architecture stores explicit tree relationships.

Typical operations include:

```cpp
getleft()
getrigh()
getlay()
getswap()
```

and operations for setting / linking nodes.

The tree also stores information about node depth and layer.

This allows the project to treat the tree as more than a simple recursive structure.

It can be inspected as an indexed node array.

For example:

```text
node #0
node #1
node #2
...
node #N
```

with explicit relationships:

```text
node -> left child
node -> right child
node -> parent
node -> layer
node -> type
```

This representation is convenient for compression research because node numbering and symbol numbering can be manipulated independently.

---

# 10. Tree depth and layers

After construction, the tree depth is calculated.

Layer information is also stored.

Conceptually:

```text
                    0
                   / \
                  /   \
                 1     1
                / \   / \
               2   2 2   2
```

The layer information is later useful for drawing.

It also allows the tree to be inspected numerically.

For example, the program can enumerate nodes belonging to a particular layer.

This is useful when studying:

* maximum code depth;
* tree balance;
* symbol distribution;
* code length;
* pathological trees;
* effects of tree rotation.

---

# 11. Tree rotation

The internal architecture supports tree rotation.

Rotation is not merely a cosmetic SVG operation.

It changes the left/right orientation of nodes while preserving the fundamental tree structure.

This can be useful when the same coding tree needs to be represented in an alternative canonical orientation.

The generation functions therefore accept a rotation option.

Conceptually:

```text
original tree

       N
      / \
     A   B


rotated representation

       N
      / \
     B   A
```

The exact operation is performed at the tree architecture level before the drawing stage.

This is important because the SVG renderer should only visualize the resulting structure.

---

# 12. SCode

`SCode` is one of the important intermediate representations used by the project.

It is stored as a vector of:

```cpp
stnode
```

objects.

An SCode record therefore contains both:

```text
node identity
node type
```

rather than only a stream of bits.

This makes SCode suitable as a structural intermediate representation.

The project also contains conversion routines between the structural representation and textual / binary forms.

For example:

```cpp
SCodeToText(...)
SCodeToTCode(...)
```

The textual representation is useful during development because it can be dumped directly to the console.

---

# 13. SCode → TCode packing

The project contains a compact conversion path which packs node types into a byte-oriented representation.

The implementation uses two bits per tree-node type.

Therefore eight 2-bit values can occupy one byte:

```text
7 6 5 4 3 2 1 0
+-----+-----+-----+-----+
|  N7 |  N6 |  N5 |  N4 |
+-----+-----+-----+-----+
```

where every `N` represents a 2-bit node type.

The resulting byte stream is converted to hexadecimal for textual output.

This is one of the places where the project is clearly oriented toward compact machine representations rather than only visualization.

---

# 14. Symbol re-indexing

When a Huffman tree is traversed, the project maintains a re-indexing table.

This is exposed through:

```cpp
reidxtbl()
```

The reason is that there are several numbering domains involved:

```text
tree node index
symbol index
generated node index
serialized node order
```

They are not necessarily identical.

The re-indexing table therefore provides the mapping required to move between the internal tree representation and the coding representation.

This is particularly important when the tree is serialized or compared with another tree.

---

# 15. Bit paths

A symbol/node can also be represented by its binary path from the root.

The project provides:

```cpp
bitpath(int idx)
```

Conceptually:

```text
              ROOT
             /    \
            0      1
           / \    / \
          00 01  10 11
```

A node therefore has a binary code corresponding to the sequence of left/right decisions from the root.

The implementation walks from the selected node toward the root and reconstructs the path in reverse order.

This is the direct relationship between:

```text
tree topology
      |
      v
left/right decisions
      |
      v
binary code
```

---

# 16. Symbol rate estimation

The tree class also contains a small helper for estimating a symbol's relative coding cost.

The calculation is based on:

```text
code path length
----------------
symbol frequency
```

and is reported as a percentage-like value.

This is not intended to replace a full entropy analysis.

It is a visualization / diagnostic metric.

The useful part is that the information can be placed next to the corresponding tree node in generated graphics.

---

# 17. SVG generation

The project does not use an external SVG library.

Instead it contains a small SVG writer:

```cpp
class VHSVG
```

The writer accumulates SVG fragments as strings and writes them to a file.

The interface provides primitive operations such as:

```cpp
line()
circ()
arc()
path()
rect()
text()
```

This is deliberately simple.

For this project, SVG is primarily an output serialization format.

It is not treated as a graphics programming framework.

---

# 18. Why SVG?

SVG is useful here for several reasons.

### Vector output

Tree diagrams remain scalable.

A generated image can be enlarged without rasterization artifacts.

### Text remains text

Node identifiers, layer numbers and statistics can remain actual SVG text elements.

### Easy inspection

The generated file can be opened directly in:

* web browsers;
* vector graphics editors;
* documentation systems;
* SVG-capable image viewers.

### Simple generation

No image encoder is required.

The application only needs to produce XML-like SVG elements.

---

# 19. SVG primitives

The internal SVG writer provides basic primitives.

For example:

```text
line
circle
arc
path
rectangle
text
```

A line can be generated as:

```xml
<line
    x1="..."
    y1="..."
    x2="..."
    y2="..."
    stroke="..."
/>
```

Similarly, tree branches can be represented using ordinary SVG paths.

This keeps the renderer independent from the operating system and graphics hardware.

---

# 20. Rendering architecture

The conceptual rendering pipeline is:

```text
             TREE
              |
       +------+------+
       |             |
     nodes        topology
       |             |
       +------+------+
              |
           layout
              |
       +------+------+
       |             |
     geometry     annotations
       |             |
       +------+------+
              |
             SVG
```

The tree calculation and the SVG representation are separate stages.

This separation is useful when modifying the project.

For example, a different output format can theoretically be added without changing the Huffman tree construction code.

---

# 21. Console diagnostics

The project intentionally contains a significant amount of diagnostic output.

Examples include:

```text
SCode generated from Huffman
AUTOENUM Enter
AUTOENUM Exit
node / layer information
left / right relationships
```

This is useful during algorithm development.

The console output effectively acts as a second representation of the internal tree.

Therefore there are two debugging views:

```text
machine-oriented:
    node arrays / indices / types

human-oriented:
    console tree dumps
```

The SVG output is a third view.

---

# 22. Development philosophy

The project follows a relatively low-level development model.

The important objects are represented explicitly.

Instead of hiding the complete tree inside a generic library, the implementation exposes:

* node indices;
* node types;
* left/right links;
* layers;
* depth;
* symbol mapping;
* re-indexing;
* coding paths.

This makes the code somewhat verbose in places.

That is intentional.

When developing compression algorithms, the ability to inspect an intermediate representation is often more important than reducing source-code size.

---

# 23. Example conceptual workflow

A typical analysis can be performed as follows:

```text
1. Start with symbol statistics

       A : 50
       B : 25
       C : 15
       D : 10

2. Build Huffman tree

              ROOT
             /    \
            A      N
                  / \
                 B   N
                    / \
                   C   D

3. Calculate depths

       A -> 1
       B -> 2
       C -> 3
       D -> 3

4. Generate SCode

5. Calculate node layers

6. Generate SVG

7. Inspect the resulting tree
```

The exact tree depends on the implementation and tie-breaking rules used by the tree builder.

The important point is that the project allows the complete process to be inspected rather than treating the Huffman encoder as a black box.

---

# 24. Input / output concept

The application is designed around command-line parameters.

The executable first parses the supplied parameters.

The parser determines whether the invocation contains:

```text
TCode input
SPC input
MIX parameters
output filename
```

The exact command-line interface is defined by the parser implementation and may evolve with the experimental code.

For this reason, the executable's own usage output should be considered the authoritative command reference.

---

# 25. Building

Requirements:

* CMake 3.16 or newer;
* C++17-compatible compiler.

The project uses:

```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

and builds a single executable:

```text
vhpack81treesvg
```

Typical Linux build:

```bash
git clone https://github.com/vigatron/vhpack81treesvg.git
cd vhpack81treesvg

mkdir build
cd build

cmake ..
cmake --build . -j
```

The resulting executable is:

```text
vhpack81treesvg
```

---

# 26. Build structure

CMake collects the project source files recursively.

The include paths expose the main internal modules:

```text
src
src/calc
src/draw
src/parser
src/global
src/generate

src/mods
src/mods/colors
src/mods/strtools
src/mods/layerarea
src/mods/tree
src/mods/svg
```

Compilation commands can also be exported through:

```cmake
CMAKE_EXPORT_COMPILE_COMMANDS
```

which is convenient for IDEs and static-analysis tools.

---

# 27. Project tree

A simplified project structure is:

```text
vhpack81treesvg/
│
├── CMakeLists.txt
│
├── docs/
│
├── src/
│   │
│   ├── main.cpp
│   │
│   ├── calc/
│   ├── draw/
│   ├── parser/
│   ├── global/
│   ├── generate/
│   │
│   └── mods/
│       ├── colors/
│       ├── layerarea/
│       ├── strtools/
│       ├── svg/
│       └── tree/
│
└── .vscode/
```

The `mods/tree` module contains the core tree representation.

The `mods/svg` module contains the SVG output layer.

The remaining modules provide parsing, calculation, generation, drawing and utility functionality.

---

# 28. Design notes

There are several concepts which should not be mixed together.

### Tree

The logical coding structure.

### Node

An element of the tree.

### Symbol

A value represented by a leaf.

### Code path

The left/right path from root to leaf.

### TCode

A compact coding-oriented representation.

### SCode

A structural representation used internally by the project.

### Spectrum

The frequency information used to construct a Huffman tree.

### SVG

The final visualization format.

Keeping these concepts separate makes the source easier to reason about.

---

# 29. Why not simply draw a Huffman tree?

Because the purpose of the project is not only drawing.

A generic Huffman visualization tool would normally do:

```text
frequencies -> Huffman tree -> picture
```

This project goes further:

```text
                 +----------------+
                 |   TCode        |
                 +-------+--------+
                         |
                         v
                 +----------------+
                 |    SCode       |
                 +-------+--------+
                         |
                         v
+-------------+   +----------------+   +-------------+
|  Spectrum   +-->| Tree Architecture|-->| Re-indexing|
+-------------+   +--------+-------+   +-------------+
                          |
                          v
                     layer/depth
                          |
                          v
                       geometry
                          |
                          v
                         SVG
```

The intermediate representations are therefore part of the actual subject of the project.

---

# 30. Compression research context

The tree representation is particularly useful when studying custom compression systems.

For a conventional compressor, the tree is often an internal implementation detail.

Here it can be inspected directly.

This makes it possible to ask questions such as:

```text
How many nodes are required?

How deep is the tree?

How many symbols exist at each layer?

Which symbols receive short paths?

How does the frequency distribution affect the tree?

Does tree rotation change the serialized representation?

How does re-indexing affect the resulting code?

How compact is the resulting structural representation?
```

These are engineering questions rather than purely graphical questions.

---

# 31. Tree statistics

The project exposes enough internal information to derive several useful statistics.

For example:

```text
number of symbols
number of nodes
maximum depth
node count per layer
symbol code length
symbol frequency
relative code cost
```

A future analysis tool could easily export these values as CSV or JSON.

At present the project is primarily oriented toward console diagnostics and SVG visualization.

---

# 32. Determinism

For compression research, deterministic tree construction is important.

If the same spectrum is processed repeatedly, the resulting tree should be reproducible under the same implementation rules.

This matters because otherwise:

```text
same input
   |
   +--> tree A
   |
   +--> tree B
```

would make binary comparison and benchmarking difficult.

When modifying the tree-building code, tie-breaking behavior should therefore be treated as part of the algorithm, not merely as an implementation detail.

---

# 33. Debugging strategy

The project is convenient to debug from the inside out.

A useful sequence is:

```text
1. Verify input parser.

2. Print TCode / spectrum.

3. Verify SCode.

4. Verify node types.

5. Verify left/right links.

6. Verify layers.

7. Verify bit paths.

8. Verify re-indexing.

9. Verify generated SVG.
```

If the final SVG is wrong, the SVG renderer should not be the first place to look.

Usually the error can be localized much earlier in the pipeline.

---

# 34. Experimental nature

This repository should be considered engineering / research code rather than a polished reusable library.

Some parts intentionally expose implementation details.

Some diagnostic code is still present.

The API may change.

The internal representations may change.

The SVG format may change.

The command-line interface may change.

The primary objective is to explore the representation and make the result visible.

---

# 35. Possible future directions

Possible extensions include:

```text
- SVG legend generation
- interactive HTML/SVG viewer
- automatic tree statistics
- entropy calculation
- average code length
- maximum code depth analysis
- canonical Huffman comparison
- binary TCode import/export
- SCode import/export
- JSON tree export
- CSV statistics export
- side-by-side tree comparison
- tree diff visualization
- configurable SVG themes
- configurable layout algorithms
```

Another useful extension would be a machine-readable intermediate dump:

```text
tree.json
```

containing:

```json
{
    "nodes": [],
    "layers": [],
    "symbols": [],
    "depth": [],
    "reindex": []
}
```

This would allow external analysis programs to work with the exact same tree generated by the C++ implementation.

---

# 36. Intended use

The project can be useful when working on:

* custom compression algorithms;
* Huffman coding;
* binary tree serialization;
* compact coding tables;
* FPGA-oriented data representations;
* embedded decompression;
* experimental archival formats;
* algorithm visualization;
* reverse engineering of coding structures.

It is especially useful when the coding structure itself is part of the research.

---

# 37. Non-goals

This project is not intended to be:

* a complete compression library;
* a replacement for mature Huffman implementations;
* a generic graph visualization package;
* a GUI application;
* a production SVG framework;
* a complete archival format implementation.

It is a focused development tool.

---

# 38. Author / project identity

The project belongs to the `vigatron` development environment.

The author's GitHub profile describes the work as a combination of embedded systems, FPGA, PCB design and low-level programming, with C++, VHDL and Verilog among the primary technologies.

The nickname associated with the development work is:

```text
V01G04A81
```

The code style reflects this hardware/software boundary:

```text
data representation
        |
        v
algorithm
        |
        v
compact structure
        |
        v
machine representation
        |
        v
visual inspection
```

The visualization is therefore not an isolated graphics feature.

It is a debugging window into the data structure.

---

# 39. License

See the repository for the current license information.

If this project is reused in another compression / visualization system, preserve the original project attribution and check the repository license before redistribution.

---

# 40. Final note

The most important part of `vhpack81treesvg` is not the SVG output.

The SVG is the visible end of the process.

The actual engineering object is the chain:

```text
TCode / Spectrum
       |
       v
   Tree model
       |
       v
   SCode model
       |
       v
 Node numbering
       |
       v
  Re-indexing
       |
       v
  Bit paths
       |
       v
 Layer / depth
       |
       v
   SVG drawing
```

This makes the project useful as a laboratory for examining how a compact coding structure is transformed between different representations.

The source intentionally keeps these stages visible.

That is the point.

---

## V01G04A81

```text
hardware  ->  representation  ->  algorithm
                         |
                         v
                    visualization
```

`vhpack81treesvg` is a small tool for making the invisible parts of a coding algorithm visible.

(C) 2025-2026 Viktor Glebov / V01G04A81
