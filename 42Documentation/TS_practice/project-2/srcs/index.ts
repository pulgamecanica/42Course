import products from './products';

let productName : string;
const filteredProds = 'tote bag';

const product = products.filter(element => {
  if (element.name === filteredProds)
    return true;
  return false;
})[0];

console.log(product || "Not found");

let shipping : number = 5;
let taxPercent : number = 0.05;
let taxTotal : number = 0.0;
let total : number = 0.0;

if (product) {
  let price : number = parseInt(product.price);
  if (price >= 25) {
    console.log("This product offers free shiping!");
    shipping = 0;
  }
  let shippingAddress : string = "New York City"

  if (shippingAddress.match("New York City"))
    taxPercent = 0.1;

  taxTotal = price * taxPercent;
  total = taxTotal + price + shipping;
  console.log(`Name: ${product.name}`);
  console.log(`Address: ${shippingAddress}`);
  console.log(`Price: \$${product.price}`);
  console.log(`Shipping: \$${shipping}`);
  console.log(`Tax Percent: \$${taxPercent}`);
  console.log(`Tax Total: \$${taxTotal}`);
  console.log(`Total: \$${total}`);
}
