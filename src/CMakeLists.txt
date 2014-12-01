CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(BEFORE include)

# Source files
SET(SRC_LIST
			qbencodedocument.cpp
			qbencodevalue.cpp
			qbencodeparser.cpp
			qbencodewriter.cpp
)
SET(PUBLIC_HEADERS
			include/qbencodedocument.h
			include/qbencodevalue.h
			include/QBencodeDocument
			include/QBencodeValue
)

# Build targets
ADD_LIBRARY(QBencode SHARED ${SRC_LIST})
ADD_LIBRARY(QBencode_static STATIC ${SRC_LIST})
SET_TARGET_PROPERTIES(QBencode PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(QBencode_static PROPERTIES OUTPUT_NAME QBencode)

#TARGET_LINK_LIBRARIES(QBencode Qt5::Core)
#TARGET_LINK_LIBRARIES(QBencode_static Qt5::Core)
qt5_use_modules(QBencode Core)
qt5_use_modules(QBencode_static Core)

# Install phase
INSTALL(TARGETS QBencode QBencode_static
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
INSTALL(FILES ${PUBLIC_HEADERS} DESTINATION include/QBencode)
