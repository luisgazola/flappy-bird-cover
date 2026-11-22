TARGET = flappy
TEST_TARGET = build/core_tests

CXX ?= g++
BUILD_DIR = build

WARNINGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wno-sign-conversion
CXXFLAGS ?= -O2 -g
CXXFLAGS += -std=c++17 $(WARNINGS) -MMD -MP

# SFML instalado fora do sistema (ex.: scripts/build-sfml.sh):
#   make SFML_PREFIX=/caminho/do/prefixo
PKG_CONFIG = pkg-config
ifdef SFML_PREFIX
PKG_CONFIG = PKG_CONFIG_PATH="$(SFML_PREFIX)/lib/pkgconfig:$(SFML_PREFIX)/lib64/pkgconfig" pkg-config
LDFLAGS += -Wl,-rpath,$(SFML_PREFIX)/lib -Wl,-rpath,$(SFML_PREFIX)/lib64
endif

SFML_CFLAGS = $(shell $(PKG_CONFIG) --cflags sfml-graphics)
SFML_LIBS = $(shell $(PKG_CONFIG) --libs sfml-graphics)

CORE_SRC = src/core/Bird.cpp src/core/World.cpp src/core/HighScoreStore.cpp
APP_SRC = src/app/Game.cpp src/main.cpp
TEST_SRC = tests/core_tests.cpp

CORE_OBJ = $(CORE_SRC:%.cpp=$(BUILD_DIR)/%.o)
APP_OBJ = $(APP_SRC:%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

check-sfml:
	@$(PKG_CONFIG) --atleast-version=3.0 sfml-graphics || { \
		echo "Erro: SFML 3 (sfml-graphics >= 3.0) não encontrado pelo pkg-config."; \
		echo "Versão encontrada: $$($(PKG_CONFIG) --modversion sfml-graphics 2>/dev/null || echo nenhuma)"; \
		echo "Instale o SFML 3 (veja 'Instalação' no README) ou compile-o"; \
		echo "com scripts/build-sfml.sh e use make SFML_PREFIX=<prefixo>."; \
		exit 1; }

$(APP_OBJ): | check-sfml

$(TARGET): $(CORE_OBJ) $(APP_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS) $(SFML_LIBS)

# Apenas a camada de apresentação depende de SFML.
$(BUILD_DIR)/src/app/%.o $(BUILD_DIR)/src/main.o: CXXFLAGS += $(SFML_CFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Os testes compilam só o núcleo: não exigem SFML nem janela.
$(TEST_TARGET): $(CORE_OBJ) $(TEST_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(CORE_OBJ:.o=.d) $(APP_OBJ:.o=.d) $(TEST_OBJ:.o=.d)

.PHONY: all test run clean check-sfml
