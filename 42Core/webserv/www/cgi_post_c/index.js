function cgi_fetch(form) {
	let country = form.elements["lcountries"].value.trim();
	let age = form.elements["lage"].value.trim();
	let post_body = "country=" + country + "&age=" + age;
	const cgi_request = new Request('/www/cgi-bin/c_cgi.c?id=666', {method: 'POST', body: post_body});

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
		    let flag_box = document.getElementById("flag-box");
			if (document.getElementById("c_pl")) {
				flag_box.style.backgroundImage = "url('pl.svg')";
			}
			if (document.getElementById("c_br")) {
				flag_box.style.backgroundImage = "url('br.png')";
			}
			if (document.getElementById("c_mx")) {
				flag_box.style.backgroundImage = "url('mx.png')";
			}
			if (document.getElementById("c_pt")) {
				flag_box.style.backgroundImage = "url('pt.jpg')";
			}
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
	const AGE_REQUIRED = "Please enter your age";
	const AGE_VALID = "Please enter a valid number";

	event.preventDefault();

	// validate the form
	let agePresent = hasValue(form.elements["lage"], AGE_REQUIRED);
	let ageValid = ageIsValid(form.elements["lage"], AGE_VALID);
	// if valid, submit the form.
	if (agePresent && ageValid) {
		cgi_fetch(form);
	}
}


document.addEventListener("DOMContentLoaded", () => {

	const form = document.getElementById("cgi-form");
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
	//auto-sumbit

	form.addEventListener("submit", function (event) {
		sendForm(event);
	});
});

