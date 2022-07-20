LD = alpha-linux-gnu-ld
AS = alpha-linux-gnu-as
OBJCOPY = alpha-linux-gnu-objcopy

TARGETS = matrixvecmul.bin
ELFTARGETS = $(subst .bin,.elf,$(TARGETS))

.PRECIOUS : %.o %.elf

all: $(TARGETS)

%.bin:		%.elf
		$(OBJCOPY) -O ecoff-littlealpha $< $@

%.elf:		%.o minicrt-alpha.o
		$(LD) -O3 --no-eh-frame-hdr -m elf64alpha -o $@ $^

%.o:		%.S
		$(AS) -o $@ $<


clean:
	rm -f $(TARGETS)
	rm -f *o
	rm -f $(ELFTARGETS)
