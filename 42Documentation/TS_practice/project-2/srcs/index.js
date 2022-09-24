"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const products_1 = require("./products");
let productName;
const filteredProds = 'fanny pack';
const product = products_1.default.filter(element => {
    if (element.name === filteredProds)
        return true;
    return false;
})[0] || "Not found";
console.log(product);
let pre_order_option = product.preOrder === 'true' ? true : false;
if (pre_order_option)
    console.log("This item is available for pre order, we will notify you with a message when the product is on it's way.");
