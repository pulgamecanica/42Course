import restaurants from './restaurants';

const dollarSigns = '$$';
const deliveryTimeMax = 90;
const maxDistance = '10';
let result;

const priceBracket: string = dollarSigns.length.toString();

const filteredRestaurants = restaurants.filter((restaurant) => {
  if (restaurant.priceBracket > priceBracket) {
    return false;
  }

  if (restaurant.deliveryTimeMinutes > deliveryTimeMax) {
    return false;
  }

  if (restaurant.distance > maxDistance) {
    return false;
  }

  return restaurant;
});

if (filteredRestaurants.length === 0) {
  result = 'There are no restaurants available right now.';
} else {
  result = `We found ${filteredRestaurants.length} restaurants, the first is ${filteredRestaurants[0].name}.`;
}

console.log(result);
