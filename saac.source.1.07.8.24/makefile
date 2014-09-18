#
# Edit by ringo 19980606
# 19990723
#
#
#

CC = gcc

#加入mysql資料庫功能
CFLAGS=-march=pentium4 -w -O3 -pipe -freg-struct-return -minline-all-stringops -mmmx -msse -msse2 -I /usr/include/mysql $(INCFLAGS)
MYSQL=-L /usr/lib/mysql -lmysqlclient -lz -lm -I./

TOP=../..
PROG = saac
SHELL = /bin/sh
PERL=/usr/bin/perl

RPCGEN=${TOP}/bin/lsgen.perl

RPCHTML = ${TOP}/doc/saacproto.html
#RPCSRC = saacproto_serv.c saacproto_util.c saacproto_work.c saacproto_sup.c saacproto_lserver.c saacproto_oac.c deathcontend.c
RPCSRC = saacproto_serv.c saacproto_util.c saacproto_work.c saacproto_lserver.c saacproto_oac.c deathcontend.c
RPCINCLUDE = saacproto_serv.h saacproto_util.h saacproto_work.h saacproto_lserver.h saacproto_oac.h deathcontend.h
RPCNOUSE = saacproto_cli.c saacproto_cli.h

#加入mysql資料庫功能
SRC = main.c recv.c util.c char.c db.c mail.c acfamily.c defend.c lock.c tcpip_util.c chatroom.c auction.c sasql.c $(RPCSRC)
#SRC = main.c recv.c util.c char.c db.c mail.c acfamily.c defend.c lock.c tcpip_util.c chatroom.c auction.c $(RPCSRC)
OBJ =${SRC:.c=.o}

WKDIR=wk
WK=wk/acwk




all : ${PROG} ${WK}


${PROG} : $(RPCSRC) ${OBJ} 
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ) $(MYSQL) 

${WK} :
	make -C ${WKDIR} all

depend: $(RPCSRC)
	mv makefile makefile.bak
	sed -ne '1,/^# DO NOT DELETE THIS LINE/p' makefile.bak > makefile
	cc -M $(SRC) >> makefile 

clean:
	make -C ${WKDIR} clean;
	/bin/rm -f $(OBJ)
	/bin/rm -f *~
	/bin/rm -f stderr.*

distclean:
	/bin/rm -f $(PROG)
	/bin/rm -f $(RPCSRC) $(RPCINCLUDE) $(RPCNOUSE)
	${MAKE} clean

	mv makefile makefile.bak
	sed -ne '1,/^# DO NOT DELETE THIS LINE/p' makefile.bak > makefile
	/bin/rm -f makefile.bak

${RPCSRC} $(RPCINCLUDE): #${RPCHTML} ${RPCGEN}
#	$(PERL) ${RPCGEN} -euc ${RPCHTML} -h . -o .
#	rm -f ${RPCNOUSE}


rpc : ${RPCHTML} ${RPCGEN}
	$(PERL) ${RPCGEN} ${RPCHTML} -h . -o .
	rm -f ${RPCNOUSE}



# DO NOT DELETE THIS LINE
main.o: main.c main.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/bits/types.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/bits/stdio_lim.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/netinet/in.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/limits.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/stdint.h \
 /usr/include/bits/wordsize.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/sys/socket.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/strings.h /usr/include/bits/time.h \
 /usr/include/sys/time.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/string.h util.h mail.h db.h \
 saacproto_util.h saacproto_serv.h /usr/include/signal.h \
 /usr/include/bits/signum.h /usr/include/bits/siginfo.h \
 /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h \
 /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h \
 /usr/include/sys/wait.h /usr/include/bits/waitflags.h \
 /usr/include/bits/waitstatus.h /usr/include/malloc.h \
 /usr/include/fcntl.h /usr/include/bits/fcntl.h \
 /usr/include/netinet/tcp.h sasql.h
recv.o: recv.c main.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/bits/types.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/bits/stdio_lim.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/netinet/in.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/limits.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/stdint.h \
 /usr/include/bits/wordsize.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/sys/socket.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/strings.h /usr/include/bits/time.h \
 /usr/include/sys/time.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/string.h saacproto_util.h \
 saacproto_serv.h recv.h char.h db.h mail.h
