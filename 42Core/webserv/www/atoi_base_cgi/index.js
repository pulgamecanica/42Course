function cgi_fetch(form) {
	let action = form.action;
	let string = form.elements["string"].value.trim();
	let base = form.elements["base"].value.trim();
	let post_body = "string=" + string + "&base=" + base;
	const cgi_request = new Request(action, {method: 'POST', body: post_body});

	fetch(cgi_request)
	.then(data => {
		const reader = data.body.getReader();
		let charsReceived = 0;
		console.log(reader);

		reader.read().then(function processText({ done, value }) {
			charsReceived += value.length;
			console.log(value);
			let content = new TextDecoder().decode(value); 
		    console.log(content);
		    document.getElementById("fetch-response").innerHTML = content;
		});

	});
}

function showMessage(input, message, type) {
	// get the small element and set the message
	const msg = input.parentNode.querySelector("small");
	const box = input.parentNode;

	msg.innerText = message;
	// update the class for the input
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

function ageIsValid(input, message) {
	if (input.type === "number" && parseInt(input.value) <= 0) {
		return showError(input, message);
	}
	return (true);
}

function sendForm(event) {
	const form = document.getElementById("cgi-form");
	const STR_VALID = "Input cannot be blank";

	event.preventDefault();

	// validate the form
	let stringPresent = hasValue(form.elements["string"], STR_VALID);
	let basePresent = hasValue(form.elements["base"], STR_VALID);

	// if valid, submit the form.
	if (stringPresent && basePresent) {
		cgi_fetch(form);
	}
}

document.addEventListener("DOMContentLoaded", () => {
	let auto_elems = document.querySelectorAll(".auto-sumbit");

	console.log(auto_elems);

	auto_elems.forEach((element) => {
		console.log(element);
		element.addEventListener("change", function (event) {
			sendForm(event);
		});
		element.addEventListener("keyup", function (event) {
			sendForm(event);
		});
	});	
});
