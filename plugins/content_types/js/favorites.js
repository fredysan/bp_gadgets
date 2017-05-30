(function ($) {
  Drupal.behaviors.bp_gadgetsBehavior = {
    attach: function (context, settings) {
      $('.favorites').once().click(function (event) {
        var self = this;
        var id = $(this).attr('id');
        var href = $(this).attr('href');
        var parts = href.split('/');
        var nid = parts[4];
        
        event.preventDefault();

        url = Drupal.settings.basePath + href;
        $.ajax({
          url: location.protocol + '//' + location.host + url,
          type: 'POST',
          data: {},
          dataType: 'json',
          success: function (data) {
            if (data.status == true) {
              url = Drupal.settings.basePath + 'user/favorites/get-link/' + nid;
              $.ajax({
                url: location.protocol + '//' + location.host + url,
                type: 'POST',
                data: {},
                dataType: 'json',
                success: function (data) {
                  $(self).replaceWith(data.html);
                  Drupal.attachBehaviors();
                }
              });
            }
          }
        });
      });
    }
  };

}(jQuery));