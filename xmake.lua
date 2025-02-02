add_requires("enet")
add_requires("lua")

target("client")
    set_kind("binary")
    add_files("client/*.c")
    add_files("common/*.c")
    add_packages("enet")
    add_packages("lua")

target("server")
    set_kind("binary")
    add_files("server/*.c")
    add_files("common/*.c")
    add_packages("enet")
    add_packages("lua")
