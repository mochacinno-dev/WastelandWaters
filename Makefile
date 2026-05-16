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

# ── Cross-compile para Windows desde Linux (MinGW) ──
win:
	@mkdir -p build/win build/win/core build/win/mission1 build/win/mission2 build/win/mission3 build/win/mission4 build/win/mission5 build/win/mission6 build/win/mission7 build/win/mission8
	@echo "Compilando para Windows..."
	@for src in $(SRCS); do \
		obj=build/win/$$(echo $$src | sed 's|^src/||; s|\.c$$|.o|'); \
		echo "  CC $$src"; \
		x86_64-w64-mingw32-gcc -std=c99 -Wall -Wextra -O2 -mwindows \
			-I/tmp/raylib-win/raylib-5.0_win64_mingw-w64/include \
			-Isrc/core -c $$src -o $$obj || exit 1; \
	done
	@echo "Enlazando para Windows..."
	@x86_64-w64-mingw32-gcc \
		build/win/main.o \
		build/win/core/trivia_manager.o \
		build/win/mission1/mission1.o \
		build/win/mission2/mission2.o \
		build/win/mission3/mission3.o \
		build/win/mission4/mission4.o \
		build/win/mission5/mission5.o \
		build/win/mission6/mission6.o \
		build/win/mission7/mission7.o \
		build/win/mission8/mission8.o \
		-L/tmp/raylib-win/raylib-5.0_win64_mingw-w64/lib \
		-lraylib -lopengl32 -lgdi32 -lwinmm -lm -static \
		-o build/win/WastelandWaters.exe
	@echo "Compilado: build/win/WastelandWaters.exe"

.PHONY: win

RAYLIB_INC ?= /usr/local/include
RAYLIB_LIB ?= /usr/local/lib

INCLUDES = -I$(RAYLIB_INC) -Isrc/core

SRCS = src/main.c \
       src/core/trivia_manager.c \
       src/mission1/mission1.c \
       src/mission2/mission2.c \
       src/mission3/mission3.c \
       src/mission4/mission4.c \
       src/mission5/mission5.c \
       src/mission6/mission6.c \
       src/mission7/mission7.c \
       src/mission8/mission8.c

OBJS = $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS))

.PHONY: all clean run win

all: $(BUILD)/$(TARGET)$(EXT)

$(BUILD)/$(TARGET)$(EXT): $(OBJS)
	@echo "Enlazando $(TARGET)..."
	$(CC) $(OBJS) -L$(RAYLIB_LIB) $(LIBS) -o $@
	@echo "Compilado: $@"

$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(BUILD)/$(TARGET)$(EXT)

clean:
	rm -rf $(BUILD)

install-raylib-linux:
	sudo apt-get install -y libraylib-dev || \
	(git clone --depth 1 https://github.com/raysan5/raylib.git /tmp/raylib && \
	 cd /tmp/raylib/src && make PLATFORM=PLATFORM_DESKTOP && \
	 sudo make install && cd - && rm -rf /tmp/raylib)