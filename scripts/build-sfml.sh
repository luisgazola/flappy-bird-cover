#!/usr/bin/env bash
# Compila e instala o SFML 3 em um prefixo local, para distribuições que
# ainda não empacotam a versão 3. Compila apenas os módulos usados pelo jogo
# (system, window e graphics); audio e network ficam desativados.
#
# Uso: scripts/build-sfml.sh [prefixo]   (padrão: ./.deps/sfml)
# Depois: make SFML_PREFIX=<prefixo>

set -euo pipefail

SFML_VERSION="3.0.1"
PREFIX="$(realpath -m "${1:-.deps/sfml}")"
WORK_DIR="$(mktemp -d)"
trap 'rm -rf -- "$WORK_DIR"' EXIT

echo "Baixando SFML ${SFML_VERSION}..."
curl -fsSL "https://github.com/SFML/SFML/archive/refs/tags/${SFML_VERSION}.tar.gz" \
    | tar -xz -C "$WORK_DIR"

cmake -S "$WORK_DIR/SFML-${SFML_VERSION}" -B "$WORK_DIR/build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON \
    -DSFML_BUILD_AUDIO=OFF \
    -DSFML_BUILD_NETWORK=OFF \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -DSFML_INSTALL_PKGCONFIG_FILES=ON \
    -DSFML_PKGCONFIG_INSTALL_DIR="$PREFIX/lib/pkgconfig" \
    -DCMAKE_INSTALL_PREFIX="$PREFIX"

cmake --build "$WORK_DIR/build" --parallel "$(nproc)"
cmake --install "$WORK_DIR/build"

# O SFML 3.0.1 gera "libdir=${exec_prefix}/" nos arquivos .pc quando
# CMAKE_INSTALL_LIBDIR é relativo; corrige para apontar para lib/.
sed -i 's|^libdir=.*|libdir=${exec_prefix}/lib|' "$PREFIX"/lib/pkgconfig/sfml-*.pc

echo
echo "SFML ${SFML_VERSION} instalado em: $PREFIX"
echo "Compile o jogo com: make SFML_PREFIX=$PREFIX"
