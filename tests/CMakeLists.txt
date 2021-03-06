CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

INCLUDE_DIRECTORIES(BEFORE SYSTEM gtest)
INCLUDE_DIRECTORIES(BEFORE ../src)
INCLUDE_DIRECTORIES(BEFORE ../src/include)

# Source files
AUX_SOURCE_DIRECTORY(gtest GTEST_SRC)
SET(TEST_FILES
				test_qbencodeparser.cpp
				test_qbencodewriter.cpp
)

# Build targets
ADD_EXECUTABLE(tests ${GTEST_SRC} ${TEST_FILES})
TARGET_LINK_LIBRARIES(tests QBencode)
IF(UNIX)
    TARGET_LINK_LIBRARIES(tests pthread)
ENDIF(UNIX)
#TARGET_LINK_LIBRARIES(tests Qt5::Core)
qt5_use_modules(tests Core)

# Install phase

# Copy test runner wrapper script to build directory
IF(UNIX)
	MESSAGE(STATUS "Install runtests.sh")
    CONFIGURE_FILE(runtests.sh runtests.sh COPYONLY)
ELSEIF(WIN32)
	MESSAGE(STATUS "Install runtests.bat")
    CONFIGURE_FILE(runtests.bat runtests.bat COPYONLY)
ENDIF(UNIX)

# Copy test data files
MESSAGE(STATUS "Install test data files")
CONFIGURE_FILE(data/resume.dat data/resume.dat COPYONLY)
