import glfw
from OpenGL.GL import *
import numpy as np

# Initial view parameters
zoom = 1.0
offset_x = 0.0
offset_y = 0.0

def draw_mandelbrot(window_width, window_height, max_iterations=100):
    glBegin(GL_POINTS)
    for x in range(window_width):
        for y in range(window_height):
            zx = 1.5 * (x - window_width / 2) / (0.5 * zoom * window_width) + offset_x
            zy = (y - window_height / 2) / (0.5 * zoom * window_height) + offset_y
            c = zx + zy * 1j
            z = 0 + 0j
            iteration = 0
            while abs(z) <= 2 and iteration < max_iterations:
                z = z * z + c
                iteration += 1
            color = iteration / max_iterations
            glColor3f(color, color, color)
            glVertex2f((x - window_width / 2), (y - window_height / 2))
    glEnd()

def draw_julia(window_width, window_height, c=-0.7 + 0.27015j, max_iterations=100):
    glBegin(GL_POINTS)
    for x in range(window_width):
        for y in range(window_height):
            zx = 1.5 * (x - window_width / 2) / (0.5 * zoom * window_width) + offset_x
            zy = (y - window_height / 2) / (0.5 * zoom * window_height) + offset_y
            z = zx + zy * 1j
            iteration = 0
            while abs(z) <= 2 and iteration < max_iterations:
                z = z * z + c
                iteration += 1
            color = iteration / max_iterations
            glColor3f(color, color, color)
            glVertex2f((x - window_width / 2), (y - window_height / 2))
    glEnd()

def scroll_callback(window, xoffset, yoffset):
    global zoom, offset_x, offset_y

    mouse_x, mouse_y = glfw.get_cursor_pos(window)
    width, height = glfw.get_framebuffer_size(window)

    # Convert mouse coordinates to complex plane coordinates
    mouse_x = 1.5 * (mouse_x - width / 2) / (0.5 * zoom * width) + offset_x
    mouse_y = (mouse_y - height / 2) / (0.5 * zoom * height) + offset_y

    if yoffset > 0:
        zoom *= 1.1
    else:
        zoom /= 1.1

    # Adjust the offset to zoom towards the cursor position
    offset_x = mouse_x - (mouse_x - offset_x) * (zoom / (zoom / 1.1))
    offset_y = mouse_y - (mouse_y - offset_y) * (zoom / (zoom / 1.1))

def main():
    global zoom, offset_x, offset_y

    if not glfw.init():
        print("GLFW could not be initialized.")
        return

    window_width, window_height = 1920, 1080
    window = glfw.create_window(window_width, window_height, "Mandelbrot and Julia Sets", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    glfw.set_scroll_callback(window, scroll_callback)

    while not glfw.window_should_close(window):
        glClear(GL_COLOR_BUFFER_BIT)

        # Setting up the view
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        width, height = glfw.get_framebuffer_size(window)
        glOrtho(-width / 2, width / 2, -height / 2, height / 2, -1, 1)

        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

        # Uncomment one of the following lines to draw the corresponding set
        draw_mandelbrot(window_width, window_height)
        # draw_julia(window_width, window_height)

        glfw.swap_buffers(window)
        glfw.poll_events()

    glfw.terminate()

if __name__ == "__main__":
    main()