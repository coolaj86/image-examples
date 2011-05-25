var binarr;
(function () {
  "use strict";

  function bitstr2imagedata(bitstr, w, h, imgdata) {
    var val
      , row
      , column
      , cell;

    for (row = 0; row < h; row += 1) {
      for (column = 0; column < w; column += 1) {
        var shortpos = 2 * (w * row + column) // 2 bytes for each pixel
          , pos = 4 * (w * row + column) // 4 bytes for each pixel
          , bitone = bitstr[shortpos + 0]
          , bittwo = bitstr[shortpos + 1]
          , rgba = short2rgba(bitone, bittwo)
          ;

        imgdata[pos + 0] = rgba[0];
        imgdata[pos + 1] = rgba[1];
        imgdata[pos + 2] = rgba[2];
        imgdata[pos + 3] = rgba[3];
      }
    }
  }

  function convertStringToArray(str) {
    // Notes:
    /*
      function adds about 15% overhead
      multiplication takes 12x longer
      [].toString takes another 10x or so longer
    */
    var i, n, one, two, arr = [];

    for (i = 0; i < str.length; i += 2) {
      // Little / Big Endian byteswap
      one = str.charCodeAt(i + 1) & 0xFF;
      two = str.charCodeAt(i + 0) & 0xFF;
      arr.push(one);
      arr.push(two);
    }

    return arr;
  }

  function short2rgba(rg, gb) {
    var red = (rg & ~0x7) // knock of the last 3 green bits, leave red bits as high values
      , green = ((((rg & 0x7) << 5) | (gb & 0xE0) >> 3)) // in short `00000111 11100000` becomes `11111100`
      , blue = ((gb & ~0xE0) << 3); // knock of the first 3 green bits, push the blue bits higher

    return [red, green, blue, 255];
  }

  function rgba2short(rgba, i) {
    var r = rgba[i + 0] >> 3 // 5
      , g = rgba[i + 1] >> 2 // 6 (3, 3)
      , g1 = (g >> 3) // 3 higher bits
      , g2 = (g & 0x7) // 3 lower bits
      , b = rgba[i + 2] >> 3 // 5
      , a = rgba[i + 3] >> 8 // n/a
      , one
      , two
      ;

    var one = (r << 3) | g1;
    var two = (g2 << 5) | b;
    return [one, two];
  }

  // https://developer.mozilla.org/en/using_xmlhttprequest
  // http://web.archive.org/web/20071103070418/http://mgran.blogspot.com/2006/08/downloading-binary-streams-with.html
  function getBinary(file){
    var xhr = new XMLHttpRequest();  
    xhr.open("GET", file, false);  
    xhr.overrideMimeType("text/plain; charset=x-user-defined");  
    xhr.send(null);
    return xhr.responseText;
  }

  function sendBinary(data, url){
    var xhr = new XMLHttpRequest();
    xhr.open("POST", url, true);

    if (typeof XMLHttpRequest.prototype.sendAsBinary == "function") { // Firefox 3 & 4
      var tmp = '';
      for (var i = 0; i < data.length; i++) tmp += String.fromCharCode(data.charCodeAt(i) & 0xff);
      data = tmp;
    }
    else { // Chrome 9
      // http://javascript0.org/wiki/Portable_sendAsBinary
      XMLHttpRequest.prototype.sendAsBinary = function(text){
        var data = new ArrayBuffer(text.length);
        var ui8a = new Uint8Array(data, 0);
        for (var i = 0; i < text.length; i++) ui8a[i] = (text.charCodeAt(i) & 0xff);

        var bb = new BlobBuilder(); // doesn't exist in Firefox 4
        bb.append(data);
        var blob = bb.getBlob();
        this.send(blob);
      }
    }

    xhr.sendAsBinary(data); 
  }

  // A simple demo image that is easy to console.log and check for correctness
  function convert5x5() {
    var bitmap = [
          [
            [255,0,0,255], [255,0,0,255], [255,0,0,255], [255,0,0,255], [255,0,0,255]
          ],
          [
            [128,0,0,255], [128,0,0,255], [128,0,0,255], [128,0,0,255], [128,0,0,255]
          ],
          [
            [0,255,0,255], [0,255,0,255], [0,255,0,255], [0,255,0,255], [0,255,0,255]
          ],
          [
            [0,128,0,255], [0,128,0,255], [0,128,0,255], [0,128,0,255], [0,128,0,255]
          ],
          [
            [0,0,255,255], [0,0,255,255], [0,0,255,255], [0,0,255,255], [0,0,255,255]
          ]
        ]
      , bitarr = []
      , bitstr = []
      , canvas = document.getElementById('reference-image').getContext("2d")
      , imageData = canvas.createImageData(5, 5)
      ;

    bitmap.forEach(function (row) {
      row.forEach(function (cell) {
        cell.forEach(function (subpixel) {
          bitarr.push(subpixel);
        });
      });
    });
    console.log('bitarr', bitarr);

    bitarr.forEach(function (val, i) {
      imageData.data[i] = val;
    });
    canvas.putImageData(imageData, 10, 10);

    function bitarr2bitstr() {
      var i, shorty;
      for (i = 0; i < bitarr.length; i += 4) {
        shorty = rgba2short(bitarr, i);
        bitstr.push(shorty[0]);
        bitstr.push(shorty[1]);
      }
    }
    bitarr2bitstr();
    console.log('bitstr', bitstr);

    function bitstr2bitmap(bitstr, w, h) {
      var bitmap = []
        , row
        , column
        , cell
        , val;

      for (row = 0; row < h; row += 1) {
        var cell = bitmap[row] = [];
        for (column = 0; column < w; column += 1) {
          var shortpos = 2 * (w * row + column);
          var bitone = bitstr[shortpos];
          var bittwo = bitstr[shortpos + 1];
          var rgba = short2rgba(bitone, bittwo);

          cell.push(rgba);
        }
      }
      return bitmap;
    }

    bitmap = bitstr2bitmap(bitstr, 5, 5);
    console.log('bitmap', bitmap);

    bitstr2imagedata(bitstr, 5, 5, imageData.data);
    canvas.putImageData(imageData, 50, 50);

  }

  function rgb565ToCanvas(canvas) {
    var binstr = getBinary('/reference.rgb565')
      , timestamp = new Date().valueOf()
      , oldtime = timestamp
      , imageData
      ;

    binarr = convertStringToArray(binstr);
    timestamp = new Date().valueOf();
    console.log('convertStringToArray', timestamp - oldtime);
    oldtime = timestamp;


    imageData = canvas.createImageData(720, 480)
    timestamp = new Date().valueOf();
    console.log('createImage', timestamp - oldtime);
    oldtime = timestamp;


    bitstr2imagedata(binarr, 720, 480, imageData.data);
    timestamp = new Date().valueOf();
    console.log('bitstr2imagedata', timestamp - oldtime);
    oldtime = timestamp;


    canvas.putImageData(imageData, 0, 0);
    timestamp = new Date().valueOf();
    console.log('putImageData', timestamp - oldtime);
    oldtime = timestamp;
  }

  //convert5x5();
  rgb565ToCanvas(document.getElementById('reference-image').getContext("2d"));

  //createImage(binstr, 10, 20);
}());
