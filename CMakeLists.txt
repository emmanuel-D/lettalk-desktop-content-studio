cmake_minimum_required(VERSION 3.19)
project(LetTalkContentStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets Sql)

# Workaround: Xcode 26 SDK removed the AGL framework, but Qt's FindWrapOpenGL still links it.
# Remove AGL from the WrapOpenGL target if it was added.
if(TARGET WrapOpenGL::WrapOpenGL)
    get_target_property(_wgl_libs WrapOpenGL::WrapOpenGL INTERFACE_LINK_LIBRARIES)
    if(_wgl_libs)
        list(FILTER _wgl_libs EXCLUDE REGEX "AGL")
        set_target_properties(WrapOpenGL::WrapOpenGL PROPERTIES INTERFACE_LINK_LIBRARIES "${_wgl_libs}")
    endif()
endif()

qt_standard_project_setup()

qt_add_executable(LetTalkContentStudio
    WIN32 MACOSX_BUNDLE
    main.cpp

    # Database
    src/database/databasemanager.h
    src/database/databasemanager.cpp

    # Entities
    src/entities/post.h

    # Repositories
    src/repositories/postrepository.h
    src/repositories/postrepository.cpp

    # Services
    src/services/postservice.h
    src/services/postservice.cpp

    # UI
    src/ui/mainwindow.h
    src/ui/mainwindow.cpp
    src/ui/mainwindow.ui
)

target_include_directories(LetTalkContentStudio PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(LetTalkContentStudio
    PRIVATE
        Qt::Sql
        Qt::Widgets
)

install(TARGETS LetTalkContentStudio
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET LetTalkContentStudio
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
