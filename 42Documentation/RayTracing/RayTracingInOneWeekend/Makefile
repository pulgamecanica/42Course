PPM = 00_ppm_image
SR = 01_simple_rays
SS = 02_simple_sphere
SOS = 03_some_spheres
CA = 04_camera_abstraction

all: ex00 ex01 ex02 ex03 ex04

ex00:
	make test -C ${PPM}

ex01:
	make test -C ${SR}

ex02:
	make test -C ${SS}

ex03:
	make test -C ${SOS}

ex04:
	make test -C ${CA}

fclean:
	make fclean -C ${PPM}
	make fclean -C ${SR}
	make fclean -C ${SS}
	make fclean -C ${SOS}
	make fclean -C ${CA}
	@rm *.ppm

.PHONY: ex00 ex01 ex02 ex03 ex04
