#ifndef WEB_COMPAT_H
#define WEB_COMPAT_H

// =========================================================================
// Compatibilidade WebAssembly (Emscripten).
//
// O jogo usa loops bloqueantes aninhados (um por tela), o que o navegador não
// permite. Compilamos com -sASYNCIFY e fazemos cada EndDrawing() ceder o
// controle ao navegador via emscripten_sleep(0): isso permite que o browser
// renderize o frame e processe input, e então retoma a execução no ponto exato.
//
// Truque do pré-processador: a chamada EndDrawing() dentro da definição do macro
// NÃO é reexpandida (regra de "blue paint"), então invoca a função real do
// raylib. Como o include vem logo após "raylib.h" em all_headers.h, o macro vale
// em todos os .c sem precisar tocar em nenhum dos ~15 loops.
//
// No build nativo (sem __EMSCRIPTEN__) nada disso é definido e EndDrawing()
// continua sendo a função normal do raylib.
// =========================================================================

#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
  #define EndDrawing() do { EndDrawing(); emscripten_sleep(0); } while (0)
#endif

#endif // WEB_COMPAT_H
