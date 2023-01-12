/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/


#include "support/fixture.h"
#include "gtest/gtest.h"
#include <dynamic_test.h>
#include <ghc/filesystem.hpp>

/**
 * @return std::string of the current-assignment subdirectory
 */
static std::string ass_dir() {
    if (CURRENT_ASSIGNMENT & ASS_FIVE)
        return "ass5";
    else if (CURRENT_ASSIGNMENT & ASS_FOUR)
        return "ass4";
    else if (CURRENT_ASSIGNMENT & ASS_THREE)
        return "ass3";
    else if (CURRENT_ASSIGNMENT & ASS_TWO)
        return "ass2";
    return "ass1";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    ghc::filesystem::path root_path = ghc::filesystem::canonical(ghc::filesystem::absolute(argv[0])); // Distro-dependent method to get executable-path.
    std::cerr << "Test root path is: " << root_path << std::endl;

    ghc::filesystem::path project_root_path = root_path.parent_path().parent_path().parent_path().parent_path(); // Project root path.

    ghc::filesystem::path exe_path = root_path.parent_path().parent_path().parent_path() / "src" / "machine-code" / "coco_compiler_machine_code"; // Absolute path to regular executable.
    if (!ghc::filesystem::exists(exe_path))
        throw std::runtime_error("Path does not exist: "+exe_path.string());

    ghc::filesystem::path test_path_general = project_root_path / "test" / ass_dir(); // Absolute path to general tests.
    if (!ghc::filesystem::exists(test_path_general))
        throw std::runtime_error("Path does not exist: "+test_path_general.string());

    ghc::filesystem::path test_path_grammar = project_root_path / "src" / "grammar" / "test";
    if (!ghc::filesystem::exists(test_path_grammar))
        throw std::runtime_error("Path does not exist: "+test_path_grammar.string());
    test_path_grammar /= ass_dir();

    ////////////////////// Execute correct tests //////////////////////
    test::support::register_tests<MachineCodeTest>(project_root_path, exe_path, test_path_general, "DynamicCorrectTest", {"incorrect", "warn"}, [](const std::string& exe_path, const std::string& test_path) -> MachineCodeTest* {
        return new DynamicMachineCodeCorrectTest(exe_path, test_path);
    });

    ////////////////////// Execute incorrect grammar tests //////////////////////
    // NOTE: we may not have incorrect tests for each assignment
    if (ghc::filesystem::exists(test_path_grammar)) {
        test::support::register_tests<MachineCodeTest>(project_root_path, exe_path, test_path_grammar, "DynamicGrammarIncorrectTest", {"warn", "correct"}, [](const std::string& exe_path, const std::string& test_path) -> MachineCodeTest* {
            return new DynamicMachineCodeIncorrectTest(exe_path, test_path);
        });
    }
    return RUN_ALL_TESTS();
}