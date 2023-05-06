SFML_LIB_DIR = ../SFML-2.5.1/lib
SFML_INCLUDE_DIR = ../SFML-2.5.1/include

BIN_NAME = psim.exe

CC = g++
O_DIR = obj/
SRC_DIR = src/

CPP_FILES = main.cpp \
            particle.cpp \
            rule.cpp \
            statusbar.cpp \
            simulationhandler.cpp

O_FILES   = $(addsuffix .o,$(addprefix $(O_DIR),$(basename $(CPP_FILES))))
O_FILES  += $(O_DIR)fontData.o

D_FILES   = $(addsuffix .d,$(addprefix $(O_DIR),$(basename $(CPP_FILES))))

all: $(BIN_NAME)

clean:
	rm -rf $(O_DIR)*
	rm -rf $(BIN_NAME)

test: all
	./$(BIN_NAME)

LINKER_FLAGS = -DSFML_STATIC \
               -mwindows \
               -O3 \
               -static \
               -static-libgcc \
               -static-libstdc++

LIB_FLAGS = -L$(SFML_LIB_DIR) \
            -lsfml-graphics-s \
            -lsfml-window-s \
            -lsfml-system-s \
            -lopengl32 \
            -lwinmm \
            -lgdi32 \
            -Wl,-Bstatic \
            -lfreetype \
            -lbrotlidec \
            -lbrotlicommon \
            -lz \
            -lbz2 \
            -lfmt

WARNING_FLAGS = -Wall \
                -Wextra \
                -pedantic \
                -Wnon-virtual-dtor \
                -Wold-style-cast \
                -Wcast-align \
                -Wunused \
                -Woverloaded-virtual \
                -Wpedantic \
                -Wconversion \
                -Wsign-conversion \
                -Wmisleading-indentation \
                -Wduplicated-cond \
                -Wduplicated-branches \
                -Wlogical-op \
                -Wnull-dereference \
                -Wuseless-cast \
                -Wdouble-promotion \
                -Wformat=2 \
                -Wimplicit-fallthrough

$(BIN_NAME) : $(O_FILES)
	@echo Link $@
	@$(CC) $(LINKER_FLAGS) -o $@ $^ $(LIB_FLAGS)

COMPILER_FLAGS = -std=gnu++23 \
                 -DSFML_STATIC \
                 -MMD \
                 -Wall \
                 -Wextra \
                 -O3 \
                 $(WARNING_FLAGS)

INCLUDE_FLAGS = -isystem$(SFML_INCLUDE_DIR)

$(SRC_DIR)statusbar.cpp : $(O_DIR)fontData.c

# Generate font date file
$(O_DIR)fontData.c $(O_DIR)fontData.o &: res/fonts/ShareTechMono-Regular.ttf
	xxd -i $< > $(O_DIR)fontData.c
	$(CC) -c $(O_DIR)fontData.c -o $(O_DIR)fontData.o

# generic .cpp to .o rule
define cpp_to_o

$(O_DIR)$(basename $(Item)).o : $(SRC_DIR)$(Item)
	@echo Compile $$@
	@$(CC) $(INCLUDE_FLAGS) $(COMPILER_FLAGS) -c $$< -o $$@

endef

$(foreach Item,$(CPP_FILES),$(eval $(cpp_to_o)))

-include $(D_FILES)
