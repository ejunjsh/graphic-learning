#!/usr/bin/env bash
set -euo pipefail

# make-macos-icon.sh
# Generates assets/icon.icns from assets/icon.svg
# Tries: rsvg-convert, ImageMagick `convert`, then `qlmanage` as fallback.

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
SVG="$ROOT_DIR/assets/icon.svg"
OUT_ICNS="$ROOT_DIR/assets/icon.icns"
ICONSET="$ROOT_DIR/icon.iconset"

if [ ! -f "$SVG" ]; then
  echo "SVG not found: $SVG"
  exit 1
fi

rm -rf "$ICONSET"
mkdir -p "$ICONSET"

# choose rasterizer
rasterizer=""
if command -v rsvg-convert >/dev/null 2>&1; then
  rasterizer="rsvg-convert"
elif command -v convert >/dev/null 2>&1; then
  rasterizer="convert"
elif command -v qlmanage >/dev/null 2>&1; then
  rasterizer="qlmanage"
else
  echo "No rasterizer found (rsvg-convert, convert, or qlmanage). Install librsvg or ImageMagick." >&2
  exit 2
fi

echo "Using rasterizer: $rasterizer"

# helper to render a size into the iconset with correct filename
render_png() {
  size=$1
  filename=$2
  if [ "$rasterizer" = "rsvg-convert" ]; then
    rsvg-convert -w "$size" -h "$size" "$SVG" -o "$ICONSET/$filename"
  elif [ "$rasterizer" = "convert" ]; then
    convert "$SVG" -background none -resize ${size}x${size} "$ICONSET/$filename"
  else
    # qlmanage: create a temporary png and move
    tmp="$(mktemp -t iconpng).png"
    qlmanage -t -s $size -o "$(dirname "$tmp")" "$SVG" >/dev/null 2>&1 || true
    # qlmanage outputs file with -png suffix in cwd of output; find latest png
    found=$(ls -t "$(dirname "$tmp")"/*.png 2>/dev/null | head -n1 || true)
    if [ -z "$found" ]; then
      echo "qlmanage failed to render PNG" >&2
      exit 3
    fi
    mv "$found" "$ICONSET/$filename"
  fi
}

# generate required sizes
render_png 16  icon_16x16.png
render_png 32  icon_16x16@2x.png
render_png 32  icon_32x32.png
render_png 64  icon_32x32@2x.png
render_png 128 icon_128x128.png
render_png 256 icon_128x128@2x.png
render_png 256 icon_256x256.png
render_png 512 icon_256x256@2x.png
render_png 512 icon_512x512.png
render_png 1024 icon_512x512@2x.png

# build icns
if command -v iconutil >/dev/null 2>&1; then
  iconutil -c icns "$ICONSET" -o "$OUT_ICNS"
  echo "Created $OUT_ICNS"
  rm -rf "$ICONSET"
else
  echo "iconutil not found — cannot build .icns. The iconset remains at $ICONSET" >&2
  exit 4
fi
