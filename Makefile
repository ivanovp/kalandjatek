# vim:set noet:
#
# Makefile
# Copyright (C) Peter Ivanov, 2005
#
# First:
# make dep
# 
# Normal code:
# make
#
# Debugging code:
# make DEBUG=1
#
# English language:
# make LANG=0
#
# Hungarian language (default):
# make LANG=1
#
# Creating documentation:
# make doc

# DEBUG:
# 0: disabled, normal code
# 1: enabled, compiling code with debugging features
DEBUG = 1
# LANG:
# 0: english
# 1: hungarian
LANG = 1
# GTKMM:
# 0: disabled
# 1: enabled
GTKMM = 1

EXTRALIBS =
EXTRAFLAGS = -DLANG=$(LANG)
ifeq ($(GTKMM), 1)
EXTRALIBS += `pkg-config gtkmm-2.4 --libs`
EXTRAFLAGS += -DGTKMM `pkg-config gtkmm-2.4 --cflags`
endif
#CC = gcc-3.4
#CXX = g++-3.4
# `pkg-config lua50 --libs` 
LDLIBS = `pcre-config --libs` `log4cpp-config --libs` $(EXTRALIBS)
CFLAGS = `pcre-config --cflags` `log4cpp-config --cflags` $(EXTRAFLAGS)
ifeq ($(DEBUG), 1)
CFLAGS += -Wall -g -D__DEBUG__
else
CFLAGS += -Wall -ffast-math -fomit-frame-pointer -O3 -s $(EXTRA)
endif
CXXFLAGS = $(CFLAGS)

PROG = kj
OBJS = $(PROG).o app.o thing.o item.o creature.o map.o cfg.o regex.o split.o trans.o lang.o inv.o
#ifeq ($(GTKMM), 1)
#OBJS += textbuf.o
#endif

.PHONY: all
all:    $(PROG)

.PHONY: doc
doc:	doxygen.cfg *.cc *.h
	doxygen $<

#.PHONY: dep
#dep:	.depend

#.depend: *.cc *.h
#	$(CXX) -E -MM *.cc *.h > .depend

.PHONY: dep
dep:	.depend

.depend:
	makedepend -- $(CXXFLAGS) -- *.cc
#	touch $@
#	makedepend -f$@ -- $(CXXFLAGS) -- *.cc

$(PROG): $(OBJS)

.PHONY: sync
sync: synckoli synctriton

.PHONY: synckoli
synckoli: clean
	rsync -avuz --delete --progress --exclude "*.swp" ~/progs/test2 koli.kando.hu:progs/
	rsync -avuz --delete --progress --exclude "*.swp"  koli.kando.hu:progs/test2 ~/progs/

.PHONY: synctriton
synctriton: clean
	rsync -avuz --delete --progress --exclude "*.swp" ~/progs/test2 triton.homelinux.org:progs/
	rsync -avuz --delete --progress --exclude "*.swp"  triton.homelinux.org:progs/test2 ~/progs/

.PHONY: clean cleanobj cleanall
clean:  cleanall

cleanobj:
	@rm -fv $(OBJS) *~

cleanall:
	@rm -rfv $(PROG) $(OBJS) doc/ *~ *.log .depend

%o:     %cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%::
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDLIBS)

%.tar.gz:
	tar -cvzf $@ $^

%.zip:
	zip -r $@ $^

# DO NOT DELETE

