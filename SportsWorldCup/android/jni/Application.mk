APP_STL := gnustl_static
APP_CPPFLAGS := -frtti
APP_CPPFLAGS	+= -fexceptions
APP_CPPFLAGS	+= -std=gnu++0x
APP_CFLAGS	:= -std=c99
APP_TOOLCHAIN_VERSION	:= 4.6.3
APP_USE_CPP0X	 := true
APP_OPTIM := release