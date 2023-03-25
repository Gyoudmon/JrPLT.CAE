import sdl2

import ctypes as ffi

###############################################################################
def get_current_mouse_location():
    mx = ffi.c_int()
    my = ffi.c_int()

    sdl2.SDL_GetMouseState(ffi.byref(mx), ffi.byref(my))

    return (float(mx.value), float(my.value))
