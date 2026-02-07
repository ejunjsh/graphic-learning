Icon files and integration

- File added: assets/icon.svg — a scalable SVG icon (512×512 viewBox).

How to use in this Qt project

- Quick: reference the SVG directly in your UI or load it as a QIcon from the file system.
  Example in code:

  - QIcon icon("assets/icon.svg");
  - window->setWindowIcon(icon);

- To embed in a Qt resource file (.qrc):
  1. Create a `resources.qrc` and add `assets/icon.svg`.
  2. Add `RESOURCES += resources.qrc` to `graphic-learning.pro`.

- macOS .icns (optional) — create an iconset and convert:

  ```bash
  mkdir -p icon.iconset
  sips -z 16 16     assets/icon.svg --out icon.iconset/icon_16x16.png
  sips -z 32 32     assets/icon.svg --out icon.iconset/icon_16x16@2x.png
  sips -z 32 32     assets/icon.svg --out icon.iconset/icon_32x32.png
  sips -z 64 64     assets/icon.svg --out icon.iconset/icon_32x32@2x.png
  sips -z 128 128   assets/icon.svg --out icon.iconset/icon_128x128.png
  sips -z 256 256   assets/icon.svg --out icon.iconset/icon_128x128@2x.png
  sips -z 256 256   assets/icon.svg --out icon.iconset/icon_256x256.png
  sips -z 512 512   assets/icon.svg --out icon.iconset/icon_256x256@2x.png
  sips -z 512 512   assets/icon.svg --out icon.iconset/icon_512x512.png
  sips -z 1024 1024 assets/icon.svg --out icon.iconset/icon_512x512@2x.png
  iconutil -c icns icon.iconset
  ```

  (If `sips` doesn't accept SVG directly on your setup, rasterize with `rsvg-convert` or ImageMagick `convert`.)

Notes

- The SVG is intentionally simple and scalable; you can generate PNGs at any size for platform-specific needs.
- If you'd like, I can add the SVG to a `.qrc` and update `graphic-learning.pro` to embed it into the app bundle. Tell me if you want that.