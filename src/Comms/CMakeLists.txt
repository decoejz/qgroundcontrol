add_subdirectory(AirLink)
add_subdirectory(MockLink)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/sign_scheme sign_scheme.build)

string(ASCII 27 Esc)
set(ColourReset "${Esc}[m")
set(Red         "${Esc}[31m")
set(Green       "${Esc}[32m")
set(Blue        "${Esc}[34m")

# Check if SIGN_TYPE environment variable is defined
if(DEFINED ENV{SIGN_TYPE})
	if("$ENV{SIGN_TYPE}" STREQUAL "RSA")
		message(STATUS "SIGN_TYPE is ${Blue}RSA${ColourReset}.")
		set(SCHEME_LIB rsa)
		add_definitions(-DRSA_SCHEME)
    elseif("$ENV{SIGN_TYPE}" STREQUAL "ECSDA")
        message(STATUS "SIGN_TYPE is ${Blue}ECDSA${ColourReset}.")
        set(SCHEME_LIB ecdsa)
        add_definitions(-DECDSA_SCHEME)
	else()
		message(STATUS "SIGN_TYPE ${Blue}no signature${ColourReset} configured.")
		set(SCHEME_LIB no_sign)
	endif()
else()
	message(STATUS "SIGN_TYPE ${Blue}no signature${ColourReset} configured.")
	set(SCHEME_LIB no_sign)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Network Qml Test Widgets)

qt_add_library(Comms STATIC
    LinkConfiguration.cc
    LinkConfiguration.h
    LinkInterface.cc
    LinkInterface.h
    LinkManager.cc
    LinkManager.h
    LogReplayLink.cc
    LogReplayLink.h
    MAVLinkProtocol.cc
    MAVLinkProtocol.h
    TCPLink.cc
    TCPLink.h
    UDPLink.cc
    UDPLink.h
)

target_link_libraries(Comms
    PRIVATE
        Qt6::Qml
        Qt6::Test
        MockLink
        PositionManager
        QGC
        Settings
        Vehicle
        ${SCHEME_LIB}
    PUBLIC
        Qt6::Core
        Qt6::Network
        AirLink
        MAVLink
        QmlControls
        Utilities
)

target_include_directories(Comms
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/libs/sign_scheme/${SCHEME_LIB}
)

############# Serial

if(QGC_NO_SERIAL_LINK)
    target_compile_definitions(Comms PUBLIC NO_SERIAL_LINK)
else()
    target_sources(Comms
        PRIVATE
            QGCSerialPortInfo.cc
            QGCSerialPortInfo.h
            SerialLink.cc
            SerialLink.h
            UdpIODevice.cc
            UdpIODevice.h
    )
    target_link_libraries(Comms
        PRIVATE
            GPS
            PositionManager
    )
    if(ANDROID)
        add_subdirectory(${CMAKE_SOURCE_DIR}/android/libs/qtandroidserialport qtandroidserialport.build)
        target_sources(qtandroidserialport
            PRIVATE
                ${CMAKE_SOURCE_DIR}/android/src/AndroidSerial.cc
                ${CMAKE_SOURCE_DIR}/android/src/AndroidSerial.h
        )
        target_include_directories(qtandroidserialport PUBLIC ${CMAKE_SOURCE_DIR}/android/src)
        target_link_libraries(Comms PUBLIC qtandroidserialport)
    else()
        find_package(Qt6 REQUIRED COMPONENTS SerialPort)
        target_link_libraries(Comms PUBLIC Qt6::SerialPort)
        target_precompile_headers(Comms
            PRIVATE
                <QtSerialPort/QSerialPort>
        )
    endif()
endif()

############# Bluetooth

if(QGC_ENABLE_BLUETOOTH)
    find_package(Qt6 REQUIRED COMPONENTS Bluetooth)
    target_link_libraries(Comms PUBLIC Qt6::Bluetooth)
    target_sources(Comms
        PRIVATE
            BluetoothLink.cc
            BluetoothLink.h
    )
    target_compile_definitions(Comms PUBLIC QGC_ENABLE_BLUETOOTH)
    target_precompile_headers(Comms
        PRIVATE
            <QtBluetooth/QBluetoothSocket>
    )
endif()

############# ZEROConf

if(QGC_ZEROCONF_ENABLED)
    set(BUILD_DOC OFF CACHE INTERNAL "")
    set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
    set(BUILD_TESTS OFF CACHE INTERNAL "")

    include(FetchContent)
    FetchContent_Declare(qmdnsengine
        GIT_REPOSITORY https://github.com/nitroshare/qmdnsengine.git
        GIT_TAG 4e54bc86c8ed2d4fa2e7449d4ba6a6a2742d9eb1
    )
    FetchContent_MakeAvailable(qmdnsengine)

    target_link_libraries(Comms PRIVATE qmdnsengine)
    target_compile_definitions(Comms PUBLIC QGC_ZEROCONF_ENABLED)
endif()
