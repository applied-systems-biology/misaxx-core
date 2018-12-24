from conans import ConanFile, CMake


class MisaxxCoreConan(ConanFile):
    name = "misaxx-core"
    version = "0.1.0"
    license = "MIT"
    author = "Ruman Gerst ruman.gerst@leibniz-hki.de"
    url = "https://asb-git.hki-jena.de/RGerst/misaxx-core"
    description = "MISA++ framework core framework"
    topics = ("MISA++", "utility", "opencv", "images")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=True"
    generators = "cmake"

    def requirements(self):
        self.requires.add('boost/1.68.0@conan/stable')
        self.requires.add('jsonformoderncpp/3.3.0@vthiery/stable')
        self.requires.add('misaxx-helpers/0.1.0@rgerst/stable')
        self.requires.add('misaxx-coixx/0.1.0@rgerst/stable')
        
    def configure(self):
        self.options["opencv"].contrib = True

    def source(self):
        self.run("git clone https://asb-git.hki-jena.de/RGerst/misaxx-core.git")

    def configure_cmake(self):        
        cmake = CMake(self)
        cmake.definitions["WITH_CONAN"] = True
        cmake.configure(source_folder="misaxx-core")

        return cmake
      
    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["misaxx-core"]

