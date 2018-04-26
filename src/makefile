#PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/home/Nadia/opencv2.4.5/lib/pkgconfig/
#export PKG_CONFIG_PATH
all:main.cpp	
	 g++ -O2 main.cpp OsiManager.cpp OsiEye.cpp OsiProcessings.cpp OsiCircle.cpp -o osiris `pkg-config opencv --cflags --libs` -lsqlite3
#clean : osiris
#	rm *[~o]
	
