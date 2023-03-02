CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( QuIKVis )

#SET( EXECUTABLE_OUTPUT_PATH "${QuIK_BINARY_DIR}/bin" )


#######################################
# Include Qt
#######################################

FIND_PACKAGE( Qt4 REQUIRED )

SET( QT_USE_QTOPENGL TRUE )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR} )

# Set up variables for moc
SET( QT_UI qvMainWindow.ui )
SET( QT_HEADER qvMainWindow.h qvGLWidget.h )
SET( QT_SRC QuIKVis.cpp qvMainWindow.cpp qvGLWidget.cpp )

# Do moc stuff
QT4_WRAP_UI( QT_UI_HEADER ${QT_UI} )
QT4_WRAP_CPP( QT_MOC_SRC ${QT_HEADER} )
ADD_DEFINITIONS( -DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT )
SET_SOURCE_FILES_PROPERTIES( ${QT_SRC} PROPERTIES OBJECT_DEPENDS "${QT_UI_HEADER}" )


#######################################
# Include QuIK
#######################################

INCLUDE_DIRECTORIES( ${QuIK_SOURCE_DIR} )
LINK_DIRECTORIES( ${QuIK_BINARY_DIR} )
SET( QuIK_LIB QuIK )


#######################################
# Include QuIKVis code
#######################################

#SET( SRC overlap_vp.glsl overlap_fp.glsl )
SET( SRC )

ADD_EXECUTABLE( QuIKVis ${QT_HEADER} ${QT_SRC} ${QT_MOC_SRC} ${SRC} )
ADD_DEPENDENCIES( QuIKVis QuIK )
TARGET_LINK_LIBRARIES( QuIKVis opengl32 ${QT_LIBRARIES} ${QuIK_LIB} )