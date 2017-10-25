cmake_minimum_required (VERSION 3.6)

project (Osiris)

set(srcs 
    src/OsiCircle.cpp
	src/OsiEye.cpp
	src/OsiMain.cpp
	src/OsiManager.cpp
	src/OsiProcessings.cpp
	)
set(incs
    src/OsiCircle.h
	src/OsiEye.h
	src/OsiManager.h
	src/OsiProcessings.h
	src/OsiStringUtils.h
	)

find_package(OpenCV QUIET)
if (OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})

  add_executable(Osiris ${srcs} ${incs})
  target_link_libraries(Osiris ${OpenCV_LIBS} )
else()
  message("OpenCV not found, so we won't build the Osiris.")
endif()

if (MSVC)
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user")
		file(GENERATE
			OUTPUT "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user"
			INPUT "${CMAKE_SOURCE_DIR}/cmake/ALL_BUILD.vcxproj.user.in")
	endif()
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/Osiris.vcxproj.user")
		file(GENERATE
			OUTPUT "${CMAKE_BINARY_DIR}/Osiris.vcxproj.user"
			INPUT "${CMAKE_SOURCE_DIR}/cmake/Osiris.vcxproj.user.in")
	endif()
endif()
