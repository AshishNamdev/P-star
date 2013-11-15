AM_CPPFLAGS = -I$(top_srcdir)/include -include $(top_srcdir)/config.h
AM_CXXFLAGS = -std=c++11 -rdynamic

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libwpl-1.0.pc

lib_LTLIBRARIES = libwpl-1.0.la
libwpl_1_0_la_LDFLAGS = -lmimetic
libwpl_1_0_la_SOURCES = \
	program.cpp parser.cpp block.cpp pragma.cpp \
	module_loader.cpp \
	expression.cpp matcher.cpp identifier.cpp \
	variable.cpp namespace.cpp runable.cpp \
	shunting_yard.cpp block_if.cpp block_while.cpp \
	scene.cpp string.cpp object.cpp types.cpp \
	function.cpp user_function.cpp internal_function.cpp \
	value.cpp operator.cpp text.cpp \
	value_array.cpp value_struct.cpp value_hash.cpp \
	value_unresolved.cpp value_function_ptr.cpp value_list.cpp \
	value_string.cpp value_vstring.cpp \
	value_env.cpp value_get.cpp value_post.cpp value_stdin.cpp \
	array.cpp hash.cpp struct.cpp template.cpp \
	namespace_session.cpp \
	block_state.cpp expression_state.cpp function_state.cpp \
	pragma_state.cpp text_state.cpp \
	output_json.cpp