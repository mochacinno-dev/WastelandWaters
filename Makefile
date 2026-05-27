# ──────────────────────────────────────────────────
#  WastelandWaters — Makefile para C + Raylib
#  Soporta: Linux, macOS, Windows (MinGW)
# ──────────────────────────────────────────────────

CC      = gcc
TARGET  = WastelandWaters
BUILD   = build

UNAME := $(shell uname 2>/dev/null || echo Windows)

ifeq ($(OS),Windows_NT)
    EXT    = .exe
    LIBS   = -lraylib -lopengl32 -lgdi32 -lwinmm -lm
    CFLAGS_EXTRA = -mwindows
else ifeq ($(UNAME),Darwin)
    EXT    =
    LIBS   = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm
    CFLAGS_EXTRA =
else
    EXT    =
    LIBS   = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    CFLAGS_EXTRA =
endif

CFLAGS = -std=c99 -Wall -Wextra -O2 $(CFLAGS_EXTRA)

# ── Paths ──────────────────────────────────────────
RAYLIB_INC ?= /usr/include
RAYLIB_LIB ?= /usr/lib

INCLUDES = -I$(RAYLIB_INC) -Isrc/core

SRCS = src/core/main.c \
       src/core/trivia_manager.c \
       src/core/locale.c \
       src/mission1/mission1.c \
       src/mission2/mission2.c \
       src/mission3/mission3.c \
       src/mission4/mission4.c \
       src/mission5/mission5.c \
       src/mission6/mission6.c \
       src/mission7/mission7.c \
       src/mission8/mission8.c

OBJS = $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS))

.PHONY: all clean run win install-deps-arch

all: $(BUILD)/$(TARGET)$(EXT)

$(BUILD)/$(TARGET)$(EXT): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJS) -L$(RAYLIB_LIB) $(LIBS) -o $@
	@echo "Built: $@"

$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(BUILD)/$(TARGET)$(EXT)

clean:
	rm -rf $(BUILD)

# ── Cross-compile for Windows from Linux (MinGW) ──
WIN_INC ?= /tmp/raylib-win/raylib-5.0_win64_mingw-w64/include
WIN_LIB ?= /tmp/raylib-win/raylib-5.0_win64_mingw-w64/lib

win:
	@mkdir -p build/win/core \
	           build/win/mission1 build/win/mission2 build/win/mission3 \
	           build/win/mission4 build/win/mission5 build/win/mission6 \
	           build/win/mission7 build/win/mission8
	@echo "Compiling for Windows..."
	@for src in $(SRCS); do \
		obj=build/win/$$(echo $$src | sed 's|^src/||; s|\.c$$|.o|'); \
		echo "  CC $$src"; \
		x86_64-w64-mingw32-gcc -std=c99 -Wall -Wextra -O2 -mwindows \
			-I$(WIN_INC) -Isrc/core \
			-c $$src -o $$obj || exit 1; \
	done
	@echo "Linking for Windows..."
	@x86_64-w64-mingw32-gcc \
		build/win/core/main.o \
		build/win/core/trivia_manager.o \
		build/win/core/locale.o \
		build/win/mission1/mission1.o \
		build/win/mission2/mission2.o \
		build/win/mission3/mission3.o \
		build/win/mission4/mission4.o \
		build/win/mission5/mission5.o \
		build/win/mission6/mission6.o \
		build/win/mission7/mission7.o \
		build/win/mission8/mission8.o \
		-L$(WIN_LIB) \
		-lraylib -lopengl32 -lgdi32 -lwinmm -lm -static \
		-o build/win/WastelandWaters.exe
	@echo "Done: build/win/WastelandWaters.exe"

# ── Arch Linux helper ──────────────────────────────
install-deps-arch:
	sudo pacman -S --needed raylib gcc make
