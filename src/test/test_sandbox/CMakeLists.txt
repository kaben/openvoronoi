
# The next line tells CMake and CTest about "test_sandbox".
ADD_OVD_TEST(test_sandbox "")


# If you need to link additional libraries, replace the empty quotes "" with
# the names of the libraries, like so:
#ADD_OVD_TEST(test_sandbox "my_lib my_other_lib")


# The easiest way to check conditions and run test variants is to put the
# checks and variants in the source code.  But you can also ask CTest to
# parse any test output using regular expressions, like so:
set_tests_properties(test_sandbox
  PROPERTIES
  PASS_REGULAR_EXPRESSION
  "VoronoiDiagram"
  PASS_REGULAR_EXPRESSION
  " *num_vertices *= *13"
  PASS_REGULAR_EXPRESSION
  " *num_edges *= *21"
  PASS_REGULAR_EXPRESSION
  " *num_point_sites *= *1"
  PASS_REGULAR_EXPRESSION
  " *num_line_sites *= *0"
)
# Other available test properties include WILL_FAIL, FAIL_REGULAR_EXPRESSION,
# and TIMEOUT". For more details, see CMake's documentation for the
# "set_tests_properties" command.


# After calling ADD_OVD_TEST (which automatically calls the test program
# without any arguments), you can add additional test cases, each with their
# own arguments, like so:
ADD_TEST(test_sandbox_blah_blah_blah test_sandbox "blah, blah, blah")
