#include <iostream>

#include <Application.h>

int main() {

    try {
        Application app;
        app.Run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
