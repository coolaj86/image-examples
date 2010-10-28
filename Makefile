CC_OPTS = -Wall -Werror
CC += $(CC_OPTS)

all: bin/rgb565tobmp bin/rgb565toppm
clean: rm bin/ -rf

bin/rgb565tobmp: src/rgb565tobmp.c bin
	@$(CC) -o bin/rgb565tobmp -lbmp src/rgb565tobmp.c >/dev/null 2>/dev/null || \
    echo ""; echo "#### rgb565tobmp requires libbmp from code.google.com. Skipping... ####"; echo ""

bin/rgb565toppm: src/rgb565toppm.c bin
	@$(CC) -o bin/rgb565toppm src/rgb565toppm.c && echo "Built rgb565toppm."

bin:
	@mkdir bin
