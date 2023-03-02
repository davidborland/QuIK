CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( QuIK )


#######################################
# Include QuIK code
#######################################

SET( SRC QuIK.h QuIK.cpp
         Vec3.h Vec3.cpp
         Sphere.h Sphere.cpp
         SphereInterior.h SphereInterior.cpp 
		 SphereExterior.h SphereExterior.cpp )

ADD_LIBRARY( QuIK ${SRC} )

OPTION( QuIK_BUILD_QuIKVis
        "Build QuiKVis application."
        ON )
		
	
#######################################
# Include application code
#######################################

IF( QuIK_BUILD_QuIKVis )
  ADD_SUBDIRECTORY( QuIKVis )
ENDIF( QuIK_BUILD_QuIKVis )