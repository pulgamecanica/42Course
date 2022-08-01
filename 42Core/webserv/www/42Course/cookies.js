function cgi_fetch(form) {
	let selection = form.elements["yes"].checked;
	let post_body = "yes_no=" + selection;
	let action = form.action;
	if (selection === true) {
		const cgi_request = new Request(action, {method: 'POST', body: post_body});
		fetch(cgi_request)
		.then(data => {
			console.log("Perfect!");
			console.log(selection);
		});
	}
}

function sendForm(event) {
	const form = document.getElementById("session-form");
	event.preventDefault();
	cgi_fetch(form);
	form.style.display = "none";
}

document.addEventListener("DOMContentLoaded", () => {
	const form = document.getElementById("session-form");
	const list = document.getElementsByClassName("delete-session-form");
	for (var i = 0; i < list.length; ++i) {
		let action = list[i].action;
		list[i].addEventListener("submit", function (event) {
			event.preventDefault();
			req = new Request (action, {method: 'DELETE'});
			console.log("Sending req...");
			fetch(req)
			.then(data => {
				const reader = data.body.getReader();
				let charsReceived = 0;
				console.log(reader);
				reader.read().then(function processText({ done, value }) {
					charsReceived += value.length;
					let content = new TextDecoder().decode(value); 
				    console.log(content);
				    document.getElementById("notice").innerHTML = content;
				});
			});
		});
	}

	if (form) {
		form.addEventListener("submit", function (event) {
			sendForm(event);
		});
	}
});
