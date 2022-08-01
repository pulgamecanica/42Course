window.addEventListener('DOMContentLoaded', (event) => {
	let gandalf = document.getElementById("gandalf");
	let dialog = document.getElementById("dialog-thinking");
	gandalf.addEventListener("mouseover", function reveal_dialog() {
		dialog.style.visibility = "visible";
	});
	gandalf.addEventListener("mouseout", function hide_dialog() {
		dialog.style.visibility = "hidden";
	});
});

function cgi_fetch(form) {
	let query = form.elements["query"].value.trim();
	let post_body = "query=" + query;
	let action = form.action;
	const cgi_request = new Request(action, {method: 'POST', body: post_body});
	fetch(cgi_request)
	.then(data => {
		const reader = data.body.getReader();
		let charsReceived = 0;
		reader.read().then(function processText({ done, value }) {
			charsReceived += value.length;
			let content = new TextDecoder().decode(value); 
		    query_box = document.getElementById("query-box");
		    query_box.innerHTML = content;
		    query_box.style.visibility = "visible";
		});
	});
}

function showMessage(input, message, type) {
	const msg = input.parentNode.querySelector("small");
	const box = input.parentNode;
	msg.innerText = message;
	input.className = type ? "success" : "error";
	box.className = type ? "success" : "error";
	return type;
}

function showError(input, message) {
	return showMessage(input, message, false);
}

function showSuccess(input) {
	return showMessage(input, "", true);
}

function hasValue(input, message) {
	if (input.value.trim() === "") {
		return showError(input, message);
	}
	return showSuccess(input);
}

function sendForm(event) {
	const form = document.getElementById("cgi-form");
	const VALID_STRING = "Please enter valid input";
	event.preventDefault();
	let agePresent = hasValue(form.elements["query"], VALID_STRING);
	if (agePresent) {
		cgi_fetch(form);
	} else {
	    query_box = document.getElementById("query-box");
		query_box.style.visibility = "hidden";
	}
}

document.addEventListener("DOMContentLoaded", () => {
	const form = document.getElementById("cgi-form");
	let auto_elems = document.querySelectorAll(".auto-sumbit");
	auto_elems.forEach((element) => {
		element.addEventListener("change", function (event) {
			sendForm(event);
		});
		element.addEventListener("keyup", function (event) {
			sendForm(event);
		});
	});
	form.addEventListener("submit", function (event) {
		sendForm(event);
	});
});
