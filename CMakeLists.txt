cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(dev)

if (WINDOWS)
add_definitions(-D__WINDOWS__)
add_definitions(-D_WIN32_WINNT=0x0501)
add_definitions(-D_USE_SELECT)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else ()
add_definitions(-D__LINUX__)
add_definitions(-D_USE_EPOLL)
	if (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-std=gnu++0x)
	else ()
	add_definitions(-std=stdc++11)
	endif()
endif()

if (DEBUG)
add_definitions(-D_DEBUG)
endif()

set(dev_SOURCES
	Dev.hpp
	base/Base.hpp
	base/Atomic.hpp
	base/Runtime.hpp
	base/Logger.hpp
	base/Mutex.hpp
	base/LockGuard.hpp
	base/Exception.hpp
	base/Exception.cpp
	base/Thread.hpp
	base/Thread.cpp
	base/Mutex.hpp
	base/LockedQueue.hpp
	base/TimeUtil.hpp
	base/TimeUtil.cpp
	base/detail/ConfigFile.hpp
	base/detail/IniConfigFile.hpp
	base/detail/XmlConfigFile.hpp
	
	net/Net.hpp
	net/SocketApi.hpp
	net/Socket.hpp
	net/Socket.cpp
	net/SocketEventHandler.hpp
	net/Reactor.hpp
	net/detail/SelectReactor.hpp
	net/detail/SelectReactor.cpp
	net/EventLoop.hpp
	net/EventLoop.cpp
	net/TcpConnectionStream.hpp
	net/TcpConnectionInputStream.hpp
	net/TcpConnectionOutputStream.hpp
	net/TcpConnectionStream.cpp
	net/TcpConnectionInputStream.cpp
	net/TcpConnectionOutputStream.cpp
	net/TcpConnection.hpp
	net/TcpConnection.cpp
	net/TcpConnectionFactory.hpp
	net/TcpAcceptor.hpp
	net/TcpAcceptor.cpp
	net/TcpWorker.hpp
	net/TcpWorker.cpp
	net/TcpServer.hpp
	net/TcpServer.cpp
)

if (WINDOWS)
set(plat_SRC
)
else()
set(plat_SRC
	net/detail/EPollReactor.hpp
	net/detail/EPollReactor.cpp
)
endif()

if (WINDOWS)
include_directories( 
	"E:/SDKs/boost_1_58_0.msw"
	"E:/git"
)
else ()
include_directories(
	"~/git"
)
endif()

add_library(dev STATIC ${dev_SOURCES} ${plat_SRC})
