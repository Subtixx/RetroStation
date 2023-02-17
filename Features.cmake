option(FEATURE_TESTS "Enable the tests" OFF)
if (FEATURE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif ()

option(FEATURE_DOCS "Enable the docs" OFF)
if (FEATURE_DOCS)
    list(APPEND VCPKG_MANIFEST_FEATURES "docs")

    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(DOXYGEN_PROJECT_NAME "RetroStation")
        set(DOXYGEN_PROJECT_BRIEF "RetroStation is a graphical front-end for emulators, game engines and media players.")
        SET(DOXYGEN_USE_MDFILE_AS_MAINPAGE "../README.md")
        set(DOXYGEN_OUTPUT_DIRECTORY doxygen)
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_TREEVIEW YES)
        set(DOXYGEN_DISABLE_INDEX NO)
        set(DOXYGEN_FULL_SIDEBAR YES)
        set(DOXYGEN_SOURCE_BROWSER YES)
        set(DOXYGEN_HTML_EXTRA_STYLESHEET
                ${CMAKE_SOURCE_DIR}/external/doxygen-awesome-css/doxygen-awesome.css
                ${CMAKE_SOURCE_DIR}/external/doxygen/custom.css
                )
        set(DOXYGEN_HTML_HEADER ${CMAKE_SOURCE_DIR}/external/doxygen/header.html)
        set(DOXYGEN_HTML_EXTRA_FILES
                ${CMAKE_SOURCE_DIR}/external/doxygen-awesome-css/doxygen-awesome-darkmode-toggle.js
                ${CMAKE_SOURCE_DIR}/external/doxygen-awesome-css/doxygen-awesome-fragment-copy-button.js
                ${CMAKE_SOURCE_DIR}/external/doxygen-awesome-css/doxygen-awesome-interactive-toc.js
                ${CMAKE_SOURCE_DIR}/external/doxygen-awesome-css/doxygen-awesome-paragraph-link.js
                ${CMAKE_SOURCE_DIR}/external/doxygen-awesome-css/doxygen-awesome-tabs.js
                )
        #set(DOXYGEN_HTML_COLORSTYLE LIGHT)
        #        set( DOXYGEN_COLLABORATION_GRAPH YES )
        #        set( DOXYGEN_CLASS_DIAGRAMS YES )
        #        set( DOXYGEN_HIDE_UNDOC_RELATIONS NO )
        #        set( DOXYGEN_HAVE_DOT YES )
        #        set( DOXYGEN_CLASS_GRAPH YES )
        #        set( DOXYGEN_CALL_GRAPH YES )
        #        set( DOXYGEN_CALLER_GRAPH YES )
        #        set( DOXYGEN_COLLABORATION_GRAPH YES )
        #        set( DOXYGEN_BUILTIN_STL_SUPPORT YES )
        #        set( DOXYGEN_EXTRACT_PRIVATE YES )
        #        set( DOXYGEN_EXTRACT_PACKAGE YES )
        #        set( DOXYGEN_EXTRACT_STATIC YES )
        #        set( DOXYGEN_EXTRACT_LOCALMETHODS YES )
        #        set( DOXYGEN_UML_LOOK YES )
        #        set( DOXYGEN_UML_LIMIT_NUM_FIELDS 50 )
        #        set( DOXYGEN_TEMPLATE_RELATIONS YES )
        #        set( DOXYGEN_DOT_GRAPH_MAX_NODES 100 )
        #        set( DOXYGEN_MAX_DOT_GRAPH_DEPTH 0 )
        #        set( DOXYGEN_DOT_TRANSPARENT YES )
        set(DOXYGEN_EXCLUDE_PATTERNS
                "external/*"
                "data/*"
                "datainit/*"
                "locale/*"
                )

        doxygen_add_docs(doxygen
                es-core/src
                es-core/include
                es-app/src
                es-app/include
                )

        MESSAGE(WARNING "ATTENTION: The doxygen documentation is very very big! (>3GB)")
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif ()