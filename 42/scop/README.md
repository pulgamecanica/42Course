# Scop a project to visualize 3d objects

### Using GLFW (Graphics Library Framework) to Handle the Window and events


GLFW is a library which allows me to create windows and handle the same window events.

I decided to create 4 wrappers for the "generic" events as follows:

	- Key Input - When the user presses a key (or a series of keys)
	- Mouse Move - When the user moves the mouse
	- Mouse Input - When the user click with the mouse (right, left, wheel)
	- Resize Window - When the user resizes the window

GLFW receives a callback function to handle the events.
This project is being done with C++ and as such, I should always try to do it the most Object Oriended possible way.
That is why it would not be very productive to use a conventional callback function to handle the events.
Why not just using my object function as a callback?
A callback function is a static function (obviously), and not an object method.
It would be great to just use my object method as a callback, for example:

```c++
class MyGame {
	public:
		mouseCallback(double x, double y);
}
main (void) {
	...
	MyGame game;

	glfwSetCursorPosCallback(MyGame::mouseCallback); // This function is just an example, it is not defined like this
	// Or maybe this?!?
	glfwSetCursorPosCallback(game.mouseCallback); // This function is just an example, it is not defined like this
}
```

Buy this is not possible, also which object would GLFW Library use to call this function? How could it know that we want to call that method for our variable game? Also GLFW is written in C, there is no Object concept in C...
So in short, this it not possible and it wouldn't make sense, but we can solve this very easy, by using wrapper:

```c++
static callback_wrapper(GLFWwindow * window, double x, double y) {
	// Do something
	// Here we need to call the function of our game object, to handle the event
	// How do we access our game here!? HELPPPPPPP!
}
main (void) {
	...
	MyGame game;
	GLDWwindow window;
	// initialize game, initialize window etd...
	...

	glfwSetCursorPosCallback(&window, callback_wrapper); // This is the actual function used to set the mouse move hook to a callback 
}
```

The code above works and it will be trigegred when the mouse moves, but something is missing in order to trigger our `game.mouseCallback()` function.

	There is a way to access to our object through the GLFWwindow pointer on the `callback_wrapper`, we just have to set it up beforehand with the function `glfwSetWindowUserPointer()`

```c++
static callback_wrapper(GLFWwindow * window, double x, double y) {
	static_cast<MyGame *>(glfwGetWindowUserPointer(window))->mouseCallback(x, y);
}
main (void) {
	...
	MyGame game;
	GLDWwindow window;
	// initialize game, initialize window etd...
	...
	glfwSetWindowUserPointer(&window, &game);
	glfwSetCursorPosCallback(&window, callback_wrapper);
}
```

Now it will work, and the window mouse move even will call our object function!!! :D

	More info:
	No singleton is being used, because programming with singletons only makes your code week and vulnerable besides the obvious limitation of being impossible to scale the project for more than one window.
	In the actual project, all this setup is fone on the Scop constructor, in order for scop to wrap all the callback functions around itself.
	So using a pointer for my Window is no problem, as long as I have the care to free this pointer on destruction.

***

[GLFW Docs](https://www.glfw.org/docs/latest/quick.html)

[Compilation Guide](https://www.glfw.org/docs/latest/build_guide.html)



### Using Vulkan (to compute and render the project graphics)

Vulkan is a platform agnostic API, meaning that it's free of any ties with a specific platform. This is in part, why it cannot interact with the windowing system. Because it is different in all platforms.
In order to start using Vulkan and render to a window, you need to integrate Vulkan with a Windowing System (which in this cas is GLFW)

With Vulkan we can:

	- Manage the kernel graphic drivers (to enable certain aspect of out CPU if needed)
	- Manage graphical shaders
	- RayTracing

In Vulkan, anything from drawing to uploading textures, requires commands to be submitted to a queue.
There are different types of queues; each of the queues allows different commands, or actions.
In this porject, we will be implementing a queue that supports graphics commands (we will be using the API graphics functions; etc)



Dependencies: 
- GLFW, for Window and events handling
- Vulkan, rendering API
- lodepng, to load the Window png icon