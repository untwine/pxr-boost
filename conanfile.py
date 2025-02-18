import os
import sys

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class PxrBoostConan(ConanFile):
    name = "pxr-boost"
    version = "26.8"
    package_type = "library"
    license = "LicenseRef-TOST-1.0"
    homepage = "https://github.com/untwine/pxr-boost"
    url = "https://github.com/untwine/pxr-boost"
    description = "Boost.Python fork optimized for OpenUSD"
    topics = ("pixar", "open-usd")

    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_tests": [True, False],
        "precompiled_headers": [True, False],
        "python_version": ["ANY"],
    }
    default_options = {
        "shared": True,
        "fPIC": True,
        "build_tests": False,
        "precompiled_headers": False,
        "python_version": f"{sys.version_info.major}.{sys.version_info.minor}",
    }

    exports_sources = "CMakeLists.txt", "cmake/*", "src/*", "LICENSE.txt", "NOTICE.txt"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def validate(self):
        actual = f"{sys.version_info.major}.{sys.version_info.minor}"
        if str(self.options.python_version) != actual:
            raise ConanInvalidConfiguration(
                f"python_version={self.options.python_version} requested, but "
                f"Conan is running under Python {actual}."
            )

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTS"] = bool(self.options.build_tests)
        tc.variables["ENABLE_PRECOMPILED_HEADERS"] = bool(self.options.precompiled_headers)
        tc.variables["Python_EXECUTABLE"] = sys.executable.replace("\\", "/")
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE.txt", self.source_folder,
             os.path.join(self.package_folder, "licenses"))
        copy(self, "NOTICE.txt", self.source_folder,
             os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "none")
        self.cpp_info.set_property("system_package_version", "0.26.8")
        self.cpp_info.builddirs = ["share/cmake/pxr-boost"]
