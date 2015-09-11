#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Atlas.o \
	${OBJECTDIR}/Bsp.o \
	${OBJECTDIR}/BspRenderer.o \
	${OBJECTDIR}/Camera.o \
	${OBJECTDIR}/Color.o \
	${OBJECTDIR}/Engine.o \
	${OBJECTDIR}/Entity.o \
	${OBJECTDIR}/Font.o \
	${OBJECTDIR}/Main.o \
	${OBJECTDIR}/Shader.o \
	${OBJECTDIR}/ShaderManager.o \
	${OBJECTDIR}/TextRenderer.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Llib/GLFW -Llib/GLEW -Linclude -Llib/Freetype -lglfw3 -lglew32 -lopengl32 -lglu32 -lgdi32 -lfreetype

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/openbsp.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/openbsp.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/openbsp ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Atlas.o: Atlas.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Atlas.o Atlas.cpp

${OBJECTDIR}/Bsp.o: Bsp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Bsp.o Bsp.cpp

${OBJECTDIR}/BspRenderer.o: BspRenderer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BspRenderer.o BspRenderer.cpp

${OBJECTDIR}/Camera.o: Camera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Camera.o Camera.cpp

${OBJECTDIR}/Color.o: Color.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Color.o Color.cpp

${OBJECTDIR}/Engine.o: Engine.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Engine.o Engine.cpp

${OBJECTDIR}/Entity.o: Entity.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Entity.o Entity.cpp

${OBJECTDIR}/Font.o: Font.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Font.o Font.cpp

${OBJECTDIR}/Main.o: Main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Main.o Main.cpp

${OBJECTDIR}/Shader.o: Shader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Shader.o Shader.cpp

${OBJECTDIR}/ShaderManager.o: ShaderManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ShaderManager.o ShaderManager.cpp

${OBJECTDIR}/TextRenderer.o: TextRenderer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_DEBUG -Iinclude -Iglm -Iinclude/Freetype -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TextRenderer.o TextRenderer.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/openbsp.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
