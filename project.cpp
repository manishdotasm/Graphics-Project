#include <GLFW/glfw3.h>
#include <complex>

// Initial view parameters
double zoom = 1.0;
double offset_x = 0.0;
double offset_y = 0.0;

void draw_mandelbrot(int window_width, int window_height, int max_iterations = 100) {
    glBegin(GL_POINTS);
    for (int x = 0; x < window_width; ++x) {
        for (int y = 0; y < window_height; ++y) {
            double zx = 1.5 * (x - window_width / 2.0) / (0.5 * zoom * window_width) + offset_x;
            double zy = (y - window_height / 2.0) / (0.5 * zoom * window_height) + offset_y;
            std::complex<double> c(zx, zy);
            std::complex<double> z(0.0, 0.0);
            int iteration = 0;
            while (std::abs(z) <= 2.0 && iteration < max_iterations) {
                z = z * z + c;
                ++iteration;
            }
            double color = static_cast<double>(iteration) / max_iterations;
            glColor3f(color, color, color);
            glVertex2f(x - window_width / 2.0, y - window_height / 2.0);
        }
    }
    glEnd();
}

void draw_julia(int window_width, int window_height, std::complex<double> c = {-0.7, 0.27015}, int max_iterations = 100) {
    glBegin(GL_POINTS);
    for (int x = 0; x < window_width; ++x) {
        for (int y = 0; y < window_height; ++y) {
            double zx = 1.5 * (x - window_width / 2.0) / (0.5 * zoom * window_width) + offset_x;
            double zy = (y - window_height / 2.0) / (0.5 * zoom * window_height) + offset_y;
            std::complex<double> z(zx, zy);
            int iteration = 0;
            while (std::abs(z) <= 2.0 && iteration < max_iterations) {
                z = z * z + c;
                ++iteration;
            }
            double color = static_cast<double>(iteration) / max_iterations;
            glColor3f(color, color, color);
            glVertex2f(x - window_width / 2.0, y - window_height / 2.0);
        }
    }
    glEnd();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Convert mouse coordinates to complex plane coordinates
    mouse_x = 1.5 * (mouse_x - width / 2.0) / (0.5 * zoom * width) + offset_x;
    mouse_y = (mouse_y - height / 2.0) / (0.5 * zoom * height) + offset_y;

    if (yoffset > 0) {
        zoom *= 1.1;
    } else {
        zoom /= 1.1;
    }

    // Adjust the offset to zoom towards the cursor position
    offset_x = mouse_x - (mouse_x - offset_x) * (zoom / (zoom / 1.1));
    offset_y = mouse_y - (mouse_y - offset_y) * (zoom / (zoom / 1.1));
}

int main() {
    if (!glfwInit()) {
        printf("GLFW could not be initialized.\n");
        return -1;
    }

    int window_width = 1024, window_height = 786;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Mandelbrot and Julia Sets", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Setting up the view
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Uncomment one of the following lines to draw the corresponding set
        //draw_mandelbrot(window_width, window_height);
        draw_julia(window_width, window_height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
