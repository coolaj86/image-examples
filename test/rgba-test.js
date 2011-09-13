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

             // 248 252 248
    rgba2short([255, 0, 0, 255], 0); // 248, 0
    rgba2short([0, 0, 255, 255], 0); // 0, 31
    rgba2short([0, 255, 0, 255], 0); // 7, 224

    rgba2short([128, 0, 0, 255], 0);
    rgba2short([0, 128, 0, 255], 0);
    rgba2short([0, 0, 128, 255], 0);

    function short2rgba(rg, gb) {
      var red = (rg & ~0x7); // knock of the last 3 green bits, leave red bits high values
      var green = ((((rg & 0x7) << 3) | (gb & 0xE0) >> 5)) << 2; // good luck understanding this
      var blue = (gb & ~0xE0) << 3;
      return [red, green, blue, 255];
    }
