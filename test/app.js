(function () {
  "use strict";

  var connect = require('connect');

  module.exports = connect.createServer(
    connect.static('./')
  );
}());
