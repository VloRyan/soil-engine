import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class SoilEngineRecipe(ConanFile):
    name = "soil_engine"
    version = "0.1"
    package_type = "library"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of engine package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "skip_test": {True, False}}
    default_options = {"shared": False, "fPIC": True, "skip_test": False}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "test/*"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("devil/1.8.0")
        self.requires("glm/1.0.1", transitive_headers=True)
        self.requires("plog/1.1.10")
        self.requires("openal-soft/1.24.3")
        self.requires("gl3w/1.0")

        self.test_requires("gtest/1.16.0")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        do_tests = not self.conf.get("tools.build:skip_test", default=False) and not self.options.skip_test
        cmake = CMake(self)
        if do_tests:
            cmake.configure()
        else:
            cmake.configure(variables={"BUILD_TESTING": "OFF"})
        cmake.build()
        if do_tests:
            test_folder = os.path.join("test")
            if self.settings.os == "Windows":
                test_folder = os.path.join("test", str(self.settings.build_type))
            self.run(os.path.join(test_folder, "test_soil_engine"))

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["soil_engine"]

    def package_id(self):
        # Don't let running unit tests affect the package_id
        del self.info.options.skip_test
