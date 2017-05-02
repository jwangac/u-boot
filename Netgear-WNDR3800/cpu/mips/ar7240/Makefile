include $(TOPDIR)/config.mk

LIB	= lib$(SOC).a

START	=
#OBJS	= ar7240_serial.o ag7240.o ar7240_flash.o meminit.o
OBJS	= meminit.o 
SOBJS	=

ifeq ($(BOARD), ap121)
OBJS	+= hornet_serial.o
SOBJS	+= hornet_ddr_init.o
else
ifeq ($(BOARD), wnr1000v4)
OBJS	+= hornet_serial.o
SOBJS	+= hornet_ddr_init.o
else
ifeq ($(BOARD), hw29763847p16p64)
OBJS	+= hornet_serial.o
SOBJS	+= hornet_ddr_init.o
else
OBJS	+= ar7240_serial.o
endif
endif
endif

ifeq ($(BOARD), db12x)
OBJS    += ag934x.o
else
ifeq ($(BOARD), wndr3700v3)
OBJS    += ag934x.o
else
OBJS    += ag7240.o
endif
endif
all:	.depend $(START) $(LIB)

$(LIB):	$(OBJS) $(SOBJS) $(OBJS-0)
	$(AR) crv $@ $(OBJS) $(SOBJS)

#########################################################################

.depend:	Makefile $(START:.o=.S) $(OBJS:.o=.c)
		$(CC) -M $(CFLAGS) $(START:.o=.S) $(OBJS:.o=.c) > $@

sinclude .depend

#########################################################################
