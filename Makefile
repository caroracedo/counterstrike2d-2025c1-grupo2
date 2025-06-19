.PHONY: all test clean editor client common server build compile-debug run-tests install uninstall check-os

LIME   := \033[38;2;138;206;0m
RESET  := \033[0m

SYSTEM_DEPENDENCIES = \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev \
	libsdl2-gfx-dev \
    qt6-base-dev \
    qt6-base-dev-tools

compile-debug:
	@echo "$(LIME)Compilando en modo debug...$(RESET)"
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

run-tests: compile-debug
	@echo "$(LIME)Ejecutando los tests...$(RESET)"
	./build/taller_tests

all: clean run-tests

clean:
	@echo "$(LIME)Limpiando archivos de compilación...$(RESET)"
	rm -Rf build/

check-os:
	@echo "$(LIME)Verificando el sistema operativo...$(RESET)"
	@if grep -q 'VERSION_ID="24.04"' /etc/os-release; then \
		echo "$(LIME)Sistema operativo compatible.$(RESET)"; \
	else \
		echo "$(LIME)Este proyecto recomienda Ubuntu 24.04 para compatibilidad.$(RESET)"; \
		false; \
	fi

install: check-os
	@echo "$(LIME)Instalando dependencias del proyecto...$(RESET)"
	@for pkg in $(SYSTEM_DEPENDENCIES); do \
		if dpkg -s $$pkg >/dev/null 2>&1; then \
			echo "$(LIME)$$pkg ya está instalado.$(RESET)"; \
		else \
			echo "$(LIME)Instalando $$pkg...$(RESET)"; \
			sudo apt-get install -y $$pkg; \
		fi; \
	done

uninstall:
	@echo "$(LIME)Desinstalando dependencias del proyecto...$(RESET)"
	@for pkg in $(SYSTEM_DEPENDENCIES); do \
		if dpkg -s $$pkg >/dev/null 2>&1; then \
			echo "$(LIME)Desinstalando $$pkg...$(RESET)"; \
			sudo apt-get remove -y $$pkg; \
		else \
			echo "$(LIME)$$pkg no estaba instalado.$(RESET)"; \
		fi; \
	done
