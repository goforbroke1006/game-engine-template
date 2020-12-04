cmake_minimum_required(VERSION 3.0.2)
project(game-engine-template VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

# *.lib lookup
link_directories(
        ${CMAKE_SYSTEM_LIBRARY_PATH}
        "C:/Program Files (x86)/Windows Kits/10/Lib/${CMAKE_SYSTEM_VERSION}.0/um/x86"
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Utilities/bin/x86"
)

# project sources
add_executable(${PROJECT_NAME} main.cpp
        def.h
        utils.h
        domain.h
        Application.cpp Application.h
        Scene.cpp Scene.h
        Node.cpp Node.h
        Transform.cpp Transform.h
        Mesh.cpp Mesh.h

        BaseRenderTarget.cpp BaseRenderTarget.h
        render-type/DX11RenderTarget.cpp render-type/DX11RenderTarget.h
        render-type/factory/DX11Factory.cpp render-type/factory/DX11Factory.h
        render-type/OpenGLRenderTarget.cpp render-type/OpenGLRenderTarget.h

        PrimitivesFactory.cpp PrimitivesFactory.h
        Camera.cpp Camera.h Behavior.cpp Behavior.h)

# link DirectX 11 SDK
set(DXSDK_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/")
include_directories(
        "C:/Program Files (x86)/Windows Kits/10/Include/${CMAKE_SYSTEM_VERSION}.0/um"
        "${DXSDK_DIR}/Include/"
)
target_link_libraries(${PROJECT_NAME}
        d3dcompiler d2d1 dxgi windowscodecs WindowsCodecs Dwrite
        DxErr legacy_stdio_definitions
        d3d11 d3dx11 d3dx11d)

# link OpnGL/freeglut
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} opengl32 glu32 freeglut)