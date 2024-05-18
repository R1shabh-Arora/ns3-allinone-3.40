build_lib_example(
  NAME greyattackaodv
  SOURCE_FILES greyattackaodv.cc
  LIBRARIES_TO_LINK
    ${libwifi}
    ${libinternet}
    ${libgreyattackaodv}
    ${libinternet-apps}
)
