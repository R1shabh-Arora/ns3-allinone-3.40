build_lib(
  LIBNAME greyattackaodv
  SOURCE_FILES
        helper/greyattackaodv-helper.cc
        model/greyattackaodv-dpd.cc
        model/greyattackaodv-id-cache.cc
        model/greyattackaodv-neighbor.cc
        model/greyattackaodv-packet.cc
        model/greyattackaodv-routing-protocol.cc
        model/greyattackaodv-rqueue.cc
        model/greyattackaodv-rtable.cc
  HEADER_FILES
        helper/greyattackaodv-helper.h
        model/greyattackaodv-dpd.h
        model/greyattackaodv-id-cache.h
        model/greyattackaodv-neighbor.h
        model/greyattackaodv-packet.h
        model/greyattackaodv-routing-protocol.h
        model/greyattackaodv-rqueue.h
        model/greyattackaodv-rtable.h
  LIBRARIES_TO_LINK ${libinternet}
                    ${libwifi}
  TEST_SOURCES
        test/greyattackaodv-id-cache-test-suite.cc
        test/greyattackaodv-regression.cc
        test/greyattackaodv-test-suite.cc
    test/loopback.cc
    test/bug-772.cc
)
