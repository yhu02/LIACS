/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_TESTUTILS_DYNAMICTEST
#define COCO_FRAMEWORK_TESTUTILS_DYNAMICTEST

#include <string>
#include <unordered_set>
#include <vector>
#include <ghc/filesystem.hpp>
#include <picosha2.h>
#include <gtest/gtest.h>

// https://github.com/google/googletest/blob/master/docs/advanced.md
// https://stackoverflow.com/questions/19160244/create-tests-at-run-time-google-test
namespace test {
    namespace support {
        namespace internal {
            static std::string dirname(const ghc::filesystem::path& path) {
                auto path_str = ghc::filesystem::is_directory(path) ? path.generic_string() : path.parent_path().generic_string();
                size_t found = path_str.find_last_of("/\\");
                return path_str.substr(found + 1);
            }

            inline std::vector<std::string> test_iterator(const std::string& root, const std::unordered_set<std::string>& excluded_dirs) {
                std::vector<std::string> paths;
                auto iterator = ghc::filesystem::recursive_directory_iterator(root);
                for (decltype(iterator) end; iterator != end; ++iterator) {
                    auto p = iterator->path();
                    if (ghc::filesystem::is_directory(p)) {                        //exclude directories from output.
                        if (excluded_dirs.find(dirname(p)) != excluded_dirs.end()) // Skip directory if in exclusion dirnames.
                            iterator.disable_recursion_pending();
                        continue;
                    }
                    if (p.extension() != ".c") // We are not dealing with a c-minus file.
                        continue;
                    // We have a file
                    paths.emplace_back(p.generic_string());
                }
                return paths;
            }

            inline std::vector<std::string> test_iterator(const std::string& root) {
                std::vector<std::string> paths;
                auto iterator = ghc::filesystem::recursive_directory_iterator(root);
                for (decltype(iterator) end; iterator != end; ++iterator) {
                    auto p = iterator->path();
                    if (ghc::filesystem::is_directory(p)) //exclude directories from output
                        continue;
                    // We have a file
                    paths.emplace_back(p.generic_string());
                }
                return paths;
            }
        }

        /**
         * Adds a test for each file found in the file tree pointed to by `test_path`.
         * @tparam T Type of Returned fixture.
         * @param project_root_path Path to the project root. Used to calculate non-conflicting names for tests with equivalent names.
         * @param exe_path Executable to use for segfault tests.
         * @param test_path Root directory to search.
         * @param fixtureName Name of the fixture we register.
         * @param excluded_dirs Names of directories to exclude from search. Upon encounter, these directories will not be expanded.
         * @param factory A function/lambda that takes an absolute test_path to a c-minus test, and produces a Fixture.
         */
        // https://stackoverflow.com/questions/19160244/create-tests-at-run-time-google-test
        template<typename T>
        inline void register_tests(const std::string& project_root_path, const std::string& exe_path, const std::string& test_path, const std::string& fixtureName, const std::unordered_set<std::string>& excluded_dirs, T* (*factory)(const std::string&, const std::string&) ) {
            const std::vector<std::string> tp = ::test::support::internal::test_iterator(test_path, excluded_dirs);
            for (const auto& file: tp) {
                const auto stripped_path = ghc::filesystem::proximate(ghc::filesystem::path(file).parent_path(), ghc::filesystem::path(project_root_path));
                const auto unique_testname = ghc::filesystem::path(file).stem().string()+"_"+picosha2::hash256_hex_string(stripped_path.string());
                ::testing::RegisterTest(fixtureName.c_str(), unique_testname.c_str(), nullptr, file.c_str(), __FILE__, __LINE__, [factory, exe_path, file]() -> T* {
                    return factory(exe_path, file);
                });
            }
        }

        /**
         * Adds a test for each file found in the file tree pointed to by test_path.
         * @tparam T Type of Returned fixture.
         * @param project_root_path Path to the project root. Used to calculate non-conflicting names for tests with equivalent names.
         * @param exe_path Executable to use for segfault tests.
         * @param test_path Root directory to search.
         * @param fixtureName Name of the fixture we register.
         * @param factory A function/lambda that takes an absolute test_path to a c-minus test, and produces a Fixture.
         */
        template<typename T>
        inline void register_tests(const std::string& project_root_path, const std::string& exe_path, const std::string& test_path, const std::string& fixtureName, T* (*factory)(const std::string&, const std::string&) ) {
            register_tests(exe_path, test_path, fixtureName, {}, factory);
        }
    }
}

#endif