import products from './products';

let productName : string;
const filteredProds = 'fanny pack';

const product = products.filter(element => {
  if (element.name === filteredProds)
    return true;
  return false;
})[0] || "Not found";

console.log(product);

let pre_order_option : boolean = product.preOrder === 'true' ? true : false;

if (pre_order_option)
  console.log("This item is available for pre order, we will notify you with a message when the product is on it's way.")

let shipping : number;
let taxPercent : number = 0.05;
let taxTotal : number;
let total : number;
let shippingAddress : string = 'Some address in the planet Earth';

