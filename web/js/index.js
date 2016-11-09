  function load() {
  	var href = "data.json"
    $.ajax(href, {
      success: function(data) {
         $('#limit').html(data.max);
      },
      error: function() {
         $('#limit').html("0");
      }
   });
}

load()