#!/usr/bin/env bash
# =============================================================================
# Build WebAssembly do Mario-Potter (raylib) via Emscripten.
#
# Pré-requisito: ambiente Emscripten ativo (emcc/emmake no PATH). No GitHub
# Actions isso vem do mymindstorm/setup-emsdk. Localmente: source emsdk_env.sh.
#
# Estratégia: raylib compilada para web + jogo com -sASYNCIFY (preserva os loops
# bloqueantes aninhados; ver Headers/web_compat.h). Assets via --preload-file.
#
# Saída: web/index.{html,js,wasm,data}. Sirva via HTTP (não file://):
#   cd web && python3 -m http.server 8000   ->  http://localhost:8000
# =============================================================================
set -euo pipefail
cd "$(dirname "$0")"

RAYLIB_VERSION="5.5"

# --- 1. raylib para web (libraylib.web.a) ---------------------------------
if [ ! -f raylib/src/libraylib.web.a ]; then
  if [ ! -d raylib ]; then
    git clone --depth 1 --branch "$RAYLIB_VERSION" https://github.com/raysan5/raylib.git
  fi
  emmake make -C raylib/src PLATFORM=PLATFORM_WEB -B
fi

mkdir -p web

# --- 2. assets usados (preload curado) ------------------------------------
# Apenas o que load_media.c carrega — as pastas têm muitos arquivos não usados
# (vozes, duplicatas, ground (1) (1).png ~1.4MB) que inflariam o download.
IMAGES=(harryRight harryLeft harryFront dracoRight dracoFront fireplace
        fireplaceGreen ground broom enemies patronum house_cup sorting_hat
        owl podium letter gryffindor hufflepuff ravenclaw slytherin envelope
        hogwarts_express)
AUDIO=(afterAllThisTime.mp3 about.mp3 hat_song.mp3 HPthemeSong.mp3 names.mp3
       fastSpell.wav heavySpell.wav expelliarmus.wav next_phase.wav
       patronum.wav harryDied.wav)

PRELOAD=( --preload-file Phases --preload-file Fonts/fonteHP.TTF )
for i in "${IMAGES[@]}"; do PRELOAD+=( --preload-file "Images/$i.png" ); done
for a in "${AUDIO[@]}";  do PRELOAD+=( --preload-file "Audio/$a" ); done

# --- 3. compilar o jogo ----------------------------------------------------
emcc main.c Sources/*.c \
  -IHeaders -Iraylib/src \
  -O2 -std=c99 \
  raylib/src/libraylib.web.a \
  -sUSE_GLFW=3 \
  -sASYNCIFY \
  -sASYNCIFY_STACK_SIZE=131072 \
  -sALLOW_MEMORY_GROWTH=1 \
  "${PRELOAD[@]}" \
  -o web/index.html

echo "OK -> web/index.html (+ .js/.wasm/.data)"
