// =========================================================================
// Desbloqueio de áudio no navegador (política de autoplay).
//
// raylib/miniaudio cria um AudioContext logo na InitAudioDevice(), antes de
// qualquer gesto do usuário — então o navegador o mantém "suspended" e nada
// toca. Aqui interceptamos a criação do AudioContext, guardamos as instâncias
// e as retomamos (resume()) no primeiro clique/tecla/toque.
//
// Injetado no bundle via `emcc --pre-js`, roda antes do runtime WASM, então a
// substituição de window.AudioContext já está ativa quando o miniaudio cria o
// seu contexto.
// =========================================================================
(function () {
  var Native = window.AudioContext || window.webkitAudioContext;
  if (!Native) return;

  var contexts = [];
  function Wrapped(opts) {
    var ctx = new Native(opts);
    contexts.push(ctx);
    return ctx;
  }
  Wrapped.prototype = Native.prototype;
  window.AudioContext = Wrapped;
  window.webkitAudioContext = Wrapped;

  function resumeAll() {
    for (var i = 0; i < contexts.length; i++) {
      var c = contexts[i];
      if (c && c.state === 'suspended' && typeof c.resume === 'function') {
        c.resume();
      }
    }
  }

  var events = ['pointerdown', 'mousedown', 'keydown', 'touchstart', 'click'];
  for (var i = 0; i < events.length; i++) {
    window.addEventListener(events[i], resumeAll, true);
    document.addEventListener(events[i], resumeAll, true);
  }
})();
