    #include <CL/cl.hpp>
    #include <iostream>

    int main() {
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms); // Get available platforms

        if (platforms.empty()) {
            std::cerr << "No OpenCL platforms found." << std::endl;
            return 1;
        }

        cl::Platform platform = platforms[0]; // Use the first platform
        std::cout << "Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU, &devices); // Get GPU devices

        if (devices.empty()) {
            std::cerr << "No GPU devices found on this platform." << std::endl;
            return 1;
        }

        cl::Device device = devices[0]; // Use the first GPU device
        std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;

        // Further OpenCL operations using cl.hpp classes
        // (e.g., creating context, command queue, buffers, compiling kernel)

        return 0;
    }