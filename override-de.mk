#
# misc.mk
#
MENUGFXDIR := graphics/interface
MISCGFXDIR := graphics/misc

$(MENUGFXDIR)/wordgroup_frame.4bpp: $(MENUGFXDIR)/wordgroup_frame.png
	$(GBAGFX) $< $@ -num_tiles 76

$(MISCGFXDIR)/end_copyright.4bpp: $(MISCGFXDIR)/end_copyright.png
	$(GBAGFX) $< $@ -num_tiles 92

#
# tilesets.mk
#
TILESETDIR := data/tilesets

$(TILESETDIR)/secondary/shop/tiles.4bpp: $(TILESETDIR)/secondary/shop/tiles.png
	$(GBAGFX) $< $@ -num_tiles 503
