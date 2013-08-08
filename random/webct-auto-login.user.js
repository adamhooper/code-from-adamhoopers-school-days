// Auto-login to McGill WebCT
// 2006-03-28
// Copyright (c) 2006, Adam Hooper
// Released under the GPL license
// http://www.gnu.org/copyleft/gpl.html
//
// ==UserScript==
// @name Auto McGill-WebCT login
// @namespace http://www.adamhooper.com:4242/blog
// @description Log in to WebCT automatically with the auto-complete password
// @include https://signin.mcgill.ca/?ImaPopup=1&GLBAuthPath=/webct/&GLBAuthServer=home&Tracking=2
// ==/UserScript==

// Submit if the word 'REFUSED' is not on the web page
function submitIfNotRefused() {
	// if-not-refused...
	var nodes = document.evaluate('//text()', document, null,
				      XPathResult.UNORDERED_NODE_SNAPSHOT_TYPE,
				      null);
	for (var i = 0; i < nodes.snapshotLength; i++) {
		var node = nodes.snapshotItem(i);
		if (node.data.indexOf('REFUSED') != -1) {
			return;
		}
	}

	// submit!
	document.SigninForm.submit();
}

// Delay call to submitIfNotRefused() until after the password auto-complete
function delaySubmitIfNotRefused() {
	setTimeout(submitIfNotRefused, 0);
}

window.addEventListener('load', delaySubmitIfNotRefused, true);
