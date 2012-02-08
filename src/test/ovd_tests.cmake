
ENABLE_TESTING()

#
# Python tests.
#
set( RAND_PT_CASES 128 256 512 1024 2048 4096 10000 20000)
foreach( CASE ${RAND_PT_CASES} )
    ADD_TEST(random_points_${CASE} python ../src/test/random_points.py 0 ${CASE})
endforeach()

set( LINESEG_CASES 128 256 512 1024 2048 4096)
foreach( CASE ${LINESEG_CASES} )
    ADD_TEST(random_linesegments_${CASE} python ../src/test/random_linesegments.py ${CASE})
endforeach()

set( 2OPT_RPG_CASES 5 10 15 20 30) # the number of vertices in the polygon
set( 2OPT_RPG_MAX_SEED 100) # run test for seeds 0,1,2,3,...,max-1
foreach( CASE ${2OPT_RPG_CASES} )
    ADD_TEST(2opt_random_polygon_${CASE} python ../src/test/2opt_random_polygon.py ${CASE} ${2OPT_RPG_MAX_SEED})
endforeach()

set( 2OPT_RPG_CASES 40 50 100 200) # 400 800 the number of vertices in the polygon
set( 2OPT_RPG_MAX_SEED 10) # run test for seeds 0,1,2,3,...,max-1
foreach( CASE ${2OPT_RPG_CASES} )
    ADD_TEST(2opt_random_polygon_${CASE} python ../src/test/2opt_random_polygon.py ${CASE} ${2OPT_RPG_MAX_SEED})
endforeach()

foreach( CASE RANGE 25) # characters A..Z
    ADD_TEST(ttt_single_glyph_${CASE} python ../src/test/ttt_single_glyph.py ${CASE})
endforeach()

set( TTT_ALPHA_CASES 200 100 50 25 )  # 12 6 3
# comment out the small values so that test doesn't take forever!
# cpu-times (t400s laptop):
#   25== takes 23 seconds 
#   12== takes ca 71 seconds
#    6== takes ca 384 seconds
#    3== takes ca ??  seconds
foreach( CASE ${TTT_ALPHA_CASES}) # characters A..Z
    ADD_TEST(ttt_alphabet_${CASE} python ../src/test/ttt_alphabet.py ${CASE})
endforeach()

foreach( CASE RANGE 25) # seeds 1..25. These take about 4s each to run.
    ADD_TEST(ttt_random_chars_${CASE} python ../src/test/ttt_random_chars.py ${CASE})
endforeach()

#
# C++ tests. Each test subdirectory has its own CMakeLists.txt file with
# build and test commands. Use "test_sandbox/*" as a template to make a new
# test subdirectory, like so:
#
# $ cp --recursive test_sandbox <test name>_test
#
macro(ADD_OVD_TEST test_name libs)
  # This macro was borrowed and modified from Open CASCADE Community Edition
  # (http://github.com/tpaviot/oce).

  file(GLOB TEST_SOURCES *.cpp)
  set(SOURCE_FILES ${TEST_SOURCES})
  add_executable(${test_name} ${SOURCE_FILES})

  foreach(LIB ${libs})
     target_link_libraries(${test_name} ${LIB})
  endforeach(LIB ${libs})
  target_link_libraries(${test_name} openvoronoi_static ${QD_LIBRARY})

  ADD_TEST(${test_name}  ${test_name})
endmacro(ADD_OVD_TEST test_name libs)

include_directories(${OpenVoronoi_SOURCE_DIR})
file(GLOB TEST_SUBDIRS test/*_test test/test_sandbox)
subdirs(${TEST_SUBDIRS})
