/**
 * @file
 * Url Redirect javascript functions.
 */

Drupal.behaviors.url_redirect = {
    attach: function (context) {
        jQuery(".form-item-roles").hide();
        jQuery(".users-checkbox").hide();
        jQuery("#edit-checked-for-role").change(function () {
            if (this.checked) {
                jQuery(".form-item-roles").show();
                jQuery(".users-checkbox").hide();
            }
        });
        jQuery("#edit-checked-for-user").change(function () {
            if (this.checked) {
                jQuery(".users-checkbox").show();
                jQuery(".form-item-roles").hide();
            }
        });
    }
};