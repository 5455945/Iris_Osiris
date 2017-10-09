PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/home/Nadia/opencv2.4.5/lib/pkgconfig/
export PKG_CONFIG_PATH

all : OsiMain.cpp OsiManager.cpp OsiEye.cpp OsiProcessings.cpp OsiCircle.cpp
	g++ OsiMain.cpp OsiManager.cpp OsiEye.cpp OsiProcessings.cpp OsiCircle.cpp -o osiris `pkg-config opencv --cflags --libs`
	
clean : osiris
	rm *[~o]
	