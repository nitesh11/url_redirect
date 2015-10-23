/**
 * @file
 * Url Redirect javascript functions.
 */
(function($) {
  Drupal.behaviors.url_redirect = {
    attach: function (context, settings) {

      $(".form-item-roles").hide();
      $(".users-checkbox").hide();
      $("#edit-checked-for-role").change(function () {
        if (this.checked) {
          $(".form-item-roles").show();
          $(".users-checkbox").hide();
        }
      });
      $("#edit-checked-for-user").change(function () {
        if (this.checked) {
          $(".users-checkbox").show();
          $(".form-item-roles").hide();
        }
      });
    }
  };
})(jQuery);