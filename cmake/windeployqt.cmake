find_package(Qt${QT_VERSION_MAJOR}Core REQUIRED)

# get absolute path to qmake, then use it to find windeployqt executable
get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

function(windeployqt target)

    # POST_BUILD step
    # - after build, we have a bin/lib for analyzing qt dependencies
    # - we run windeployqt on target and deploy Qt libs
    #create a windeployqt dir
    cmake_path(SET currBinPath ${CMAKE_CURRENT_BINARY_DIR})
    cmake_path(GET currBinPath PARENT_PATH currBinPathParent)
    #file(MAKE_DIRECTORY ${qtLibDir}/windeployqt)
    add_custom_command(TARGET ${target} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${currBinPathParent}/windeployqt/${CMAKE_BUILD_TYPE})
    cmake_path(SET qtLibDir ${currBinPathParent}/windeployqt/${CMAKE_BUILD_TYPE})
    message("---windeployqt target:" ${target} ">>>" "windeployqt libs to be copied to: \"" ${qtLibDir} "\"  during build...")

    
   	#decide if windeploy for debug. buildType: 0 -> debug, 1 -> release
    set(buildType 1) 
    if(DEFINED ENV{CMAKE_BUILD_TYPE})
    	 message("---windeployqt target:" ${target} ">>>" "CMAKE_BUILD_TYPE = " $ENV{CMAKE_BUILD_TYPE})
    	 if($ENV{CMAKE_BUILD_TYPE} STREQUAL Debug)
    	 	set(buildType 0)
    	 endif()
    endif()
    message("---windeployqt target:" ${target} ">>>" "buildType = " ${buildType})


	#call windeployqt command and place dlls into the temporary dir
    if(buildType EQUAL 0)
        message("---windeployqt target:" ${target} ">>>" "Deploy for debug config")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${_qt_bin_dir}/windeployqt.exe" 
                    --debug 
                    --dir ${qtLibDir}       
                    --verbose 1
                    --release
                    --no-svg
                    --no-angle
                    --no-opengl
                    --no-opengl-sw
                    --no-compiler-runtime
                    --no-system-d3d-compiler
                    --no-translations
                    \"$<TARGET_FILE:${target}>\"
            COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
        )
        #delete temporary windeployqt dir
        #add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E rm "windeployqt")

    else()
        message("---windeployqt target:" ${target} ">>>" "Deploy for release config")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${_qt_bin_dir}/windeployqt.exe"  
                    --dir ${qtLibDir} 
                    --verbose 1
                    --release
                    --no-svg
                    --no-angle
                    --no-opengl
                    --no-opengl-sw
                    --no-compiler-runtime
                    --no-system-d3d-compiler
                    --no-translations
                    \"$<TARGET_FILE:${target}>\"
            COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
        )
    endif()

    # copy deployment directory during installation
    install(
        DIRECTORY ${qtLibDir}/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
endfunction()