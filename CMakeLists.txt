cmake_minimum_required (VERSION 3.6)

project (Osiris)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_AFXDLL")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

set(srcs 
    src/OsiCircle.cpp
	src/OsiEye.cpp
	src/OsiManager.cpp
	src/OsiProcessings.cpp
	src/OsirisMfc.cpp
	src/OsirisMfcDlg.cpp
	src/OsirisMfc.rc
	)
set(incs
    src/OsiCircle.h
	src/OsiEye.h
	src/OsiManager.h
	src/OsiProcessings.h
	src/OsirisMfc.h
	src/OsirisMfcDlg.h
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
		 

file(COPY "${CMAKE_SOURCE_DIR}/data/input" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/data/output" DESTINATION "${CMAKE_BINARY_DIR}")

		 
if(MSVC AND NOT EXISTS "${CMAKE_BINARY_DIR}/Osiris.vcxproj.user")
	file(GENERATE
		OUTPUT "${CMAKE_BINARY_DIR}/Osiris.vcxproj.user"
		INPUT "${CMAKE_SOURCE_DIR}/cmake/Osiris.vcxproj.user.in")
endif()
if(MSVC AND NOT EXISTS "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user")
	file(GENERATE
		OUTPUT "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user"
		INPUT "${CMAKE_SOURCE_DIR}/cmake/ALL_BUILD.vcxproj.user.in")
endif()
