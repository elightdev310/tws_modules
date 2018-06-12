var TWCApp = TWCApp || {
    reloadPage: function (target) {
        console.log(target);
        if (typeof target == 'undefined') {
            target = '_blank';
        }

        if (target == '_blank') {
            window.location.reload(true);
        } else if (target == '_opener') {
            window.opener.location.reload(true);
            window.close();
        } else if (target == '_parent') {
            parent.location.reload(true);
        }
    }
};

function string_trim(text) {
    return text.replace(/^\s+/g, '').replace(/\s+$/g, '');
}
