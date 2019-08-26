#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_SRCDIRS := . pwm/ clock/ wifi/ config/ storage/
COMPONENT_ADD_INCLUDEDIRS := .

# timezones
COMPONENT_EMBED_TXTFILES += clock/zones.json
