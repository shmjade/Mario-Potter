// =========================================================================
// Desbloqueio de áudio no navegador (política de autoplay) + diagnóstico.
//
// raylib/miniaudio cria um AudioContext na InitAudioDevice(), antes de qualquer
// gesto — então o navegador o mantém "suspended". Interceptamos a criação do
// AudioContext, guardamos as instâncias e as retomamos no primeiro gesto.
//
// Injetado via `emcc --pre-js`. Loga em [audio-unlock] quantos contextos foram
// capturados e o estado de cada um após o gesto (diagnóstico).
// =========================================================================
(function () {
  var Native = window.AudioContext || window.webkitAudioContext;
  if (!Native) { console.log('[audio-unlock] sem AudioContext no navegador'); return; }

  var contexts = [];
  var gestured = false;
  window.__rayAudioCtxs = contexts; // inspecionável no console

  function tryResume(c) {
    if (c && c.state === 'suspended' && typeof c.resume === 'function') c.resume();
  }
  function track(ctx) {
    contexts.push(ctx);
    console.log('[audio-unlock] AudioContext criado (#' + contexts.length + '), state=' + ctx.state);
    if (gestured) tryResume(ctx);
    return ctx;
  }
  function Wrapped(opts) { return track(new Native(opts)); }
  Wrapped.prototype = Native.prototype;
  window.AudioContext = Wrapped;
  window.webkitAudioContext = Wrapped;

  function onGesture() {
    gestured = true;
    var states = [];
    for (var i = 0; i < contexts.length; i++) { tryResume(contexts[i]); states.push(contexts[i].state); }
    console.log('[audio-unlock] gesto: ' + contexts.length + ' contexto(s), states=' + states.join(','));
  }
  var events = ['pointerdown', 'mousedown', 'keydown', 'touchstart', 'click'];
  for (var j = 0; j < events.length; j++) {
    window.addEventListener(events[j], onGesture, true);
    document.addEventListener(events[j], onGesture, true);
  }
})();
