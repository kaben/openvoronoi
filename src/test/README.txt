Running tests

The Python and C++ tests can be run with CTest.  In the top-level
build-directory either "make test" or "ctest" will run all tests.  You can
run only tests that have e.g. "ttt" in the test-name with
  $ ctest -R ttt
or
  $ make test ARGS="-R ttt"


Notes

- Currently the tests do not produce any output (png or svg output could be
  an option?)
- The Python tests cannot be run without first calling "sudo make install".
  And if the Python tests are to respond to recoding in OVD, OVD must first
  be reinstalled.  This means that developers will usually have an unstable
  version of OVD installed in "/usr/" or "/usr/local/".  This seems to be on
  purpose, in order to verify that the packaging system works.  Can we
  somehow test packaging separately, and only in that case use the installed
  version? Or can we stage and test the install in isolation using fakeroot,
  or something along those lines?


Adding new Python tests

Python test scripts are pretty simple to write: they call "exit(0)" to
indicate success, and "exit(-1)" to indicate failure.  Adding a new Python
test script is also simple: place the new script beneath "src/test/", and
tell CTest about new test scripts by adding something like the following to
"src/test/ovd_tests.cmake", in the "Python tests" section:
  ADD_TEST(<test name> python ../src/test/<test file>.py)

Don't forget to git add (I almost always forget):
  $ git add "src/test/<test file>.py"


Adding new C++ tests

C++ tests are also easy to write: an "int main()" function returns "0" for
success, and "-1" for failure.  Each C++ test has its own subdirectory ending
with "_test", which can contain any number of source files ending in ".cpp",
all of which will be compiled and used to build the new test.  Each "_test"
subdirectory should also contain a CMakeLists.txt with at least the following
line:
  ADD_OVD_TEST(<test name> "")

The easiest way to add a C++ test is to make a copy of the template
"test_sandbox/*" subdirectory, like so:
  $ cd src/test/
  $ cp --recursive test_sandbox <test name>_test

and then edit "<test name>_test/CMakeLists.txt", replacing the word
"test_sandbox" with the name of the new test.

Don't forget to git add:
  $ git add "src/test/<test name>_test/CMakeLists.txt"
  $ git add "src/test/<test name>_test/*.cpp"


Hints

CMake and CTest have lots of tools to make testing easier, some of which are
demonstrated in "src/test/ovd_tests.cmake" and in the CMakeLists.txt files of
the various C++ test subdirectories.

To recode, rebuild, and run a single test (say, "test_sandbox"), repeat ad
nauseum:
  $ make all test ARGS="-R test_sandbox"

CTest normally suppresses test output.  If you'd like to see test output, ask
CTest to be verbose:
  $ make all test ARGS="-VV -R test_sandbox"
