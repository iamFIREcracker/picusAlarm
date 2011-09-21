var Logger = (function() {
  return {
    _$data: null,

    onReady: function() {
      Logger._$data = $('#screen');
    },

    _log: function(msg) {
      Logger._$data.text(msg);
      setTimeout(Logger.timeoutCb, 3000);
    },

    timeoutCb: function() {
      Logger._$data.empty();
    },

    info: function(msg) {
      Logger._$data.css('background-color', 'grey');
      Logger._log(msg);
    },

    warning: function(msg) {
      Logger._$data.css('background-color', 'orange');
      Logger._log(msg);
    },

    error: function(msg) {
      Logger._$data.css('background-color', 'red');
      Logger._log(msg);
    }
  }
})();
