CC := g++-11

SRCDIR := .
INCDIR := .
BUILDDIR := Build
TARGETDIR := .
SRCEXT := cpp
DEPEXT := d
OBJEXT := o

CFLAGS_ALL := -Wall -std=c++20 -DBT_USE_DOUBLE_PRECISION -Wno-unknown-pragmas
CFLAGS_DEBUG := -g
CFLAGS_RELEASE := -O2 -DNDEBUG
LIB := -pthread -lpqxx -lpq -lBullet3Collision-float64 -lBullet3Common-float64 -lBullet3Dynamics-float64 -lBullet3Geometry-float64 -lBullet3OpenCL_clew-float64 -lBulletCollision-float64 -lBulletDynamics-float64 -lBulletSoftBody-float64 -lLinearMath-float64
INC := -I"$(INCDIR)" -I/usr/include/bullet

SOURCES := $(shell find $(SRCDIR) -type f -name \*.$(SRCEXT))
OBJECTS_DEBUG := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/Debug/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
OBJECTS_RELEASE := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/Release/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

all: directories rise-test

directories:
	@mkdir -p $(BUILDDIR) $(TARGETDIR)/rise-test $(TARGETDIR)/rise-server

clean:
	@$(RM) -rf $(BUILDDIR)

-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

rise-test: $(OBJECTS_DEBUG)
	$(CC) -o $(TARGETDIR)/$@ $^ $(LIB)

rise-server: $(OBJECTS_RELEASE)
	$(CC) -o $(TARGETDIR)/$@ $^ $(LIB)

$(BUILDDIR)/Debug/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_ALL) $(CFLAGS_DEBUG) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS_ALL) $(CFLAGS_DEBUG) $(INC) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/Debug/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/Debug/$*.$(DEPEXT) $(BUILDDIR)/Debug/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/Debug/$*.$(OBJEXT):|' < $(BUILDDIR)/Debug/$*.$(DEPEXT).tmp > $(BUILDDIR)/Debug/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/Debug/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/Debug/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/Debug/$*.$(DEPEXT).tmp

$(BUILDDIR)/Release/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_ALL) $(CFLAGS_RELEASE) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS_ALL) $(CFLAGS_RELEASE) $(INC) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/Release/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/Release/$*.$(DEPEXT) $(BUILDDIR)/Release/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/Release/$*.$(OBJEXT):|' < $(BUILDDIR)/Release/$*.$(DEPEXT).tmp > $(BUILDDIR)/Release/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/Release/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/Release/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/Release/$*.$(DEPEXT).tmp

.PHONY: all clean directories