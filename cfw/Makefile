# Custom firmware discs, each a separate copy of the NAV_SW disc (../cd/carin_nav_sw.img):
#   make poc     -> ../cd/carin_poc.img/.cue/.chd       (hello world, keys, pi, RAM/video benchmark)
#   make doom    -> ../cd/carminat_doom.img/.cue/.chd   (Doom, Freedoom E1M1)
#   make         -> both
#   make clean
all: poc doom

poc:
	$(MAKE) -C poc chd

doom:
	$(MAKE) -C carminat_doom chd

clean:
	$(MAKE) -C poc clean
	$(MAKE) -C carminat_doom clean

.PHONY: all poc doom clean
