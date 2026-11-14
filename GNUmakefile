# file fltrk/GNUmakefile

## see https://github.com/bstarynk/fltruk
## Author(s):
##      Basile STARYNKEVITCH, 92340 Bourg-la-Reine, France,
##                    <basile@starynkevitch.net>
CC = gcc
CXX = g++ #the Gnu C compiler
CCACHE ?= ccache
OPTIM ?= -Og
CXXFLAGS += -Wall -Wextra -Weffc++ -D_GNU_SOURCE=1 $(OPTIM) -g $(shell fltk-config --cxxflags)
ASTYLE = astyle
SOURCEFILES:= $(wildcard *.cc)
OBJECTFILES=$(patsubst %.cc, obj/%.o, $(SOURCEFILES))
.PHONY: all clean indent
all: fltruk | GNUmakefile

-include generated/$(basename $(first $(MAKECMDGOALS))).mk

clean:
	/bin/rm -vf *~ obj/*.o obj/*.so fltruk _*_.cc *.i *~ *% _*_.c
	/bin/rm -vf *.orig *% generated/*orig generated/*%
indent:
	$(ASTYLE) -v $(wildcard [a-z]*.cc [a-z]*.hh)

_listfiles_.c: | GNUmakefile
	-@cp -f $@ $$HOME/tmp/
	printf "/// generated (in github.com/bstarynk/fltruk) file %s\n" $@ > $@
	printf "const char source_directory_fltruk[] = \"%s\";\n" $(realpath .) >> $@
	printf "const char* source_files_fltruk[] = {\n" >> $@
	for f in $(SOURCEFILES) ; do \
	  printf "  \"%s\",\n" $$f ; \
	done >> $@
	printf "  (const char*)0};\n" >> $@
	printf "/// our make is %s\n" $(MAKE) >> $@
	printf "const char make_fltruk[]=\"%s\";\n" $(shell which $(MAKE)) >> $@

# https://stackoverflow.com/a/3220288/841108
# compile a source file to an object 
obj/%.o: %.cc fltruk.hh | GNUmakefile get-short-git.sh
	@printf "$$< is %s\n" "$<"
	$(CXX) -UFLTRUK_MAKOBJ -DFLTRUK_BASE=\"$(basename $<)\" $(CXXFLAGS) \
             -DFLTRUK_SHORTGIT=\"$(shell ./get-short-git.sh $^)\" \
             -c $< -o $@

obj/%.o: %.c | GNUmakefile get-short-git.sh
	$(CC) -UFLTRUK_COBJ  -DFLTRUK_BASE=\"$(basename $<)\" -g $(OPTIM) \
             -DFLTRUK_SHORTGIT=\"$(shell ./get-short-git.sh $^)\" \
             -c $< -o $@

#https://stackoverflow.com/a/14061796/841108
obj/%.so: generated/%.c generated/%.cflags fltruk.hh | GNUmakefile get-short-git.sh
	@printf "#dotless='%s' dotcir='%s' dotat='%s' cfl='%s'\n" "$<" "$^" "$@" "$(notdir $(basename $<)).cflags"
	@printf "#MAKECMDGOALS=%s\n" $(MAKECMDGOALS)
	$(CCACHE) $(CC) -I. -Igenerated/ $$(grep -v '^#' generated/$(notdir $(basename $<)).cflags) -UFLTRUK_GENCFLPLUGIN  -DFLTRUK_BASE=\"$(basename $<)\" -g $(OPTIM) \
             -DFLTRUK_SHORTGIT=\"$(shell ./get-short-git.sh $^)\" \
             -c $< -fPIC -shared -o $@

obj/%.so: generated/%.c fltruk.hh | GNUmakefile get-short-git.sh
	$(CCACHE) $(CC) -I. -Igenerated/ -UFLTRUK_GENPLUGIN  -DFLTRUK_BASE=\"$(basename $<)\" -g $(OPTIM) \
             -DFLTRUK_SHORTGIT=\"$(shell ./get-short-git.sh $^)\" \
             -c $< -fPIC -shared -o $@


obj/%.so: generated/%.cc fltruk.hh | GNUmakefile get-short-git.sh
	$(CCACHE) $(CXX) -I. -Igenerated/  -UFLTRUK_GENCXXPLUGIN  -DFLTRUK_BASE=\"$(basename $<)\" -g $(OPTIM) \
             -DFLTRUK_SHORTGIT=\"$(shell ./get-short-git.sh $^)\" \
             -c $< -fPIC -shared -o $@

fltruk: $(OBJECTFILES) obj/_listfiles_.o | GNUmakefile
	$(CXX) $(OPTIM) -g $(OBJECTFILES) obj/_listfiles_.o $(shell fltk-config --ldflags) -o $@

#obj/main-fltruk.o: main-fltruk.cc

####
## for GNU emacs...
## Local-Variables: ;;
## compile-command: "make -j3" ;;
## End: ;;
####

# end of file fltrk/GNUmakefile
