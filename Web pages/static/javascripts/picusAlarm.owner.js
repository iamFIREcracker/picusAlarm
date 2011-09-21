var Owner = (function() {
  return {
    onReady: function() {
      $('<form>'
        + '<label>Email: <input type="text" name="email" id="email" /></label>'
        + '<label>Phone: +<input type="text" name="phone" id="phone" /></label>'
        + '<input type="submit" id="save_owner" value="Save" />'
        + '</form>').appendTo('#owner');
    }
  }
})();
