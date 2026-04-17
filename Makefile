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

.PHONY: all clean run

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