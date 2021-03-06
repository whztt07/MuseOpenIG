set( LIB_NAME IgPlugin-SkyDome )
set( TARGET_SRC_FILES igpluginskydome.cpp )
set( TARGET_OTHER_FILES ConfigFiles/libIgPlugin-SkyDome.so.xml
                        ConfigFiles/libIgPlugin-SkyDome.so.windows.xml )

add_library( ${LIB_NAME} SHARED
             ${TARGET_SRC_FILES}
             ${TARGET_OTHER_FILES} )

target_link_libraries( ${LIB_NAME}
    ${OSG_LIBRARIES}
    OpenIG
)

set_target_properties( ${LIB_NAME} PROPERTIES VERSION ${OPENIG_VERSION} )
set_target_properties( ${LIB_NAME} PROPERTIES SOVERSION ${OPENIG_VERSION} )
set_target_properties( ${LIB_NAME} PROPERTIES PROJECT_LABEL "Plugin ${LIB_NAME}" )

include( PluginInstall REQUIRED )

if (APPLE)
    install(FILES ${CMAKE_CURRENT_LIST_DIR}/ConfigFiles/libIgPlugin-SkyDome.so.xml DESTINATION ${INSTALL_LIBDIR} RENAME libIgPlugin-SkyDome.dylib.xml)
elseif(WIN32)
    install(FILES ${CMAKE_CURRENT_LIST_DIR}/ConfigFiles/libIgPlugin-SkyDome.so.windows.xml DESTINATION ${INSTALL_BINDIR} RENAME IgPlugin-SkyDome.dll.xml)
else()
    install(FILES ${CMAKE_CURRENT_LIST_DIR}/ConfigFiles/libIgPlugin-SkyDome.so.xml DESTINATION ${INSTALL_LIBDIR} RENAME libIgPlugin-SkyDome.so.xml)
endif()
