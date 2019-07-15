import os
from datetime import datetime

path = os.getcwd()
src = "src"
tests = "tests"
cmake = "CMakeLists.txt"
host = os.getlogin()
now = datetime.today().strftime('%Y-%m-%d')

print("Current working directory %s" % path)

# print current projects
print("Current source projects: ")
for d in next(os.walk(os.path.join(path, src)))[1]:
    print("\t", d)

# print current projects
print("Current source projects - tests: ")
for d in next(os.walk(os.path.join(path, tests)))[1]:
    print("\t", d)


class ProjectBuilder:
    def __init__(self, lower, upper, camel):
        self.lower = lower
        self.upper = upper
        self.camel = camel

    def append_to_cmake(self):
        # add_subdirectory to CMakeFile
        cmake_string = ("\n"
                        "add_subdirectory({1}/{0})\n"
                        "add_subdirectory({2}/{0}_test)\n"
                        ).format(self.lower, src, tests)
        with open("CMakeLists.txt", "a") as cmake_file:
            cmake_file.write(cmake_string)

    def create_src(self):
        src_path = os.path.join(path, src, self.lower)
        os.mkdir(src_path)
        cmake_string = ("cmake_minimum_required(VERSION 3.10)\n"
                        "project({0})\n"
                        "\n"
                        "set(CMAKE_CXX_STANDARD 17)\n"
                        "\n"
                        "set({1}_SOURCE_FILES\n"
                        "        {2}.cpp {2}.h)\n"
                        "\n"
                        "add_library({0} ${{{1}_SOURCE_FILES}})\n").format(self.lower, self.upper, self.camel)
        with open(os.path.join(src_path, cmake), 'w+') as f:
            f.write(cmake_string)

        h_string = ("//\n"
                    "// Created by {0} on {1}.\n"
                    "//\n"
                    "\n"
                    "#ifndef STUNNING_JOURNEY_{2}_H\n"
                    "#define STUNNING_JOURNEY_{2}_H  \n"
                    "\n"
                    "class {3} {{\n"
                    "\n"
                    "}}; \n"
                    "\n"
                    "#endif //STUNNING_JOURNEY_{2}_H\n"
                    ).format(host, now, self.upper, self.camel)
        with open(os.path.join(src_path, self.camel + ".h"), 'w+') as f:
            f.write(h_string)

        cpp_string = ("//\n"
                      "// Created by {0} on {1}.\n"
                      "//\n"
                      "#include \"{2}.h\"\n"
                      ).format(host, now, self.camel)
        with open(os.path.join(src_path, self.camel + ".cpp"), 'w+') as f:
            f.write(cpp_string)

    def create_test(self):
        tests_path = os.path.join(path, tests, self.lower + "_test")
        os.mkdir(tests_path)

        cmake_string = ("cmake_minimum_required(VERSION 3.10)\n"
                        "project({0})\n"
                        "\n"
                        "enable_testing()\n"   
                        "include_directories(${{gtest_SOURCE_DIR}}/include ${{gtest_SOURCE_DIR}})\n"
                        "set({1}_TEST_FILES {2}_tests.cpp)\n"
                        "add_executable(run{3}Tests ${{{1}_TEST_FILES}})\n"
                        "target_link_libraries(run{3}Tests gtest gtest_main)\n"
                        "target_link_libraries(run{3}Tests {2})\n"
                        ).format(tests, self.upper, self.lower, self.camel)
        with open(os.path.join(tests_path, cmake), 'w+') as f:
            f.write(cmake_string)

        cpp_string = ("//\n"
                      "// Created by {0} on {1}.\n"
                      "//\n"
                      "#include \"{3}/{2}.h\"\n"
                      "#include \"gtest/gtest.h\"\n"
                      "\n"
                      "\n"
                      "TEST(basic_{3}_check, test_eq){{\n"
                      "\tEXPECT_EQ(1, 1);\n"
                      "}}\n"
                      "\n"
                      "TEST(basic_{3}_check, test_neq){{\n"
                      "\tEXPECT_NE(1, 0);\n"
                      "}}\n"
                      "\n"
                      ).format(host, now, self.camel, self.lower)
        with open(os.path.join(tests_path, self.lower + "_tests.cpp"), 'w+') as f:
            f.write(cpp_string)

    def create_project(self):
        self.append_to_cmake()
        self.create_src()
        self.create_test()


if __name__ == "__main__":
    name = {
        "lower": "bike_racers",
        "upper": "BIKE_RACERS",
        "camel": "BikeRacers",
    }
    p = ProjectBuilder(**name)
    p.create_project()
