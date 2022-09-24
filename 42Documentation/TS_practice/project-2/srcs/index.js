"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const products_1 = require("./products");
let productName;
const filteredProds = 'tote bag';
const product = products_1.default.filter(element => {
    if (element.name === filteredProds)
        return true;
    return false;
})[0];
console.log(product || "Not found");
let shipping = 5;
let taxPercent = 0.05;
let taxTotal = 0.0;
let total = 0.0;
if (product) {
    let price = parseInt(product.price);
    if (price >= 25) {
        console.log("This product offers free shiping!");
        shipping = 0;
    }
    let shippingAddress = "New York City";
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
