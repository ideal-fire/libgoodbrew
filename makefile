src = base.c graphics.c images.c paths.c useful.c text.c controls.c sound.c
obj = $(src:.c=.o)

CFLAGS = -g -I. -I./SDL/ `pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf`
OUTNAME = libgoodbrew.a

PREFIX = /usr/local

$(OUTNAME): $(obj)
	ar rcs $(OUTNAME) $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(OUTNAME)

install: $(OUTNAME)
	mkdir -p $(DESTDIR)$(PREFIX)/include/goodbrew
	cp ./goodbrew/* $(DESTDIR)$(PREFIX)/include/goodbrew/
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp $(OUTNAME) $(DESTDIR)$(PREFIX)/lib/
	@echo installed
