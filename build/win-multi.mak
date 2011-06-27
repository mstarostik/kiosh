# This makefile is taylored to cross-compiling from Linux
# If you have a mingw environment on Windows it might work, too
# If you use a different toolchain, feel free to contribute a
# more generic makefile

ABIS := i686 x86_64

ifeq ($(V),1)
Q :=
else
Q := @
endif

i686_CHOST := i686-pc-mingw32-
x86_64_CHOST := x86_64-pc-mingw32-

CXX := g++
LD := g++

SRCDIR := src
BINDIR := bin

.PHONY: all clean binaries

all: binaries

clean:
	rm -rf $(BINDIR)

define GENERIC_BINARY
# 1 = abi
# 2 = output basename

$1_$2_OBJS := $$(addprefix $(BINDIR)/$1/,$$($2_SOURCES:%.cpp=%.o))
ifdef $2_CXXFLAGS
$1_$2_CXXFLAGS := $$($2_CXXFLAGS)
else
$1_$2_CXXFLAGS := $(CXXFLAGS)
endif
ifdef $2_LDFLAGS
$1_$2_LDFLAGS := $$($2_LDFLAGS)
else
$1_$2_LDFLAGS := $(LDFLAGS)
endif

BINARY_TARGETS += $$(BINDIR)/$1/$2

$(BINDIR)/$1/$2: $$($1_$2_OBJS) $(MAKEFILE_LIST)
	@mkdir -p $$(@D)
	@printf "%-8s LD   %s\n" "$1" "$$(@F)"
	$(Q)$($1_CHOST)$(LD) -o $$@ $$($1_$2_LDFLAGS) $$($1_$2_OBJS)

$$($1_$2_OBJS): $(BINDIR)/$1/%.o: $(SRCDIR)/%.cpp $(MAKEFILE_LIST)
	@mkdir -p $$(@D)
	@printf "%-8s C++  %s\n" "$1" "$$(@F)"
	$(Q)$($1_CHOST)$(CXX) -c -MMD -o $$@ $$($1_$2_CXXFLAGS) $$<

-include $$($1_$2_OBJS:%.o=%.d)
endef

$(foreach abi,$(ABIS),$(foreach binary,$(BINARIES),$(eval $(call GENERIC_BINARY,$(abi),$(binary)))))

binaries: $(BINARY_TARGETS)
