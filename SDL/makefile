src = SDL_FontCache.c
obj = $(src:.c=.o)

CFLAGS = -g -I. `pkg-config --cflags sdl2 SDL2_ttf`
OUTNAME = libSDLFontCache.a

PREFIX = /usr/local

$(OUTNAME): $(obj)
	ar rcs $(OUTNAME) $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(OUTNAME)

install: $(OUTNAME)
	mkdir -p $(DESTDIR)$(PREFIX)/include
	cp SDL_FontCache.h $(DESTDIR)$(PREFIX)/include
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp $(OUTNAME) $(DESTDIR)$(PREFIX)/lib
	@echo installed
