add_requires("enet", { system = true, optional = false })

target("client")
	set_kind("binary")
	add_files("client/*.c")
	add_packages("enet")

target("server")
	set_kind("binary")
	add_files("server/*.c")
	add_packages("enet")
