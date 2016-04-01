########## VARIABLES/MACROS ##########

# Compiler
CXX = cl

# Compiler flags (all warnings)
CXXFLAGS = /c /EHsc /Wall
COPY = copy
DELETE = del
RENAME = rename

# Project directories
SDFFILES = .
CPPFILES = ..\..\lib
OBJFILES = ..\..\obj
HFILES = ..\..\include

# Executable name
SYSTEMNAME = qpsk_transmitter

# Object files
OBJS = $(OBJFILES)\$(SYSTEMNAME)_sdf.obj $(OBJFILES)\netplus.obj $(OBJFILES)\binary_source.obj $(OBJFILES)\m_qam_mapper.obj $(OBJFILES)\pulse_shaper.obj 

########## DEPENDENCIES + RULES ##########
$(SYSTEMNAME).exe: $(OBJS)
	$(CXX) $(OBJS) 
	$(RENAME) $(SYSTEMNAME)_sdf.exe $(SYSTEMNAME).exe

$(OBJFILES)\$(SYSTEMNAME)_sdf.obj: $(SDFFILES)\$(SYSTEMNAME)_sdf.cpp $(HFILES)\netplus.h $(HFILES)\netplus_lib.h
	$(CXX) $(CXXFLAGS) $(SDFFILES)\$(SYSTEMNAME)_sdf.cpp /I$(HFILES)
	$(COPY) $(SYSTEMNAME)_sdf.obj $(OBJFILES)
	$(DELETE) $(SYSTEMNAME)_sdf.obj
	
$(OBJFILES)\netplus.obj: $(CPPFILES)\netplus.cpp $(HFILES)\netplus.h $(HFILES)\netplus_lib.h
	$(CXX) $(CXXFLAGS) $(CPPFILES)\netplus.cpp /I$(HFILES)
	$(COPY) netplus.obj $(OBJFILES)
	$(DELETE) netplus.obj
		
$(OBJFILES)\binary_source.obj: $(CPPFILES)\binary_source.cpp $(HFILES)\netplus.h $(HFILES)\netplus_lib.h
	$(CXX) $(CXXFLAGS) $(CPPFILES)\binary_source.cpp /I$(HFILES) 
	$(COPY) binary_source.obj $(OBJFILES)
	$(DELETE) binary_source.obj 

$(OBJFILES)\m_qam_mapper.obj: $(CPPFILES)\m_qam_mapper.cpp $(HFILES)\netplus.h $(HFILES)\netplus_lib.h
	$(CXX) $(CXXFLAGS) $(CPPFILES)\m_qam_mapper.cpp /I$(HFILES) 
	$(COPY) m_qam_mapper.obj $(OBJFILES)
	$(DELETE) m_qam_mapper.obj

$(OBJFILES)\pulse_shaper.obj: $(CPPFILES)\pulse_shaper.cpp $(HFILES)\netplus.h $(HFILES)\netplus_lib.h
	$(CXX) $(CXXFLAGS) $(CPPFILES)\pulse_shaper.cpp /I$(HFILES)
	$(COPY) pulse_shaper.obj $(OBJFILES)
	$(DELETE) pulse_shaper.obj

# nmake clean	
clean:
	del $(OBJS) $(SYSTEMNAME).exe 

# nmake run
run:
	.\$(SYSTEMNAME).exe	