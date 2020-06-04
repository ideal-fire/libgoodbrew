src = base.c graphics.c images.c paths.c useful.c text.c controls.c sound.c
obj = $(src:.c=.o)

LDFLAGS = 
CFLAGS = -g -I.
OUTNAME = libgoodbrew.a

$(OUTNAME): $(obj)
	ar rcs $(OUTNAME) $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(OUTNAME)

install: $(OUTNAME)
	mkdir -p /usr/local/include/goodbrew
	cp ./goodbrew/* /usr/local/include/goodbrew/
	cp $(OUTNAME) /usr/local/lib/
	@echo installed