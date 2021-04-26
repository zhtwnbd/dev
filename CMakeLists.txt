cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(dev)

if (WINDOWS)
add_definitions(-D__WINDOWS__)
add_definitions(-D_WIN32_WINNT=0x0501)
else ()
add_definitions(-D__LINUX__)
endif()
add_definitions(-D_USE_SELECT)

set(dev_SOURCES
	Dev.hpp
	base/Base.hpp
	base/Runtime.hpp
	base/Mutex.hpp
	base/LockGuard.hpp
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
)

if (WINDOWS)
include_directories( 
	"E:/SDKs/boost_1_58_0.msw"
	"E:/git"
)
endif()

add_library(dev STATIC ${dev_SOURCES})