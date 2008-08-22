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
DEBUG = 0
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
app.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
app.o: /usr/include/sys/types.h /usr/include/features.h
app.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
app.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
app.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
app.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
app.o: /usr/include/sys/select.h /usr/include/bits/select.h
app.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
app.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
app.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/fcntl.h
app.o: /usr/include/bits/fcntl.h /usr/include/log4cpp/Priority.hh
app.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
app.o: /usr/include/log4cpp/TimeStamp.hh
app.o: /usr/include/log4cpp/threading/Threading.hh
app.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
app.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
app.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
app.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h
app.o: /usr/include/signal.h /usr/include/bits/setjmp.h /usr/include/assert.h
app.o: /usr/include/log4cpp/CategoryStream.hh
app.o: /usr/include/log4cpp/Manipulator.hh /usr/include/log4cpp/convenience.h
app.o: /usr/include/log4cpp/BasicLayout.hh
app.o: /usr/include/log4cpp/PatternLayout.hh
app.o: /usr/include/log4cpp/Configurator.hh
app.o: /usr/include/log4cpp/FileAppender.hh
app.o: /usr/include/log4cpp/LayoutAppender.hh
app.o: /usr/include/log4cpp/AppenderSkeleton.hh
app.o: /usr/include/log4cpp/Filter.hh /usr/include/log4cpp/OstreamAppender.hh
app.o: thing.h regex.h /usr/include/pcre.h /usr/include/stdlib.h
app.o: /usr/include/alloca.h debug.h split.h item.h creature.h nullstream.h
app.o: lang.h trans.h functor.h map.h loader.h cfg.h keyword.h colors.h
app.o: ansi.h
cfg.o: cfg.h regex.h /usr/include/pcre.h /usr/include/stdlib.h
cfg.o: /usr/include/features.h /usr/include/sys/cdefs.h
cfg.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
cfg.o: /usr/include/gnu/stubs-64.h /usr/include/sys/types.h
cfg.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
cfg.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
cfg.o: /usr/include/sys/select.h /usr/include/bits/select.h
cfg.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
cfg.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
cfg.o: /usr/include/alloca.h debug.h /usr/include/log4cpp/Category.hh
cfg.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
cfg.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
cfg.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/fcntl.h
cfg.o: /usr/include/bits/fcntl.h /usr/include/log4cpp/Priority.hh
cfg.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
cfg.o: /usr/include/log4cpp/TimeStamp.hh
cfg.o: /usr/include/log4cpp/threading/Threading.hh
cfg.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
cfg.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
cfg.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
cfg.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h
cfg.o: /usr/include/signal.h /usr/include/bits/setjmp.h /usr/include/assert.h
cfg.o: /usr/include/log4cpp/CategoryStream.hh
cfg.o: /usr/include/log4cpp/Manipulator.hh /usr/include/log4cpp/convenience.h
creature.o: /usr/include/assert.h /usr/include/features.h
creature.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
creature.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h debug.h
creature.o: /usr/include/log4cpp/Category.hh
creature.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
creature.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
creature.o: /usr/include/sys/types.h /usr/include/bits/types.h
creature.o: /usr/include/bits/typesizes.h /usr/include/time.h
creature.o: /usr/include/endian.h /usr/include/bits/endian.h
creature.o: /usr/include/sys/select.h /usr/include/bits/select.h
creature.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
creature.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
creature.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
creature.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
creature.o: /usr/include/log4cpp/Priority.hh /usr/include/log4cpp/Layout.hh
creature.o: /usr/include/log4cpp/LoggingEvent.hh
creature.o: /usr/include/log4cpp/TimeStamp.hh
creature.o: /usr/include/log4cpp/threading/Threading.hh
creature.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
creature.o: /usr/include/libio.h /usr/include/_G_config.h
creature.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
creature.o: /usr/include/bits/sys_errlist.h /usr/include/pthread.h
creature.o: /usr/include/sched.h /usr/include/bits/sched.h
creature.o: /usr/include/signal.h /usr/include/bits/setjmp.h
creature.o: /usr/include/log4cpp/CategoryStream.hh
creature.o: /usr/include/log4cpp/Manipulator.hh
creature.o: /usr/include/log4cpp/convenience.h item.h thing.h regex.h
creature.o: /usr/include/pcre.h /usr/include/stdlib.h /usr/include/alloca.h
creature.o: split.h creature.h nullstream.h lang.h trans.h functor.h map.h
creature.o: keyword.h inv.h colors.h ansi.h app.h
creature.o: /usr/include/log4cpp/BasicLayout.hh
creature.o: /usr/include/log4cpp/PatternLayout.hh
creature.o: /usr/include/log4cpp/Configurator.hh
creature.o: /usr/include/log4cpp/FileAppender.hh
creature.o: /usr/include/log4cpp/LayoutAppender.hh
creature.o: /usr/include/log4cpp/AppenderSkeleton.hh
creature.o: /usr/include/log4cpp/Filter.hh
creature.o: /usr/include/log4cpp/OstreamAppender.hh loader.h cfg.h
inv.o: inv.h lang.h trans.h /usr/include/pcre.h /usr/include/stdlib.h
inv.o: /usr/include/features.h /usr/include/sys/cdefs.h
inv.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
inv.o: /usr/include/gnu/stubs-64.h /usr/include/sys/types.h
inv.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
inv.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
inv.o: /usr/include/sys/select.h /usr/include/bits/select.h
inv.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
inv.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
inv.o: /usr/include/alloca.h regex.h debug.h /usr/include/log4cpp/Category.hh
inv.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
inv.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
inv.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/fcntl.h
inv.o: /usr/include/bits/fcntl.h /usr/include/log4cpp/Priority.hh
inv.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
inv.o: /usr/include/log4cpp/TimeStamp.hh
inv.o: /usr/include/log4cpp/threading/Threading.hh
inv.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
inv.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
inv.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
inv.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h
inv.o: /usr/include/signal.h /usr/include/bits/setjmp.h /usr/include/assert.h
inv.o: /usr/include/log4cpp/CategoryStream.hh
inv.o: /usr/include/log4cpp/Manipulator.hh /usr/include/log4cpp/convenience.h
item.o: item.h thing.h /usr/include/log4cpp/Category.hh
item.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
item.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
item.o: /usr/include/sys/types.h /usr/include/features.h
item.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
item.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
item.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
item.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
item.o: /usr/include/sys/select.h /usr/include/bits/select.h
item.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
item.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
item.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/fcntl.h
item.o: /usr/include/bits/fcntl.h /usr/include/log4cpp/Priority.hh
item.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
item.o: /usr/include/log4cpp/TimeStamp.hh
item.o: /usr/include/log4cpp/threading/Threading.hh
item.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
item.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
item.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
item.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h
item.o: /usr/include/signal.h /usr/include/bits/setjmp.h
item.o: /usr/include/assert.h /usr/include/log4cpp/CategoryStream.hh
item.o: /usr/include/log4cpp/Manipulator.hh
item.o: /usr/include/log4cpp/convenience.h regex.h /usr/include/pcre.h
item.o: /usr/include/stdlib.h /usr/include/alloca.h debug.h split.h lang.h
item.o: trans.h keyword.h
kj.o: app.h /usr/include/log4cpp/Category.hh
kj.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
kj.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
kj.o: /usr/include/sys/types.h /usr/include/features.h
kj.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
kj.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
kj.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
kj.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
kj.o: /usr/include/sys/select.h /usr/include/bits/select.h
kj.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
kj.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
kj.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/fcntl.h
kj.o: /usr/include/bits/fcntl.h /usr/include/log4cpp/Priority.hh
kj.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
kj.o: /usr/include/log4cpp/TimeStamp.hh
kj.o: /usr/include/log4cpp/threading/Threading.hh
kj.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
kj.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
kj.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
kj.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h
kj.o: /usr/include/signal.h /usr/include/bits/setjmp.h /usr/include/assert.h
kj.o: /usr/include/log4cpp/CategoryStream.hh
kj.o: /usr/include/log4cpp/Manipulator.hh /usr/include/log4cpp/convenience.h
kj.o: /usr/include/log4cpp/BasicLayout.hh
kj.o: /usr/include/log4cpp/PatternLayout.hh
kj.o: /usr/include/log4cpp/Configurator.hh
kj.o: /usr/include/log4cpp/FileAppender.hh
kj.o: /usr/include/log4cpp/LayoutAppender.hh
kj.o: /usr/include/log4cpp/AppenderSkeleton.hh /usr/include/log4cpp/Filter.hh
kj.o: /usr/include/log4cpp/OstreamAppender.hh thing.h regex.h
kj.o: /usr/include/pcre.h /usr/include/stdlib.h /usr/include/alloca.h debug.h
kj.o: split.h item.h creature.h nullstream.h lang.h trans.h functor.h map.h
kj.o: loader.h cfg.h keyword.h
lang.o: lang.h trans.h /usr/include/pcre.h /usr/include/stdlib.h
lang.o: /usr/include/features.h /usr/include/sys/cdefs.h
lang.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
lang.o: /usr/include/gnu/stubs-64.h /usr/include/sys/types.h
lang.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
lang.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
lang.o: /usr/include/sys/select.h /usr/include/bits/select.h
lang.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
lang.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
lang.o: /usr/include/alloca.h regex.h debug.h
lang.o: /usr/include/log4cpp/Category.hh /usr/include/log4cpp/Portability.hh
lang.o: /usr/include/log4cpp/config.h /usr/include/log4cpp/Export.hh
lang.o: /usr/include/log4cpp/Appender.hh /usr/include/sys/stat.h
lang.o: /usr/include/bits/stat.h /usr/include/fcntl.h
lang.o: /usr/include/bits/fcntl.h /usr/include/log4cpp/Priority.hh
lang.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
lang.o: /usr/include/log4cpp/TimeStamp.hh
lang.o: /usr/include/log4cpp/threading/Threading.hh
lang.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
lang.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
lang.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
lang.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h
lang.o: /usr/include/signal.h /usr/include/bits/setjmp.h
lang.o: /usr/include/assert.h /usr/include/log4cpp/CategoryStream.hh
lang.o: /usr/include/log4cpp/Manipulator.hh
lang.o: /usr/include/log4cpp/convenience.h
map.o: map.h thing.h /usr/include/log4cpp/Category.hh
map.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
map.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
map.o: /usr/include/sys/types.h /usr/include/features.h
map.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
map.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
map.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
map.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
map.o: /usr/include/sys/select.h /usr/include/bits/select.h
map.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
map.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
map.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/fcntl.h
map.o: /usr/include/bits/fcntl.h /usr/include/log4cpp/Priority.hh
map.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
map.o: /usr/include/log4cpp/TimeStamp.hh
map.o: /usr/include/log4cpp/threading/Threading.hh
map.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
map.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
map.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
map.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h
map.o: /usr/include/signal.h /usr/include/bits/setjmp.h /usr/include/assert.h
map.o: /usr/include/log4cpp/CategoryStream.hh
map.o: /usr/include/log4cpp/Manipulator.hh /usr/include/log4cpp/convenience.h
map.o: regex.h /usr/include/pcre.h /usr/include/stdlib.h
map.o: /usr/include/alloca.h debug.h split.h
regex.o: regex.h /usr/include/pcre.h /usr/include/stdlib.h
regex.o: /usr/include/features.h /usr/include/sys/cdefs.h
regex.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
regex.o: /usr/include/gnu/stubs-64.h /usr/include/sys/types.h
regex.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
regex.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
regex.o: /usr/include/sys/select.h /usr/include/bits/select.h
regex.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
regex.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
regex.o: /usr/include/alloca.h debug.h /usr/include/log4cpp/Category.hh
regex.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
regex.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
regex.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
regex.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
regex.o: /usr/include/log4cpp/Priority.hh /usr/include/log4cpp/Layout.hh
regex.o: /usr/include/log4cpp/LoggingEvent.hh
regex.o: /usr/include/log4cpp/TimeStamp.hh
regex.o: /usr/include/log4cpp/threading/Threading.hh
regex.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
regex.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
regex.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
regex.o: /usr/include/pthread.h /usr/include/sched.h
regex.o: /usr/include/bits/sched.h /usr/include/signal.h
regex.o: /usr/include/bits/setjmp.h /usr/include/assert.h
regex.o: /usr/include/log4cpp/CategoryStream.hh
regex.o: /usr/include/log4cpp/Manipulator.hh
regex.o: /usr/include/log4cpp/convenience.h
split.o: split.h regex.h /usr/include/pcre.h /usr/include/stdlib.h
split.o: /usr/include/features.h /usr/include/sys/cdefs.h
split.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
split.o: /usr/include/gnu/stubs-64.h /usr/include/sys/types.h
split.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
split.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
split.o: /usr/include/sys/select.h /usr/include/bits/select.h
split.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
split.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
split.o: /usr/include/alloca.h debug.h /usr/include/log4cpp/Category.hh
split.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
split.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
split.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
split.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
split.o: /usr/include/log4cpp/Priority.hh /usr/include/log4cpp/Layout.hh
split.o: /usr/include/log4cpp/LoggingEvent.hh
split.o: /usr/include/log4cpp/TimeStamp.hh
split.o: /usr/include/log4cpp/threading/Threading.hh
split.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
split.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
split.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
split.o: /usr/include/pthread.h /usr/include/sched.h
split.o: /usr/include/bits/sched.h /usr/include/signal.h
split.o: /usr/include/bits/setjmp.h /usr/include/assert.h
split.o: /usr/include/log4cpp/CategoryStream.hh
split.o: /usr/include/log4cpp/Manipulator.hh
split.o: /usr/include/log4cpp/convenience.h
thing.o: thing.h /usr/include/log4cpp/Category.hh
thing.o: /usr/include/log4cpp/Portability.hh /usr/include/log4cpp/config.h
thing.o: /usr/include/log4cpp/Export.hh /usr/include/log4cpp/Appender.hh
thing.o: /usr/include/sys/types.h /usr/include/features.h
thing.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
thing.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
thing.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
thing.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
thing.o: /usr/include/sys/select.h /usr/include/bits/select.h
thing.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
thing.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
thing.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
thing.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
thing.o: /usr/include/log4cpp/Priority.hh /usr/include/log4cpp/Layout.hh
thing.o: /usr/include/log4cpp/LoggingEvent.hh
thing.o: /usr/include/log4cpp/TimeStamp.hh
thing.o: /usr/include/log4cpp/threading/Threading.hh
thing.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
thing.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
thing.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
thing.o: /usr/include/pthread.h /usr/include/sched.h
thing.o: /usr/include/bits/sched.h /usr/include/signal.h
thing.o: /usr/include/bits/setjmp.h /usr/include/assert.h
thing.o: /usr/include/log4cpp/CategoryStream.hh
thing.o: /usr/include/log4cpp/Manipulator.hh
thing.o: /usr/include/log4cpp/convenience.h regex.h /usr/include/pcre.h
thing.o: /usr/include/stdlib.h /usr/include/alloca.h debug.h split.h lang.h
thing.o: trans.h keyword.h
trans.o: trans.h /usr/include/pcre.h /usr/include/stdlib.h
trans.o: /usr/include/features.h /usr/include/sys/cdefs.h
trans.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
trans.o: /usr/include/gnu/stubs-64.h /usr/include/sys/types.h
trans.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
trans.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
trans.o: /usr/include/sys/select.h /usr/include/bits/select.h
trans.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
trans.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
trans.o: /usr/include/alloca.h regex.h debug.h
trans.o: /usr/include/log4cpp/Category.hh /usr/include/log4cpp/Portability.hh
trans.o: /usr/include/log4cpp/config.h /usr/include/log4cpp/Export.hh
trans.o: /usr/include/log4cpp/Appender.hh /usr/include/sys/stat.h
trans.o: /usr/include/bits/stat.h /usr/include/fcntl.h
trans.o: /usr/include/bits/fcntl.h /usr/include/log4cpp/Priority.hh
trans.o: /usr/include/log4cpp/Layout.hh /usr/include/log4cpp/LoggingEvent.hh
trans.o: /usr/include/log4cpp/TimeStamp.hh
trans.o: /usr/include/log4cpp/threading/Threading.hh
trans.o: /usr/include/log4cpp/threading/PThreads.hh /usr/include/stdio.h
trans.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
trans.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
trans.o: /usr/include/pthread.h /usr/include/sched.h
trans.o: /usr/include/bits/sched.h /usr/include/signal.h
trans.o: /usr/include/bits/setjmp.h /usr/include/assert.h
trans.o: /usr/include/log4cpp/CategoryStream.hh
trans.o: /usr/include/log4cpp/Manipulator.hh
trans.o: /usr/include/log4cpp/convenience.h
