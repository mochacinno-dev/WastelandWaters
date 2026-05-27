#!/usr/bin/env bash
# =============================================================================
#  build_web.sh — Compile WastelandWaters to WebAssembly via Emscripten
#
#  Requirements (Arch Linux):
#    yay -S emscripten          # or: paru -S emscripten
#    sudo pacman -S raylib      # only needed for the header; we compile from
#                               # source below so we get the WASM static lib
#
#  Usage:
#    chmod +x build_web.sh
#    ./build_web.sh             # produces build/web/WastelandWaters.html
#    ./build_web.sh --serve     # also launches a local server on :8080
# =============================================================================
set -e

# ---------------------------------------------------------------------------
# 0. Config — tweak these if your paths differ
# ---------------------------------------------------------------------------
RAYLIB_VERSION="5.0"
RAYLIB_SRC_DIR="/tmp/raylib-wasm/raylib-${RAYLIB_VERSION}"
RAYLIB_WASM_LIB="${RAYLIB_SRC_DIR}/src/libraylib.a"

OUT_DIR="build/web"
TARGET="${OUT_DIR}/WastelandWaters"
SHELL_HTML="${RAYLIB_SRC_DIR}/src/minshell.html"

HEAP_SIZE=134217728   # 128 MB — enough for all 8 missions + trivia banks

SRCS=(
    src/core/main.c
    src/core/trivia_manager.c
    src/core/locale.c
    src/mission1/mission1.c
    src/mission2/mission2.c
    src/mission3/mission3.c
    src/mission4/mission4.c
    src/mission5/mission5.c
    src/mission6/mission6.c
    src/mission7/mission7.c
    src/mission8/mission8.c
)

# ---------------------------------------------------------------------------
# 1. Sanity-check: emcc must be on PATH
# ---------------------------------------------------------------------------
if ! command -v emcc &>/dev/null; then
    echo ""
    echo "ERROR: emcc not found."
    echo "Install Emscripten on Arch Linux:"
    echo "  yay -S emscripten or sudo pacman -S emscripten"
    echo "  source /etc/profile.d/emscripten.sh   # or open a new shell"
    exit 1
fi

echo "emcc version: $(emcc --version | head -1)"

# ---------------------------------------------------------------------------
# 2. Build raylib as a WASM static library (only done once)
# ---------------------------------------------------------------------------
if [ ! -f "${RAYLIB_WASM_LIB}" ]; then
    echo ""
    echo ">>> Building raylib ${RAYLIB_VERSION} for WASM..."

    if [ ! -d "${RAYLIB_SRC_DIR}" ]; then
        mkdir -p /tmp/raylib-wasm
        curl -sL "https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz" \
            | tar -xz -C /tmp/raylib-wasm
    fi

    make -C "${RAYLIB_SRC_DIR}/src" \
        PLATFORM=PLATFORM_WEB \
        GRAPHICS=GRAPHICS_API_OPENGL_ES2 \
        CC=emcc AR=emar \
        -j$(nproc)

    echo ">>> raylib WASM library ready: ${RAYLIB_WASM_LIB}"
else
    echo ">>> raylib WASM library already built, skipping."
fi

# ---------------------------------------------------------------------------
# 3. Compile WastelandWaters to WASM
# ---------------------------------------------------------------------------
mkdir -p "${OUT_DIR}"

echo ""
echo ">>> Compiling WastelandWaters..."

emcc -std=c99 -O3 \
    \
    `# ── Include paths ──────────────────────────────────────────` \
    -Isrc/core \
    -I"${RAYLIB_SRC_DIR}/src" \
    \
    `# ── Source files ───────────────────────────────────────────` \
    "${SRCS[@]}" \
    \
    `# ── raylib WASM static lib ─────────────────────────────────` \
    "${RAYLIB_WASM_LIB}" \
    \
    `# ── Emscripten WASM / WebGL flags ──────────────────────────` \
    -sUSE_GLFW=3                    \
    -sASYNCIFY                      \
    -sTOTAL_MEMORY=${HEAP_SIZE}     \
    -sFORCE_FILESYSTEM=1            \
    -sEXPORTED_RUNTIME_METHODS=ccall \
    -sALLOW_MEMORY_GROWTH=0         \
    -sMIN_WEBGL_VERSION=1           \
    -sMAX_WEBGL_VERSION=2           \
    -sMINIFY_HTML=0                 \
    \
    `# ── Shell template (provides the canvas + loading bar) ─────` \
    --shell-file "${SHELL_HTML}"    \
    \
    `# ── Output: .html + .js + .wasm (all in OUT_DIR) ──────────` \
    -o "${TARGET}.html"

echo ""
echo "=========================================="
echo "  Build successful!"
echo "  Output: ${OUT_DIR}/"
ls -lh "${OUT_DIR}/"
echo "=========================================="

# ---------------------------------------------------------------------------
# 4. Optional: serve locally with Python so the browser can load the .wasm
#    (file:// won't work — browsers block SharedArrayBuffer on file://)
# ---------------------------------------------------------------------------
if [[ "$1" == "--serve" ]]; then
    echo ""
    echo ">>> Serving on http://localhost:8080"
    echo "    Open: http://localhost:8080/WastelandWaters.html"
    echo "    Press Ctrl-C to stop."
    cd "${OUT_DIR}"
    python3 -m http.server 8080
fi