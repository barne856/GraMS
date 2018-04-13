LIBS = -lGL -lglfw3 -lGLU -lGLEW -lpthread -lX11 -ldl -lXcursor -lXinerama -lXxf86vm -lXrandr -lsndfile `pkg-config --libs opencv`
LIBDIR = -L ./dep/lib -L /usr/local/lib
INCLUDEDIR = -I ./dep/include
SOURCEDIR = ./src/
OUTPUTDIR = ./bin/
DEPS = $(SOURCEDIR)main.cpp $(SOURCEDIR)Shader.cpp $(SOURCEDIR)GLCall.cpp

run: buildmain
	cd $(OUTPUTDIR); ./GraMS

buildmain: $(DEPS)
	g++ -o $(OUTPUTDIR)GraMS $(DEPS) $(INCLUDEDIR) $(LIBDIR) $(LIBS)