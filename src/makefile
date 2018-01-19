main: pcsd pcs
	@echo Build Completed.
	
install: cleanup_prev_install pcs-server pcs-client
	mv pcsd /bin
	mv pcs /bin
	
pcsd: server.c
	gcc -pthread -o pcsd server.c -lpigpiod_if2 -lrt
	
pcs: client.c
	gcc -o pcs client.c
	
cleanup_prev_install: 
	if[ -e /bin/pcs-server ]
		@echo Previous version of pcs-server found.
		rm /bin/pcs-server
	fi
	
	if[ -e /bin/pcsd ]
		@echo Previous version of pcs-server found.
		rm /bin/pcsd
	fi
	
	if[ -e /bin/pcs-client ]
		@echo Previous version of pcs-client found.
		rm /bin/pcs-client	
	fi
	
	if[ -e /bin/pcs ]
		@echo Previous version of pcs-client found.
		rm /bin/pcs	
	fi