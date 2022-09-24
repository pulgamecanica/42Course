// create the image data
const imageWidth = 20;
const imageHeight = 8;
const imageData = createImageData();

// draw head
drawRectangle(0, 0, 20, 8);
// eyes
drawDot(7, 2);
drawDot(12, 2);
// smile
drawDot(4, 4);
drawHorizontalLine(4, 5, 12);
drawDot(15, 4);

// output what we drew to the console
outputImage();

/**
 * drawDot(x : number, y: number)
 * Change status of the coordinate [x, y]
 *
 * @param x - X coordinate
 * @param y - Y coordinate
 * @return void 
 */

function drawDot(x : number, y : number) : void {
	if (isPointInImage(x, y))
		imageData[y * imageWidth + x] = true;
}

function drawHorizontalLine(x : number, y : number, width : number): void {
	for(let i : number = 0; i < width; i++)
		drawDot(x + i, y);
}

function drawVerticalLine(x : number, y : number, width : number): void {
	for(let i : number = 0; i < width; i++)   
                drawDot(x, y + i);
}


function drawRectangle(
  x: number,
  y: number,
  width: number,
  height: number
) {
  // top
  drawHorizontalLine(x, y, width);
  // bottom
  drawHorizontalLine(x, y + height - 1, width);
  // left
  drawVerticalLine(x, y, height);
  // right
  drawVerticalLine(x + width - 1, y, height);
}

/**
 * Gets if the provided point is in the image.
 * @param x - The horizontal position within
 * the image.
 * @param y - The vertical position within
 * the image.
 */
function isPointInImage(x: number, y: number): boolean {
  return x >= 0 && x < imageWidth && y >= 0 && y < imageHeight;
}

/**
 * Outputs the image data state to the console.
 * @param onChar - Character to render an
 * "on" pixel with.
 * @param offChar - Character to render an
 * "off" pixel with.
 */
function outputImage(onChar = "X", offChar = " "): void {
  let text = "";

  for (let i = 0; i < imageData.length; i++) {
    if (i > 0 && i % imageWidth === 0) {
      text += "\n"; // new line
    }

    text += imageData[i] ? onChar : offChar;
  }

  console.log(text);
}

/**
 * Creates an array of booleans where a pixel
 * is "on" when the value is `true` and "off"
 * when the value is `false`.
 *
 * The pixel values are stored in rows
 * (row-major order) where the index of a
 * pixel in the array can be found via:
 *
 *     index = y * imageWidth + x
 *  
 * `x` is the horizontal position in the image
 * and `y` is the vertical position from the top
 * left corner.
 * 
 * Note: This function has a return type annotation
 * of `boolean[]`. That means it's an array of
 * booleans. We'll learn more about this in a
 * future module.
 */
function createImageData(): boolean[] {
  // create array of size `length` containing `false` values
  const length = imageWidth * imageHeight;
  return new Array(length).fill(false);
}
