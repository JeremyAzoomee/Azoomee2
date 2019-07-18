APP_STL := c++_static
APP_LDFLAGS := -latomic
APP_SHORT_COMMANDS := true

ifeq ($(NDK_DEBUG),1)
  APP_OPTIM := debug
else
  APP_OPTIM := release
endif
