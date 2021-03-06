var pages = ["#home", "#car", "#cardva"]
var page = 0;
var wasSuccessful = true;
var errors = 0;

function load() {
  var href = "/data.json?_=" + (new Date()).getTime();
  $.ajax(href, {
    success: function(data) {
      errors = 0;
      $('#current-speed').html(Math.floor(data.max_speed));
      $('#max-speed').html(Math.floor(data.current_speed / 10) + "&thinsp;km/h");
      $('#leader-speed').html(Math.floor(data.leader_distance / 10) / 10);
      $('#leader-distance').html(Math.floor(data.leader_speed / 100 * 3.6) + "&thinsp;km/h");
      $('#connection-status').html("CONEXÃO ESTABELECIDA");
      $('#connection-summary').html("OK");
      if (!wasSuccessful) {
        goToPage(0);
      }
      wasSuccessful = true;
    },
    error: function() {
      errors++;
      if (errors >= 3) {
        $('#max-speed').html("?");
        $('#current-speed').html("?");
        $('#leader-distance').html("?");
        $('#leader-speed').html("?");
        $('#connection-status').html("FALHA NA CONEXÃO");
        $('#connection-summary').html("SERVIDOR INDISPONÍVEL");
        if (wasSuccessful) {
          goToPage(1);
        }
        wasSuccessful = false;
      }
    }
  });
}

  // -------ANIMATIONS------- //

  $("#main")
  .bind("touchstart", function() {
    $(this).addClass("active");
  })
  .bind("touchend", function() {
    $(this).removeClass("active");
  });

  $('#main').click(function() {
    goToPage((page + 1) % pages.length);
  })

  function goToPage(desiredPage) {
    if (desiredPage == page) {
      return
    }
    TweenMax.to($(pages[page]), 0.4, {
      y: -360
    })
    TweenMax.fromTo($(pages[desiredPage]), 0.4, {
      y: 360,
      autoAlpha: 1
    }, {
      y: 0
    })
    page = desiredPage;
  }

  // ----- On render -----
  $(function() {

    ["#speed", "#speed2"].forEach(function(e) {
      makeRadial({
        el: $(e),
        radials: 100
      })
    });

    setInterval(function() {
      load();
    }, 1500);

    fitToScreen()

  });

 $(window).resize(fitToScreen);

 function fitToScreen() {
      var win = $(window);
      
      // scale = 1 means the diameter will be 400px
      // a panel should fit to the screen width with some margin
      // both panels should fit to the screen height with some margin
      var scale = Math.min(
        win.width() / 400 / 1.2,
        win.height() / 800 / 1.25);

      TweenMax.to($("body"), 0.4, {
        scale: scale
      })
}

  function makeRadial(options) {
    if (options && options.el) {
      var el = options.el;
      var radials = 60;
      if (options.radials) {
        radials = options.radials;
      }
      var degrees = 360 / radials;
      var i = 0;
      for (i = 0; i < (radials / 2); i++) {
        var newTick = $('<div class="tick"></div>').css({
          '-moz-transform': 'rotate(' + (i * degrees) + 'deg)'
        }).css({
          '-webkit-transform': 'rotate(' + (i * degrees) + 'deg)'
        }).css({
          'transform': 'rotate(' + (i * degrees) + 'deg)'
        })
        el.prepend(newTick);
      }
    }
  }