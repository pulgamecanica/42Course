document.addEventListener("DOMContentLoaded", function(event) {

});

document.addEventListener("click", function(){
   var e = window.event;
   let posX = e.clientX;
   let posY = e.clientY;
   //alert("Px: " + posX.toString() + "Py: " + posY.toString());
   var dateSpan = document.createElement('span')
   dateSpan.classList.add("ball");
   dateSpan.style.top = posY.toString() + "px";
   dateSpan.style.left = posX.toString() + "px";
   document.body.appendChild(dateSpan);
});
