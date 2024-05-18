check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/shared_vars-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME shared_vars
    SOURCE_FILES model/shared_vars.cc
                 helper/shared_vars-helper.cc
    HEADER_FILES model/shared_vars.h
                 helper/shared_vars-helper.h
    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES test/shared_vars-test-suite.cc
                 ${examples_as_tests_sources}
)

