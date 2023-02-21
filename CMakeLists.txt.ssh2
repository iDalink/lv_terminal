set(component_include 
	include 
	)

set(component_srcs
	src/agent.c
	src/bcrypt_pbkdf.c
	src/blowfish.c
	src/channel.c
	src/comp.c
	src/crypt.c
	src/global.c
	src/hostkey.c
	src/keepalive.c
	src/kex.c
	src/knownhost.c
	src/libgcrypt.c
	src/mac.c
	src/mbedtls.c
	src/misc.c
	src/openssl.c
	src/os400qc3.c
	src/packet.c
	src/pem.c
	src/publickey.c
	src/scp.c
	src/session.c
	src/sftp.c
	src/transport.c
	src/userauth.c
	src/version.c
	src/wincng.c
	)

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "${component_include}"
                       REQUIRES mbedtls)

target_compile_options(${COMPONENT_TARGET} PRIVATE -Wno-maybe-uninitialized)
