(function () {
  "use strict";

  var connect = require('connect');

  function cors(req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Credentials', 'true');
    res.setHeader('Access-Control-Allow-Methods', 'POST, GET, PUT, DELETE, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type, Accept, X-HTTP-Method-Override, X-Method-Override');
    next();
  }

  module.exports = connect.createServer(
    cors,
    connect.static('./')
  );
}());
