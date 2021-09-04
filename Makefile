CC := g++-11

TARGET := rise-test

SRCDIR := .
INCDIR := .
BUILDDIR := Build
TARGETDIR := .
SRCEXT := cpp
DEPEXT := d
OBJEXT := o

CFLAGS := -g -Wall -std=c++20 -DBT_USE_DOUBLE_PRECISION -Wno-unknown-pragmas
LIB := -pthread -lpqxx -lpq -lBullet3Collision-float64 -lBullet3Common-float64 -lBullet3Dynamics-float64 -lBullet3Geometry-float64 -lBullet3OpenCL_clew-float64 -lBulletCollision-float64 -lBulletDynamics-float64 -lBulletSoftBody-float64 -lLinearMath-float64
INC := -I"$(INCDIR)" -I/usr/include/bullet

SOURCES := $(shell find $(SRCDIR) -type f -name \*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILDDIR)

clean:
	@$(RM) -rf $(BUILDDIR)

-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

rise-test: $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

rise-server: $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INC) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

.PHONY: all clean directories