util.o: util.c /usr/include/unistd.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/types.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h main.h /usr/include/stdio.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/bits/stdio_lim.h /usr/include/stdlib.h \
 /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
 /usr/include/bits/endian.h /usr/include/sys/select.h \
 /usr/include/bits/select.h /usr/include/bits/sigset.h \
 /usr/include/sys/sysmacros.h /usr/include/alloca.h \
 /usr/include/netinet/in.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/limits.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/stdint.h \
 /usr/include/bits/wordsize.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/sys/socket.h /usr/include/strings.h \
 /usr/include/bits/time.h /usr/include/sys/time.h \
 /usr/include/string.h util.h
char.o: char.c main.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/bits/types.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/bits/stdio_lim.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/netinet/in.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/limits.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/stdint.h \
 /usr/include/bits/wordsize.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/sys/socket.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/strings.h /usr/include/bits/time.h \
 /usr/include/sys/time.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/string.h char.h \
 saacproto_serv.h saacproto_util.h util.h sasql.h sasql.h
db.o: db.c db.h util.h main.h /usr/include/stdio.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/bits/types.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/bits/stdio_lim.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/netinet/in.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/limits.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/stdint.h \
 /usr/include/bits/wordsize.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/sys/socket.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/strings.h /usr/include/bits/time.h \
 /usr/include/sys/time.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/string.h /usr/include/dirent.h \
 /usr/include/bits/dirent.h /usr/include/sys/stat.h \
 /usr/include/bits/stat.h
mail.o: mail.c main.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/bits/types.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/bits/stdio_lim.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/netinet/in.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/limits.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/stdint.h \
 /usr/include/bits/wordsize.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/sys/socket.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/strings.h /usr/include/bits/time.h \
 /usr/include/sys/time.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/string.h mail.h util.h \
 saacproto_serv.h saacproto_util.h /usr/include/malloc.h \
 /usr/include/dirent.h /usr/include/bits/dirent.h \
 /usr/include/sys/stat.h /usr/include/bits/stat.h

saacproto_serv.o: saacproto_serv.c saacproto_serv.h saacproto_util.h \
 /usr/include/stdlib.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/include/sys/types.h /usr/include/bits/types.h \
 /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/string.h /usr/include/strings.h \
 /usr/include/unistd.h /usr/include/bits/posix_opt.h \
 /usr/include/bits/confname.h /usr/include/getopt.h
saacproto_util.o: saacproto_util.c /usr/include/stdio.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/bits/types.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/bits/stdio_lim.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/string.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/strings.h saacproto_util.h

saacproto_lserver.o: saacproto_lserver.c saacproto_lserver.h /usr/include/stdio.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/bits/types.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/bits/stdio_lim.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/string.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/strings.h saacproto_util.h

#加入mysql資料庫功能
sasql.o: sasql.c sasql.h main.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stddef.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/stdarg.h \
 /usr/include/bits/types.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/bits/stdio_lim.h \
 /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
 /usr/include/alloca.h /usr/include/netinet/in.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/limits.h \
 /usr/lib/gcc-lib/i386-asianux-linux/3.2.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/bits/posix1_lim.h \
 /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
 /usr/include/bits/posix2_lim.h /usr/include/stdint.h \
 /usr/include/bits/wordsize.h /usr/include/bits/socket.h \
 /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/bits/in.h \
 /usr/include/bits/byteswap.h /usr/include/arpa/inet.h \
 /usr/include/netdb.h /usr/include/rpc/netdb.h \
 /usr/include/sys/socket.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
 /usr/include/getopt.h /usr/include/strings.h /usr/include/bits/time.h \
 /usr/include/sys/time.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/string.h util.h mail.h db.h \
 saacproto_util.h saacproto_serv.h /usr/include/signal.h \
 /usr/include/bits/signum.h /usr/include/bits/siginfo.h \
 /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h \
 /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h \
 /usr/include/sys/wait.h /usr/include/bits/waitflags.h \
 /usr/include/bits/waitstatus.h /usr/include/malloc.h \
 /usr/include/fcntl.h /usr/include/bits/fcntl.h \
 /usr/include/netinet/tcp.h \
