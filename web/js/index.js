var pages = ["#home", "#car", "#cardva"]
var page = 0;
var wasSuccessful = true;

function load() {
  var href = "/data.json"
  $.ajax(href, {
    success: function(data) {
      $('#max-speed').html(Math.floor(data.max_speed) + "km/h");
      $('#current-speed').html(Math.floor(data.current_speed / 10));
      $('#leader-distance').html(Math.floor(data.leader_distance / 100) + "m");
      $('#leader-speed').html(Math.floor(data.leader_speed / 100 * 3.6));
      $('#connection-status').html("CONEXÃO ESTABELECIDA");
      $('#connection-summary').html("OK");
      if (!wasSuccessful) {
        goToPage(0);
      }
      wasSuccessful = true;
    },
    error: function() {
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

  });

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