app.o: app.h /usr/include/log4cpp/Category.hh
app.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
app.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
app.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
app.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
app.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
app.o: /usr/include/log4cpp/Log4cppCleanup.hh
app.o: /usr/include/log4cpp/CategoryStream.hh
app.o: /usr/include/log4cpp/BasicLayout.hh
app.o: /usr/include/log4cpp/PatternLayout.hh
app.o: /usr/include/log4cpp/FileAppender.hh
app.o: /usr/include/log4cpp/LayoutAppender.hh
app.o: /usr/include/log4cpp/AppenderSkeleton.hh
app.o: /usr/include/log4cpp/OstreamAppender.hh thing.h regex.h
app.o: /usr/include/pcre.h /usr/include/stdlib.h /usr/include/features.h
app.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
app.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
app.o: /usr/include/sys/types.h /usr/include/bits/types.h
app.o: /usr/include/bits/typesizes.h /usr/include/time.h
app.o: /usr/include/endian.h /usr/include/bits/endian.h
app.o: /usr/include/sys/select.h /usr/include/bits/select.h
app.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
app.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
app.o: /usr/include/alloca.h debug.h split.h item.h creature.h lang.h trans.h
app.o: functor.h map.h loader.h cfg.h keyword.h colors.h ansi.h
cfg.o: cfg.h regex.h /usr/include/pcre.h /usr/include/stdlib.h
cfg.o: /usr/include/features.h /usr/include/sys/cdefs.h
cfg.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
cfg.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
cfg.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
cfg.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
cfg.o: /usr/include/sys/select.h /usr/include/bits/select.h
cfg.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
cfg.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
cfg.o: /usr/include/alloca.h debug.h /usr/include/log4cpp/Category.hh
cfg.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
cfg.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
cfg.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
cfg.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
cfg.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
cfg.o: /usr/include/log4cpp/Log4cppCleanup.hh
cfg.o: /usr/include/log4cpp/CategoryStream.hh
creature.o: debug.h /usr/include/log4cpp/Category.hh
creature.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
creature.o: /usr/include/log4cpp/Export.hh
creature.o: /usr/include/log4cpp/OstringStream.hh
creature.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
creature.o: /usr/include/log4cpp/Layout.hh
creature.o: /usr/include/log4cpp/LoggingEvent.hh
creature.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
creature.o: /usr/include/log4cpp/Log4cppCleanup.hh
creature.o: /usr/include/log4cpp/CategoryStream.hh item.h thing.h regex.h
creature.o: /usr/include/pcre.h /usr/include/stdlib.h /usr/include/features.h
creature.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
creature.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
creature.o: /usr/include/sys/types.h /usr/include/bits/types.h
creature.o: /usr/include/bits/typesizes.h /usr/include/time.h
creature.o: /usr/include/endian.h /usr/include/bits/endian.h
creature.o: /usr/include/sys/select.h /usr/include/bits/select.h
creature.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
creature.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
creature.o: /usr/include/alloca.h split.h creature.h lang.h trans.h functor.h
creature.o: map.h keyword.h inv.h colors.h ansi.h nullstream.h app.h
creature.o: /usr/include/log4cpp/BasicLayout.hh
creature.o: /usr/include/log4cpp/PatternLayout.hh
creature.o: /usr/include/log4cpp/FileAppender.hh
creature.o: /usr/include/log4cpp/LayoutAppender.hh
creature.o: /usr/include/log4cpp/AppenderSkeleton.hh
creature.o: /usr/include/log4cpp/OstreamAppender.hh loader.h cfg.h
inv.o: inv.h lang.h trans.h /usr/include/pcre.h /usr/include/stdlib.h
inv.o: /usr/include/features.h /usr/include/sys/cdefs.h
inv.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
inv.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
inv.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
inv.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
inv.o: /usr/include/sys/select.h /usr/include/bits/select.h
inv.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
inv.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
inv.o: /usr/include/alloca.h regex.h debug.h /usr/include/log4cpp/Category.hh
inv.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
inv.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
inv.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
inv.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
inv.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
inv.o: /usr/include/log4cpp/Log4cppCleanup.hh
inv.o: /usr/include/log4cpp/CategoryStream.hh
item.o: item.h thing.h /usr/include/log4cpp/Category.hh
item.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
item.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
item.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
item.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
item.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
item.o: /usr/include/log4cpp/Log4cppCleanup.hh
item.o: /usr/include/log4cpp/CategoryStream.hh regex.h /usr/include/pcre.h
item.o: /usr/include/stdlib.h /usr/include/features.h
item.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
item.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
item.o: /usr/include/sys/types.h /usr/include/bits/types.h
item.o: /usr/include/bits/typesizes.h /usr/include/time.h
item.o: /usr/include/endian.h /usr/include/bits/endian.h
item.o: /usr/include/sys/select.h /usr/include/bits/select.h
item.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
item.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
item.o: /usr/include/alloca.h debug.h split.h lang.h trans.h keyword.h
kj.o: app.h /usr/include/log4cpp/Category.hh
kj.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
kj.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
kj.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
kj.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
kj.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
kj.o: /usr/include/log4cpp/Log4cppCleanup.hh
kj.o: /usr/include/log4cpp/CategoryStream.hh
kj.o: /usr/include/log4cpp/BasicLayout.hh
kj.o: /usr/include/log4cpp/PatternLayout.hh
kj.o: /usr/include/log4cpp/FileAppender.hh
kj.o: /usr/include/log4cpp/LayoutAppender.hh
kj.o: /usr/include/log4cpp/AppenderSkeleton.hh
kj.o: /usr/include/log4cpp/OstreamAppender.hh thing.h regex.h
kj.o: /usr/include/pcre.h /usr/include/stdlib.h /usr/include/features.h
kj.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
kj.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
kj.o: /usr/include/sys/types.h /usr/include/bits/types.h
kj.o: /usr/include/bits/typesizes.h /usr/include/time.h /usr/include/endian.h
kj.o: /usr/include/bits/endian.h /usr/include/sys/select.h
kj.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
kj.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
kj.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h debug.h split.h
kj.o: item.h creature.h lang.h trans.h functor.h map.h loader.h cfg.h
kj.o: keyword.h
lang.o: lang.h trans.h /usr/include/pcre.h /usr/include/stdlib.h
lang.o: /usr/include/features.h /usr/include/sys/cdefs.h
lang.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
lang.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
lang.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
lang.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
lang.o: /usr/include/sys/select.h /usr/include/bits/select.h
lang.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
lang.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
lang.o: /usr/include/alloca.h regex.h debug.h
lang.o: /usr/include/log4cpp/Category.hh /usr/include/log4cpp/Portability.hh
lang.o: /usr/include/log4cpp/config.h /usr/include/log4cpp/Export.hh
lang.o: /usr/include/log4cpp/OstringStream.hh
lang.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
lang.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
lang.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
lang.o: /usr/include/log4cpp/Log4cppCleanup.hh
lang.o: /usr/include/log4cpp/CategoryStream.hh
map.o: map.h thing.h /usr/include/log4cpp/Category.hh
map.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
map.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
map.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
map.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
map.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
map.o: /usr/include/log4cpp/Log4cppCleanup.hh
map.o: /usr/include/log4cpp/CategoryStream.hh regex.h /usr/include/pcre.h
map.o: /usr/include/stdlib.h /usr/include/features.h /usr/include/sys/cdefs.h
map.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
map.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
map.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
map.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
map.o: /usr/include/sys/select.h /usr/include/bits/select.h
map.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
map.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
map.o: /usr/include/alloca.h debug.h split.h
regex.o: regex.h /usr/include/pcre.h /usr/include/stdlib.h
regex.o: /usr/include/features.h /usr/include/sys/cdefs.h
regex.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
regex.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
regex.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
regex.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
regex.o: /usr/include/sys/select.h /usr/include/bits/select.h
regex.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
regex.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
regex.o: /usr/include/alloca.h debug.h /usr/include/log4cpp/Category.hh
regex.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
regex.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
regex.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
regex.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
regex.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
regex.o: /usr/include/log4cpp/Log4cppCleanup.hh
regex.o: /usr/include/log4cpp/CategoryStream.hh
split.o: split.h regex.h /usr/include/pcre.h /usr/include/stdlib.h
split.o: /usr/include/features.h /usr/include/sys/cdefs.h
split.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
split.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
split.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
split.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
split.o: /usr/include/sys/select.h /usr/include/bits/select.h
split.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
split.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
split.o: /usr/include/alloca.h debug.h /usr/include/log4cpp/Category.hh
split.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
split.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
split.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
split.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
split.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
split.o: /usr/include/log4cpp/Log4cppCleanup.hh
split.o: /usr/include/log4cpp/CategoryStream.hh
test1.o: thing.h /usr/include/log4cpp/Category.hh
test1.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
test1.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
test1.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
test1.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
test1.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
test1.o: /usr/include/log4cpp/Log4cppCleanup.hh
test1.o: /usr/include/log4cpp/CategoryStream.hh regex.h /usr/include/pcre.h
test1.o: /usr/include/stdlib.h /usr/include/features.h
test1.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
test1.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
test1.o: /usr/include/sys/types.h /usr/include/bits/types.h
test1.o: /usr/include/bits/typesizes.h /usr/include/time.h
test1.o: /usr/include/endian.h /usr/include/bits/endian.h
test1.o: /usr/include/sys/select.h /usr/include/bits/select.h
test1.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
test1.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
test1.o: /usr/include/alloca.h debug.h split.h cfg.h
thing.o: thing.h /usr/include/log4cpp/Category.hh
thing.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
thing.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/OstringStream.hh
thing.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
thing.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
thing.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
thing.o: /usr/include/log4cpp/Log4cppCleanup.hh
thing.o: /usr/include/log4cpp/CategoryStream.hh regex.h /usr/include/pcre.h
thing.o: /usr/include/stdlib.h /usr/include/features.h
thing.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
thing.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
thing.o: /usr/include/sys/types.h /usr/include/bits/types.h
thing.o: /usr/include/bits/typesizes.h /usr/include/time.h
thing.o: /usr/include/endian.h /usr/include/bits/endian.h
thing.o: /usr/include/sys/select.h /usr/include/bits/select.h
thing.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
thing.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
thing.o: /usr/include/alloca.h debug.h split.h lang.h trans.h keyword.h
trans.o: trans.h /usr/include/pcre.h /usr/include/stdlib.h
trans.o: /usr/include/features.h /usr/include/sys/cdefs.h
trans.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
trans.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
trans.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
trans.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
trans.o: /usr/include/sys/select.h /usr/include/bits/select.h
trans.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
trans.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
trans.o: /usr/include/alloca.h regex.h debug.h
trans.o: /usr/include/log4cpp/Category.hh /usr/include/log4cpp/Portability.hh
trans.o: /usr/include/log4cpp/config.h /usr/include/log4cpp/Export.hh
trans.o: /usr/include/log4cpp/OstringStream.hh
trans.o: /usr/include/log4cpp/Appender.hh /usr/include/log4cpp/Priority.hh
trans.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
trans.o: /usr/include/log4cpp/TimeStamp.hh /usr/include/log4cpp/Filter.hh
trans.o: /usr/include/log4cpp/Log4cppCleanup.hh
trans.o: /usr/include/log4cpp/CategoryStream.hh
