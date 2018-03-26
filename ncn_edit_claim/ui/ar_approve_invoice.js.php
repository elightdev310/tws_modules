<?php
/**
 * @var: $container_section
 */
?>
<script>
jQuery(function($) {
$(document).ready(function() {
    $('.ars-approve-invoice-btn').on('click', function() {
        var $section = $('<?php echo $container_section; ?>');
        var claim_id = $(this).data('claim');
        if (confirm('Are you sure to approve invoice (#'+claim_id+')?')) {
            var _url = Drupal.settings.basePath + 'account/ar/'+claim_id+'/approve_invoice';
            $.ajax({
                url: _url,
                type: "POST",
                beforeSend: function(jqXHR, settings) {
                    $section.loadingOverlay();
                },
                error: function() {},
                success: function(response) {
                    eval("var json=" + response + ";");
                    if (json.status == "success") {
                        window.location.reload(true);
                    } else {
                        if (json.msg != "") { alert(json.msg); }
                    }
                }, 
                complete: function(jqXHR, textStatus) {
                    $section.loadingOverlay('remove');
                }
            });
        }
        
        return false;
    });
});
});
</script>
