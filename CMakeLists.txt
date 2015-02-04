CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(QBencode)

IF(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
ELSEIF(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF(UNIX)

FIND_PACKAGE(Qt5Core REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})

#ADD_SUBDIRECTORY(app bin)
ADD_SUBDIRECTORY(src lib)
ADD_SUBDIRECTORY(tests tests)

INSTALL(FILES LICENSE DESTINATION share/licenses)

# additional target to run test after every build
IF(UNIX)
    SET(test_command "tests/runtests.sh")
ELSEIF(WIN32)
    SET(test_command "tests/runtests.bat")
ENDIF(UNIX)
ADD_CUSTOM_TARGET(runtests ALL
                  COMMAND ${test_command}
				  DEPENDS tests/tests
				  COMMENT "Running tests...")
