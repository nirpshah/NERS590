
output: Reaction.o Point.o Surface.o Cell.o Nuclide.o Random.o Distribution.o Particle.o Material.o Main.o
	g++ Reaction.o Point.o Surface.o Cell.o Nuclide.o Random.o Distribution.o Particle.o Material.o Main.o -o output
	
Reaction.o: Reaction.cpp Reaction.h
	g++ -c Reaction.cpp -O3 -std=c++11
	
Point.o: Point.cpp Point.h
	g++ -c Point.cpp -O3 -std=c++11
	
Surface.o: Surface.cpp Surface.h
	g++ -c Surface.cpp -O3 -std=c++11
	
Cell.o: Cell.cpp Cell.h
	g++ -c Cell.cpp -O3 -std=c++11
	
Nuclide.o: Nuclide.cpp Nuclide.h
	g++ -c Nuclide.cpp -O3 -std=c++11

Random.o: Random.cpp Random.h
	g++ -c Random.cpp -O3 -std=c++11

Distribution.o: Distribution.cpp Distribution.h
	g++ -c Distribution.cpp -O3 -std=c++11
	
Particle.o: Particle.cpp Particle.h
	g++ -c Particle.cpp -O3 -std=c++11
	
Material.o: Material.cpp Material.h
	g++ -c Material.cpp -O3 -std=c++11
	
Main.o: Main.cpp
	g++ -c Main.cpp -O3 -std=c++11
	
clean:
	rm *.o output