src = base.c graphics.c images.c paths.c useful.c text.c controls.c
obj = $(src:.c=.o)

LDFLAGS = 
CFLAGS = -g
OUTNAME = libgoodbrew.a

$(OUTNAME): $(obj)
	ar rcs $(OUTNAME) $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(OUTNAME)