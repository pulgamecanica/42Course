export const randomRickAndMortyImg = () => {
	const options = [
		"surprised",
		"fk",
		"dancing",
		"dab",
		"neutral",
	];

	const baseUrl = "https://transcendence-user-images.s3.eu-west-3.amazonaws.com/assets/rick-and-morty-";

	return `${baseUrl}${options[Math.floor(Math.random() * options.length)]}.png`;
};