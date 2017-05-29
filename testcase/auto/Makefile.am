noinst_PROGRAMS=test-motioncurve test-animation test-animgroup test-effector \
				test-defeffector test-animationex test-animationcb test-animationgroup
#				test-defeffector tmptest

LIBS = @LIBS@ 
#MINIGUI_LIBS = -lmgeff -lmgplus -lgtest -lstdc++
MINIGUI_LIBS = @MINIGUI_LIBS@ @MGPLUS_LIBS@  @MGEFF_LIBS@ -lgtest -lstdc++

test_motioncurve_SOURCES=motioncurve.cpp
test_motioncurve_LDADD=$(MINIGUI_LIBS)

test_animation_SOURCES=animation/animation_main.cpp \
					   animation/animation_base.cpp \
					   animation/animation_misc.cpp \
					   animation/animation_int.cpp
test_animation_LDADD=$(MINIGUI_LIBS)

test_animationgroup_SOURCES=animation/animation_main_group.cpp \
					   animation/animation_base.cpp \
					   animation/animation_misc.cpp \
					   animation/animation_group_ex.cpp
test_animationgroup_LDADD=$(MINIGUI_LIBS)

test_animationcb_SOURCES=animation/animation_main_cb.cpp \
					   animation/animation_base.cpp \
					   animation/animation_misc.cpp \
					   animation/animation_cb.cpp
test_animationcb_LDADD=$(MINIGUI_LIBS)

test_animationex_SOURCES=animation/animation_main_ex.cpp \
					   animation/animation_base.cpp \
					   animation/animation_misc.cpp \
					   animation/animation_int_ex.cpp
test_animationex_LDADD=$(MINIGUI_LIBS)

test_animgroup_SOURCES=animgroup.cpp
test_animgroup_LDADD=$(MINIGUI_LIBS)

test_effector_SOURCES=effector.cpp
test_effector_LDADD=$(MINIGUI_LIBS)

test_defeffector_SOURCES=default-effector-test.cpp
test_defeffector_LDADD=$(MINIGUI_LIBS)

#tmptest_SOURCES=m.cpp \
#					   animation/animation_base.cpp 
#tmptest_LDADD=$(MINIGUI_LIBS)

autotest:
	@for porgram in $(PROGRAMS); do \
	  echo "running $$porgram ... ..."; \
	  ./$$porgram; \
	  echo " "; \
	done; 

