arbitrage folder
	This contains the dll project that generates the .lib and .dll file (under x64\Debug)
	Change the following: (Choose Debug and x64, because I am using a win64 system; if you are using a win32/win86 system, change correspondingly)
		Configuration Properties -- Configuration types: choose Dynamic Library(.dll)
		C/C++ -- General -- Additional Include Directories: C:\gurobi810\win64\include
		C/C++ -- General -- Debug Information Format: choose C7 Compatible
		Linker -- General --  additional Library Directories: C:\gurobi810\win64\lib
		Input -- Input -- Additional Dependencies: gurobi_c++mdd2017.lib; gurobi81.lib
		Module Definition File -- arbitrage.def
	To run: Build. And you will see a .dll and a .lib file appeared under x64\Debug
		copy the .lib file to C:\mydlls
		copy the .dll file to C:\mydlls
		remember to add C:\mydlls to your environment variable --path
		If you want the copy to be done automatically,
			in build events -- post build events -- command line: 
			copy /Y "$(SolutionDir)$(Platform)\$(Configuration)\arbitrage.dll" "C:\mydlls\arbitrage.dll" 
			copy /Y "$(SolutionDir)$(Platform)\$(Configuration)\arbitrage.lib" "C:\mydlls\arbitrage.lib"
		
		
Projectt5 folder
	This contains the main project that reads arb.dat and calls the dll we just generated
	Change the following: (Also under Debug and x64)
		Configuration Properties -- Configuration types: choose Application(.exe)
		VC++ Directories -- Executable directories: Add C:\mydlls
		VC++ Directories -- Library directories: Add C:\mydlls
		C/C++ -- Debug Information Format: choose C7 Compatible
		Linker -- Input-- Additional Dependencies: arbitrage.lib 
	Then Build and Run.
	
	Arbitrage example: 
		in Project5, main function: retcode = readit("notypeA.dat", &n, &K, &r, &N, &prices);
	No-Arb example:
	    in Project5, main function: retcode = readit("arb.dat", &n, &K, &r, &N, &prices);