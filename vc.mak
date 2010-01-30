%.o : PATH += :/c/media/common/tools/msvc8/VC/bin\:/c/media/common/tools/msvc8/Common7/IDE
make.exe : PATH += :/c/media/common/tools/msvc8/VC/bin\:/c/media/common/tools/msvc8/Common7/IDE

%.o : %.cc
	cl /nologo /EHsc -c $< /Fo$@ /Ic:\\media\\common\\tools\\msvc8\\VC\\include

%.o : %.c
	cl /nologo /EHsc -c $< /Fo$@ /Ic:\\media\\common\\tools\\msvc8\\VC\\include

make.exe : $(OBJS)
	/c/media/common/tools/msvc8/VC/bin/link /DEBUG /NOLOGO /LIBPATH:c:\\media\\common\\tools\\msvc8\\VC\\lib /SUBSYSTEM:CONSOLE /DYNAMICBASE:NO /MACHINE:X86 /LIBPATH:c:\\media\\common\\tools\\msvc8\\VC\\PlatformSDK\\Lib /PDB:make.pdb /out:$@ $^
