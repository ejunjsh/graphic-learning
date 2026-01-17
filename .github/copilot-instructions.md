# Copilot / AI assistant instructions for graphic-learning

Purpose: quick reference so an AI agent can be productive immediately in this Qt C++ demo app.

- **Big picture**: this is a single-process Qt GUI app with a tabbed main window. Each tab implements one rendering/demo (raytracing, shading, transforms, etc.). Rendering logic lives in `painter.*` and low-level geometry in `vector.h`, `vertex.h`, `triangle.h`, `sphere.h`. Tabs live as `*Tab.cpp/.h` and are instantiated from `mainwindow.cpp`.

- **Key files to read first**:
  - `main.cpp` — app entrypoint.
  - `mainwindow.cpp` — tab wiring (where tabs are added to the UI).
  - `painter.h/cpp` — central render surface and `render()` contract.
  - `config/config.h` — global constants (`CANVAS_WIDTH`, `CANVAS_HEIGHT`).
  - `config/*.h` — per-demo config headers (tweak behavior & presets).
  - One representative tab, e.g. `DiffuseTab.cpp` + `DiffuseTab.h` — shows the typical pattern for a demo tab.

- **Architecture notes / data flows**:
  - UI: `MainWindow` creates tabs and a `Painter` widget; tabs compute pixel buffers and call `Painter::render(Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT])`.
  - Renderer: `Painter` stores the pixel buffer and paints it in `paintEvent()` using `QPainter::drawPoint`.
  - Geometry & math: small hand-rolled math types in `vector.h`, `matrix.h`, `mat4x4.h`, `vertex.h`, `vertex4.h` — many demos operate directly on these plain headers.

- **Build and run (specific commands observed in repo)**
  - Prerequisite: Qt installed (Homebrew command shown in README): `brew install qt`.
  - Typical build flow (from repo root):
    ```bash
    mkdir -p build
    cd build
    qmake ..
    make
    ```
  - On macOS the app bundle is created under `build/graphic-learning.app/` (see generated `build/Makefile`). If Qt is non-standard, ensure `qmake` in PATH (Homebrew: `/opt/homebrew/bin/qmake`).

- **Patterns & conventions to follow**
  - Tab naming: each demo uses `<Name>Tab.h/.cpp` and is added to the UI in `mainwindow.cpp` with the label string.
  - Rendering contract: compute a `Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT]` buffer and pass it to `Painter::render()` — do not attempt to draw directly to the widget outside this contract.
  - Global sizes: use `config/config.h` constants rather than hard-coded numbers for the canvas.
  - Add sources: when adding a new tab/demo, update `graphic-learning.pro` (add .cpp in `SOURCES` and .h in `HEADERS`) and add an include + `tabWidget->addTab(...)` line in `mainwindow.cpp`.

- **Common developer tasks**
  - Add a new demo tab:
    1. Create `MyDemoTab.h/.cpp` following an existing tab as template.
    2. Add to `graphic-learning.pro` under `SOURCES`/`HEADERS`.
    3. Include and instantiate the tab in `mainwindow.cpp`.
    4. Build (`qmake && make`) and run the app bundle in `build/`.
  - Tweak canvas size: edit `config/config.h` and recompile — many files assume `CANVAS_WIDTH`/`CANVAS_HEIGHT` compile-time constants.

- **Integration points & gotchas**
  - MOC files are generated under `build/` (e.g. `moc_*.cpp`). If you change signals/slots, qmake will regenerate MOCs; run `qmake` before `make` if headers change.
  - The UI is created programmatically (no .ui files). Tabs are not registered via a plugin system — `mainwindow.cpp` is the authoritative place to add/remove demos.
  - `cgfs/` contains independent HTML demo pages (educational, not part of the C++ build).

- **Where to look for examples**
  - `DiffuseTab.*` and `SpecularTab.*` demonstrate shading/ray logic using the project's math primitives.
  - `BasicRaysTab.*` demonstrates a simple raytracing flow — good first-edit target.

- **When to ask the maintainer**
  - If you need to change the global canvas API (e.g. change `Painter::render` signature), ask — many tabs assume the current fixed buffer contract.
  - If you plan to introduce non-Qt build tooling (CMake), confirm platform packaging expectations first.

If anything above is unclear or you want different emphasis (tests, CI, or how to run a single tab in isolation), tell me which area to expand.
